/********************************************************************
 * The iTCH project - Copyright (c) 2007-2011 Dustin Graves
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <QtCore/QTime>
#include <QtNetwork/QHostInfo>
#include "PiTCHNetworkDialog.h"
#include "PiTCHWindow.h"
#include "ui_PiTCHWindow.h"

namespace
{

// Create formatted time string for time elapsed/remaining display
// Formats:
//   Seconds only -> 0:ss
//   Has minutes  -> m:ss
//   Has hours    -> H:mm:ss
// Will not accept more than 24 hours
QString secondsToTimePositionString(int seconds)
{
  QTime converter = QTime().addSecs(seconds);

  if (converter.hour() > 0)
  {
    return converter.toString("H:mm:ss");
  }
  else
  {
    return converter.toString("m:ss");
  }
}

} // End of anonymous namespace

PiTCHWindow::PiTCHWindow(QWidget *parent) :
  QMainWindow(parent),
  ui_(new Ui::PiTCHWindow),
  serverInfo_(QHostInfo::localHostName(), 8049),
  buttonHeld_(false),
  buttonHeldDelay_(500),
  sequenceId_(0)
{
  ui_->setupUi(this);
  ui_->statusBar->showMessage(tr("Unconnected"));

  createStandardIcons();

  connect(&client_, SIGNAL(hostnameResolved()), this, SLOT(resolvedHostname()));
  connect(&client_, SIGNAL(connected()), this, SLOT(connectedToServer()));
  connect(&client_, SIGNAL(disconnected(bool,QString)), this, SLOT(disconnectedFromServer(bool,QString)));
  connect(&client_, SIGNAL(receivedMessage(iTCH::EnvelopePtr)), this, SLOT(processMessage(iTCH::EnvelopePtr)));
  connect(&client_, SIGNAL(error(QString)), this, SLOT(error(QString)));
}

PiTCHWindow::~PiTCHWindow()
{
  client_.closeConnection();
  delete ui_;
}

void PiTCHWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type())
  {
  case QEvent::LanguageChange:
    ui_->retranslateUi(this);
    break;
  default:
    break;
  }
}

void PiTCHWindow::resolvedHostname()
{
  // Host was found, now QTcpSocket is connecting
  ui_->statusBar->showMessage(tr("Connecting..."));
}

void PiTCHWindow::connectedToServer()
{
  ui_->statusBar->showMessage(tr("Connected"));

  // Request current track, sound volume, mute, player state, and player position
  client_.sendMessage(iTCH::MessageBuilder::makeGetCurrentTrackRequest(nextSequenceId()));
  client_.sendMessage(iTCH::MessageBuilder::makeGetSoundVolumeRequest(nextSequenceId()));
  client_.sendMessage(iTCH::MessageBuilder::makeGetMuteRequest(nextSequenceId()));
  client_.sendMessage(iTCH::MessageBuilder::makeGetPlayerStateRequest(nextSequenceId()));
  client_.sendMessage(iTCH::MessageBuilder::makeGetPlayerPositionRequest(nextSequenceId()));
}

void PiTCHWindow::disconnectedFromServer(bool closedByServer, const QString &message)
{
  QString status(tr("Unconnected"));
  ui_->statusBar->showMessage(QString("%1: %2").arg(status).arg(message));

  if (!closedByServer)
  {
    // Start auto-connect
  }
}

void PiTCHWindow::processMessage(const iTCH::EnvelopePtr envelope)
{
  // Make sure envelope contains a ServerStatus message with valid values
  if (iTCH::MessageBuilder::containsValidServerStatus(envelope))
  {
    const iTCH::ServerStatus &status = envelope->status();

    switch (status.type())
    {
    case iTCH::ServerStatus::SOUNDVOLUME:
      setSoundVolume(status.value().volume());
      break;
    case iTCH::ServerStatus::MUTE:
      setMute(status.value().mute());
      break;
    case iTCH::ServerStatus::PLAYERPOSITION:
      setPlayerPosition(status.value().position());
      break;
    case iTCH::ServerStatus::PLAYERSTATE:
      setPlayerState(status.value().state());
      break;
    case iTCH::ServerStatus::CURRENTTRACK:
      setCurrentTrack(status.value().track(0));
      break;
    }
  }
}

void PiTCHWindow::error(const QString &message)
{
   ui_->statusBar->showMessage(message.toLocal8Bit().constData(), 5);
}

void PiTCHWindow::timeSliderValueChanged(int value)
{
  client_.sendMessage(iTCH::MessageBuilder::makePutPlayerPositionRequest(nextSequenceId(), value));
}

void PiTCHWindow::backButtonPressed()
{
  connect(&buttonTimer_, SIGNAL(timeout()), this, SLOT(rewindTimeout()));
  buttonTimer_.setSingleShot(true);
  buttonTimer_.start(buttonHeldDelay_);
}

void PiTCHWindow::backButtonReleased()
{
  buttonTimer_.stop();
  buttonTimer_.disconnect();

  if (buttonHeld_)
  {
    buttonHeld_ = false;

    // Restore the player state to playing or paused, sending a play
    // or paused request doesn't seem to stop the fast forward so we
    // send pauseplay twice
    client_.sendMessage(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
    client_.sendMessage(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
  }
  else
  {
    client_.sendMessage(iTCH::MessageBuilder::makeBackTrackRequest(nextSequenceId()));
  }
}

void PiTCHWindow::rewindTimeout()
{
  buttonHeld_ = true;
  client_.sendMessage(iTCH::MessageBuilder::makeRewindRequest(nextSequenceId()));
}

void PiTCHWindow::forwardButtonPressed()
{
  connect(&buttonTimer_, SIGNAL(timeout()), this, SLOT(fastForwardTimeout()));
  buttonTimer_.setSingleShot(true);
  buttonTimer_.start(buttonHeldDelay_);
}

void PiTCHWindow::forwardButtonReleased()
{
  buttonTimer_.stop();
  buttonTimer_.disconnect();

  if (buttonHeld_)
  {
    buttonHeld_ = false;

    // Restore the player state to playing or paused, sending a play
    // or paused request doesn't seem to stop the fast forward so we
    // send pauseplay twice
    client_.sendMessage(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
    client_.sendMessage(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
  }
  else
  {
    client_.sendMessage(iTCH::MessageBuilder::makeNextTrackRequest(nextSequenceId()));
  }
}

void PiTCHWindow::fastForwardTimeout()
{
  buttonHeld_ = true;
  client_.sendMessage(iTCH::MessageBuilder::makeFastForwardRequest(nextSequenceId()));
}

void PiTCHWindow::playPauseButtonClicked()
{
  client_.sendMessage(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
}

void PiTCHWindow::minVolumeButtonClicked()
{
  ui_->volumeSlider->setValue(0);
}

void PiTCHWindow::maxVolumeButtonClicked()
{
  ui_->volumeSlider->setValue(100);
}

void PiTCHWindow::volumeSliderValueChanged(int value)
{
  client_.sendMessage(iTCH::MessageBuilder::makePutSoundVolumeRequest(nextSequenceId(), value));
}

void PiTCHWindow::networkButtonClicked()
{
  PiTCHNetworkDialog dialog(serverInfo_, this);

  if (QDialog::Accepted == dialog.exec())
  {
    // Attempt the server connection
    ui_->statusBar->showMessage(tr("Looking up host..."));
    client_.openConnection(dialog.getNetworkInfo());
  }
}

void PiTCHWindow::setSoundVolume(int newVolume)
{
  ui_->volumeSlider->blockSignals(true);
  ui_->volumeSlider->setValue(newVolume);
  ui_->volumeSlider->blockSignals(false);
}

void PiTCHWindow::setMute(bool isMute)
{
  if (isMute)
  {
    // We interpret mute as setting volume to 0
    setSoundVolume(0);
  }
}

void PiTCHWindow::setPlayerPosition(int newPosition)
{
  ui_->timeSlider->blockSignals(true);
  ui_->timeSlider->setValue(newPosition);
  ui_->timeSlider->blockSignals(false);

  ui_->timeElapsed->setText(secondsToTimePositionString(newPosition));
  ui_->timeRemaining->setText(QString("-%1").arg(secondsToTimePositionString(currentTrack_.duration() - newPosition)));
}

void PiTCHWindow::setPlayerState(iTCH::PlayerState newState)
{
  if (newState == iTCH::PLAYING)
  {
    ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
  }
  else if (newState == iTCH::STOPPED)
  {
    ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
  }
}

void PiTCHWindow::setCurrentTrack(const iTCH::Track &track)
{
  currentTrack_.CopyFrom(track);
  ui_->songTitle->setText(track.name().c_str());
  ui_->artist->setText(QString("%1 -- %2")
    .arg(track.artist().c_str())
    .arg(track.album().c_str()));
  ui_->timeSlider->setMaximum(track.duration());
}

void PiTCHWindow::createStandardIcons()
{
  ui_->backButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
  ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
  ui_->forwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
  ui_->minVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
  ui_->maxVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
  ui_->networkButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
}

unsigned long PiTCHWindow::nextSequenceId()
{
  return ++sequenceId_;
}
