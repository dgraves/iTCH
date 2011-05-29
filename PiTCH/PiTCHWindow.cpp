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
#include <QtNetwork/QHostInfo>
#include "PiTCHNetworkDialog.h"
#include "PiTCHWindow.h"
#include "ui_PiTCHWindow.h"

PiTCHWindow::PiTCHWindow(QWidget *parent) :
  QMainWindow(parent),
  ui_(new Ui::PiTCHWindow),
  serverInfo_(QHostInfo::localHostName(), 8049),
  buttonHeld_(false),
  sequenceId_(0)
{
  ui_->setupUi(this);
  ui_->statusBar->showMessage(tr("Unconnected"));

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
  // Start a timer to detect if button is held for fast forward mode
}

void PiTCHWindow::backButtonReleased()
{
  if (buttonHeld_)
  {
    buttonHeld_ = false;
  }
  else
  {
    client_.sendMessage(iTCH::MessageBuilder::makeBackTrackRequest(nextSequenceId()));
  }
}

void PiTCHWindow::forwardButtonPressed()
{
  // Start a timer to detect if button is held for fast forward mode
}

void PiTCHWindow::forwardButtonReleased()
{
  if (buttonHeld_)
  {
    buttonHeld_ = false;
  }
  else
  {
    client_.sendMessage(iTCH::MessageBuilder::makeNextTrackRequest(nextSequenceId()));
  }
}

void PiTCHWindow::playPauseButtonClicked()
{
  client_.sendMessage(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
}

void PiTCHWindow::muteButtonClicked()
{
  ui_->volumeSlider->setSliderPosition(0);
}

void PiTCHWindow::fullVolumeButtonClicked()
{
  ui_->volumeSlider->setSliderPosition(100);
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

unsigned long PiTCHWindow::nextSequenceId()
{
  return ++sequenceId_;
}
