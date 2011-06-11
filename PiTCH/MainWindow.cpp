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
#include "NetworkDialog.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace
{
  // Default timer values
  enum
  {
    SERVER_PORT             = 8049,
    AUTOCONNECT_INTERVAL    = 10,   // Seconds
    BUTTON_HELD_DELAY       = 500,  // Milliseconds
    POSITION_INTERVAL       = 1000, // Milliseconds
    POSITION_INTERVAL_RAPID = 250   // Milliseconds
  };

  // Form indexes for stacked widget
  enum
  {
    BANNER_FORM = 0,
    TRACKINFO_FORM = 1
  };

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

  QString requestTypeToString(iTCH::ClientRequest::Type type)
  {
    switch (type)
    {
    case iTCH::ClientRequest::BACKTRACK:
      return "Back track";
    case iTCH::ClientRequest::FASTFORWARD:
      return "Fast forward";
    case iTCH::ClientRequest::NEXTTRACK:
      return "Next track";
    case iTCH::ClientRequest::PAUSE:
      return "Pause";
    case iTCH::ClientRequest::PLAY:
      return "Play";
    case iTCH::ClientRequest::PLAYPAUSE:
      return "Toggle play-pause";
    case iTCH::ClientRequest::PREVIOUSTRACK:
      return "Previous track";
    case iTCH::ClientRequest::RESUME:
      return "Resume";
    case iTCH::ClientRequest::REWIND:
      return "Rewind";
    case iTCH::ClientRequest::STOP:
      return "Stop";
    case iTCH::ClientRequest::GET_SOUNDVOLUME:
      return "Get volume";
    case iTCH::ClientRequest::PUT_SOUNDVOLUME:
      return "Set volume";
    case iTCH::ClientRequest::GET_MUTE:
      return "Get mute";
    case iTCH::ClientRequest::PUT_MUTE:
      return "Set mute";
    case iTCH::ClientRequest::GET_PLAYERPOSITION:
      return "Get player position";
    case iTCH::ClientRequest::PUT_PLAYERPOSITION:
      return "Set player position";
    case iTCH::ClientRequest::GET_PLAYERSTATE:
      return "Get player state";
    case iTCH::ClientRequest::GET_CURRENTTRACK:
      return "Get current track";
    case iTCH::ClientRequest::GET_CURRENTPLAYLIST:
      return "Get current playlist";
    default:
      return "Unrecognized";
    }
  }
} // End of anonymous namespace

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui_(new Ui::MainWindow),
  serverInfo_(QHostInfo::localHostName(), SERVER_PORT),
  autoConnect_(false),
  autoConnectInterval_(AUTOCONNECT_INTERVAL),
  buttonHeld_(false),
  buttonHeldDelay_(BUTTON_HELD_DELAY),
  sequenceId_(0),
  positionInterval_(POSITION_INTERVAL),
  positionIntervalRapid_(POSITION_INTERVAL_RAPID),
  playing_(false),
  playerDisconnected_(false)
{
  ui_->setupUi(this);
  ui_->statusBar->showMessage(tr("Unconnected"));

  QIcon icon = QIcon(":/app/icons/reco.svg");
  setWindowIcon(icon);

  createStandardIcons();
  setDisconnectedState(false);

  connect(&client_, SIGNAL(hostnameResolved()), this, SLOT(resolvedHostname()));
  connect(&client_, SIGNAL(connected()), this, SLOT(connectedToServer()));
  connect(&client_, SIGNAL(disconnected(bool,QString)), this, SLOT(disconnectedFromServer(bool,QString)));
  connect(&client_, SIGNAL(receivedMessage(iTCH::EnvelopePtr)), this, SLOT(processMessage(iTCH::EnvelopePtr)));
  connect(&client_, SIGNAL(protocolError(QString)), this, SLOT(processProtocolError(QString)));
}

MainWindow::~MainWindow()
{
  client_.closeConnection();
  delete ui_;
}

void MainWindow::changeEvent(QEvent *e)
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

void MainWindow::resolvedHostname()
{
  // Host was found, now QTcpSocket is connecting
  ui_->statusBar->showMessage(tr("Connecting..."));
}

void MainWindow::connectedToServer()
{
  ui_->statusBar->showMessage(QString("%1 %2")
    .arg(tr("Connected to"))
    .arg(serverInfo_.getHostname()));

  setConnectedState();

  ui_->networkButton->blockSignals(true);
  ui_->networkButton->setChecked(true);
  ui_->networkButton->blockSignals(false);

  // Request current track, sound volume, mute, player state, and player position
  sendTrackedRequest(iTCH::MessageBuilder::makeGetCurrentTrackRequest(nextSequenceId()));
  sendTrackedRequest(iTCH::MessageBuilder::makeGetSoundVolumeRequest(nextSequenceId()));
  sendTrackedRequest(iTCH::MessageBuilder::makeGetMuteRequest(nextSequenceId()));
  sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerStateRequest(nextSequenceId()));
  sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerButtonsStateRequest(nextSequenceId()));
  sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerPositionRequest(nextSequenceId()));
}

void MainWindow::disconnectedFromServer(bool closedByHost, const QString &message)
{
  QString status(tr("Unconnected"));

  setPlayerState(false);
  setDisconnectedState(false);

  ui_->networkButton->blockSignals(true);
  ui_->networkButton->setChecked(false);
  ui_->networkButton->blockSignals(false);

  if (closedByHost)
  {
    ui_->statusBar->showMessage(status);
  }
  else
  {
    ui_->statusBar->showMessage(QString("%1: %2").arg(status).arg(message));

    // Start auto-connect
    if (autoConnect_)
    {
      startAutoConnect();
    }
  }
}

void MainWindow::processMessage(const iTCH::EnvelopePtr envelope)
{
  // If the player was previously disconnected from server, and has now reconnected we need to
  // renable our state
  if (playerDisconnected_)
  {
    playerDisconnected_ = false;

    setConnectedState();

    // Request current track, sound volume, mute, player state, and player position
    sendTrackedRequest(iTCH::MessageBuilder::makeGetCurrentTrackRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetSoundVolumeRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetMuteRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerStateRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerButtonsStateRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerPositionRequest(nextSequenceId()));
  }

  // Determine message type
  switch (envelope->type())
  {
  case iTCH::Envelope::SERVERNOTIFICATION:
    processNotification(envelope);
    break;
  case iTCH::Envelope::SERVERRESPONSE:
    processResponse(envelope);
    break;
  default:
    processProtocolError("Received unrecognized message from server");
  }
}

void MainWindow::processNotification(iTCH::EnvelopePtr envelope)
{
  assert(envelope->has_notification());
  switch (envelope->notification().type())
  {
  case iTCH::ServerNotification::VOLUMECHANGED:
    // Get the volume/mute status
    sendTrackedRequest(iTCH::MessageBuilder::makeGetSoundVolumeRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetMuteRequest(nextSequenceId()));
    break;
  case iTCH::ServerNotification::PLAYINGSTARTED:
    // Get the initial time slider position, current buttons state, and current track
    setPlayerState(true);
    sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerPositionRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerButtonsStateRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetCurrentTrackRequest(nextSequenceId()));
    break;
  case iTCH::ServerNotification::PLAYINGSTOPPED:
    // Get the final time slider position, current buttons state, and current track
    setPlayerState(false);
    sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerPositionRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerButtonsStateRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makeGetCurrentTrackRequest(nextSequenceId()));
    break;
  case iTCH::ServerNotification::TRACKINFOCHANGED:
    sendTrackedRequest(iTCH::MessageBuilder::makeGetCurrentTrackRequest(nextSequenceId()));
    break;
  case iTCH::ServerNotification::PLAYERQUITTING:
    // Just stop player without sending requests, requests can cause server to start the player again making it impossible to close the player while the server is active
    playerDisconnected_ = true;
    setPlayerState(false);
    setDisconnectedState(true);
    break;
  default:
    processProtocolError("Received unrecognized notification type");
    break;
  }
}

void MainWindow::processResponse(iTCH::EnvelopePtr envelope)
{
  if (envelope->has_response())
  {
    bool isValid = false;
    unsigned int seqid = envelope->response().seqid();

    // Find the original request
    PendingRequests::const_iterator iter = requests_.find(seqid);
    if (iter == requests_.end())
    {
      processProtocolError("Server sent an unrequested response");
    }
    else
    {
      assert(iter.value()->has_request());
      const iTCH::ClientRequest &request = iter.value()->request();

      // Make sure envelope contains a server response message with valid values
      if (iTCH::MessageBuilder::containsValidServerResponse(envelope, request))
      {
        const iTCH::ServerResponse &response = envelope->response();
        if (!response.success())
        {
          // Report failure of a request
          processProtocolError(QString("%1 request failed: %2")
            .arg(requestTypeToString(request.type()))
            .arg(response.error_message().c_str()));

          // Apply default value to get request type
          switch (request.type())
          {
          case iTCH::ClientRequest::GET_SOUNDVOLUME:
            setSoundVolume(0);
            break;
          case iTCH::ClientRequest::GET_MUTE:
            setMute(false);
            break;
          case iTCH::ClientRequest::GET_PLAYERPOSITION:
            setPlayerPosition(0);
            break;
          case iTCH::ClientRequest::GET_PLAYERSTATE:
            setPlayerState(false);
            break;
          case iTCH::ClientRequest::GET_CURRENTTRACK:
            setCurrentTrack(iTCH::Track());
            break;
          case iTCH::ClientRequest::GET_PLAYERBUTTONSSTATE:
            setPlayerButtonsState(iTCH::PlayerButtonsState());
            break;
          }
        }
        else
        {
          switch (response.value().type())
          {
          case iTCH::ServerResponse::Value::VOLUME:
            setSoundVolume(response.value().volume());
            break;
          case iTCH::ServerResponse::Value::MUTE:
            setMute(response.value().mute());
            break;
          case iTCH::ServerResponse::Value::POSITION:
            setPlayerPosition(response.value().position());
            break;
          case iTCH::ServerResponse::Value::STATE:
            setPlayerState(response.value().state() == iTCH::PLAYING);
            break;
          case iTCH::ServerResponse::Value::TRACK:
            setCurrentTrack(response.value().track());
            break;
          case iTCH::ServerResponse::Value::BUTTONS:
            setPlayerButtonsState(response.value().buttons());
            break;
          default:
            processProtocolError("Received unrecognized notification type");
            break;
          }
        }
      }

      // Remove the request from the pending request list, it has been processed
      requests_.remove(seqid);
    }
  }
}

void MainWindow::processProtocolError(const QString &message)
{
}

void MainWindow::timeSliderPressed()
{
  // Stop the position timer while dragging the slider
  if (playing_)
  {
    stopPositionTimer();
  }
}

void MainWindow::timeSliderReleased()
{
  // Stop the position timer while dragging the slider
  if (playing_)
  {
    startPositionTimer(positionInterval_);
  }
}

void MainWindow::timeSliderValueChanged(int value)
{
  sendTrackedRequest(iTCH::MessageBuilder::makePutPlayerPositionRequest(nextSequenceId(), value));
  setPlayerPosition(value);
}

void MainWindow::backButtonPressed()
{
  connect(&buttonTimer_, SIGNAL(timeout()), this, SLOT(rewindTimeout()));
  buttonTimer_.setSingleShot(true);
  buttonTimer_.start(buttonHeldDelay_);
}

void MainWindow::backButtonReleased()
{
  buttonTimer_.stop();
  buttonTimer_.disconnect();

  if (buttonHeld_)
  {
    buttonHeld_ = false;

    // Reset timer state
    if (playing_)
    {
      positionTimer_.setInterval(positionInterval_);
    }
    else
    {
      stopPositionTimer();
    }

    // Restore the player state to playing or paused, sending a play
    // or paused request doesn't seem to stop the fast forward so we
    // send pauseplay twice
    sendTrackedRequest(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
  }
  else
  {
    sendTrackedRequest(iTCH::MessageBuilder::makeBackTrackRequest(nextSequenceId()));

    // Need to update the slider and track information if player is stopped
    if (!playing_)
    {
      sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerPositionRequest(nextSequenceId()));
      sendTrackedRequest(iTCH::MessageBuilder::makeGetCurrentTrackRequest(nextSequenceId()));
    }
  }
}

void MainWindow::rewindTimeout()
{
  buttonHeld_ = true;
  sendTrackedRequest(iTCH::MessageBuilder::makeRewindRequest(nextSequenceId()));

  // Increase the position request rate to update time slider position during fast forward
  // If player is not playing, the timer needs to be started
  if (playing_)
  {
    positionTimer_.setInterval(positionIntervalRapid_);
  }
  else
  {
    startPositionTimer(positionIntervalRapid_);
  }
}

void MainWindow::forwardButtonPressed()
{
  connect(&buttonTimer_, SIGNAL(timeout()), this, SLOT(fastForwardTimeout()));
  buttonTimer_.setSingleShot(true);
  buttonTimer_.start(buttonHeldDelay_);
}

void MainWindow::forwardButtonReleased()
{
  buttonTimer_.stop();
  buttonTimer_.disconnect();

  if (buttonHeld_)
  {
    buttonHeld_ = false;

    // Reset timer state
    if (playing_)
    {
      positionTimer_.setInterval(positionInterval_);
    }
    else
    {
      stopPositionTimer();
    }

    // Restore the player state to playing or paused, sending a play
    // or paused request doesn't seem to stop the fast forward so we
    // send pauseplay twice
    sendTrackedRequest(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
    sendTrackedRequest(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
  }
  else
  {
    sendTrackedRequest(iTCH::MessageBuilder::makeNextTrackRequest(nextSequenceId()));

    // Need to update the slider and track information if player is stopped
    if (!playing_)
    {
      sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerPositionRequest(nextSequenceId()));
      sendTrackedRequest(iTCH::MessageBuilder::makeGetCurrentTrackRequest(nextSequenceId()));
    }
  }
}

void MainWindow::fastForwardTimeout()
{
  buttonHeld_ = true;
  sendTrackedRequest(iTCH::MessageBuilder::makeFastForwardRequest(nextSequenceId()));

  // Increase the position request rate to update time slider position during fast forward
  // If player is not playing, the timer needs to be started
  if (playing_)
  {
    positionTimer_.setInterval(positionIntervalRapid_);
  }
  else
  {
    startPositionTimer(positionIntervalRapid_);
  }
}

void MainWindow::playPauseButtonClicked()
{
  sendTrackedRequest(iTCH::MessageBuilder::makePlayPauseRequest(nextSequenceId()));
}

void MainWindow::minVolumeButtonClicked()
{
  ui_->volumeSlider->setValue(0);
}

void MainWindow::maxVolumeButtonClicked()
{
  ui_->volumeSlider->setValue(100);
}

void MainWindow::volumeSliderValueChanged(int value)
{
  sendTrackedRequest(iTCH::MessageBuilder::makePutSoundVolumeRequest(nextSequenceId(), value));
}

void MainWindow::networkButtonToggled(bool isChecked)
{
  if (isChecked)
  {
    // Stop auto connect if it is active
    stopAutoConnect();

    NetworkDialog dialog(serverInfo_, autoConnect_, autoConnectInterval_, this);

    if (QDialog::Accepted == dialog.exec())
    {
      serverInfo_ = dialog.getNetworkInfo();
      autoConnect_ = dialog.getAutoConnect();
      autoConnectInterval_ = dialog.getAutoConnectInterval();

      // Attempt the server connection
      openConnection();
    }
    else
    {
      // Reset checked state
      ui_->networkButton->blockSignals(true);
      ui_->networkButton->setChecked(false);
      ui_->networkButton->blockSignals(false);
    }
  }
  else
  {
    client_.closeConnection();
  }
}

void MainWindow::setSoundVolume(int newVolume)
{
  ui_->volumeSlider->blockSignals(true);
  ui_->volumeSlider->setValue(newVolume);
  ui_->volumeSlider->blockSignals(false);
}

void MainWindow::setMute(bool isMute)
{
  if (isMute)
  {
    // We interpret mute as setting volume to 0
    setSoundVolume(0);
  }
}

void MainWindow::setPlayerPosition(int newPosition)
{
  // Don't update slider while it is being dragged
  if (!ui_->timeSlider->isSliderDown())
  {
    ui_->timeSlider->blockSignals(true);
    ui_->timeSlider->setValue(newPosition);
    ui_->timeSlider->blockSignals(false);
  }

  ui_->timeElapsed->setText(secondsToTimePositionString(newPosition));
  ui_->timeRemaining->setText(QString("-%1").arg(secondsToTimePositionString(currentTrack_.duration() - newPosition)));
}

void MainWindow::setPlayerState(bool playing)
{
  if (playing_ != playing)
  {
    if (playing)
    {
      startPositionTimer(positionInterval_);
    }
    else
    {
      stopPositionTimer();
    }

    playing_ = playing;
  }
}

void MainWindow::setPlayerButtonsState(const iTCH::PlayerButtonsState &buttons)
{
  ui_->backButton->setEnabled(buttons.previous_enabled());
  ui_->forwardButton->setEnabled(buttons.next_enabled());

  switch (buttons.play_pause_stop_state())
  {
  case iTCH::PLAY_ENABLED:
    ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui_->playPauseToggleButton->setEnabled(true);
    break;
  case iTCH::PAUSE_ENABLED:
    ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui_->playPauseToggleButton->setEnabled(true);
    break;
  case iTCH::PAUSE_DISABLED:
    ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui_->playPauseToggleButton->setEnabled(false);
    break;
  case iTCH::STOP_ENABLED:
    ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui_->playPauseToggleButton->setEnabled(true);
    break;
  case iTCH::STOP_DISABLED:
    ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui_->playPauseToggleButton->setEnabled(false);
    break;
  case iTCH::PLAY_DISABLED:
  default:
    ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui_->playPauseToggleButton->setEnabled(false);
  }
}

void MainWindow::setCurrentTrack(const iTCH::Track &track)
{
  currentTrack_.CopyFrom(track);

  // Build the song title string from track name and album name
  QString songTitle;
  if (!track.artist().empty() && !track.album().empty())
  {
    songTitle = QString("%1 -- %2")
      .arg(track.artist().c_str())
      .arg(track.album().c_str());
  }
  else if (!track.artist().empty())
  {
    songTitle = track.artist().c_str();
  }
  else if (!track.album().empty())
  {
    songTitle = track.album().c_str();
  }

  // If no song title or artist name specified, show the banner
  if (songTitle.isEmpty() && track.artist().empty())
  {
    ui_->trackInfoDisplay->setCurrentIndex(BANNER_FORM);
  }
  else
  {
    ui_->trackInfoDisplay->setCurrentIndex(TRACKINFO_FORM);
    ui_->trackInfo->setSongTitle(track.name().c_str());
    ui_->trackInfo->setArtist(songTitle);
  }

  ui_->timeSlider->setMaximum(track.duration());
}

void MainWindow::requestPlayerPosition()
{
  sendTrackedRequest(iTCH::MessageBuilder::makeGetPlayerPositionRequest(nextSequenceId()));
}

void MainWindow::autoConnectCountDown()
{
  if (--autoConnectCount_ > 0)
  {
    ui_->statusBar->showMessage(QString(tr("Attempting auto-connect in %1 seconds")).arg(autoConnectCount_));
  }
  else
  {
    // Stop auto connect if it is active
    stopAutoConnect();
    openConnection();
  }
}

void MainWindow::createStandardIcons()
{
  ui_->backButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
  ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
  ui_->forwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
  ui_->minVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
  ui_->maxVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
  ui_->networkButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
}

void MainWindow::openConnection()
{
  if (!client_.isOpen())
  {
    ui_->statusBar->showMessage(tr("Looking up host..."));
    client_.openConnection(serverInfo_);
  }
}

void MainWindow::setConnectedState()
{
  ui_->timeSlider->setEnabled(true);
  ui_->volumeSlider->setEnabled(true);
  ui_->minVolumeButton->setEnabled(true);
  ui_->maxVolumeButton->setEnabled(true);

  // Default player state is previous/next disabled and play button enabled
  ui_->backButton->setEnabled(false);
  ui_->forwardButton->setEnabled(false);
  ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
  ui_->playPauseToggleButton->setEnabled(true);
}

void MainWindow::setDisconnectedState(bool playButtonEnabled)
{
  currentTrack_.Clear();
  requests_.clear();
  setPlayerPosition(0);
  setSoundVolume(0);

  ui_->trackInfoDisplay->setCurrentIndex(BANNER_FORM);

  ui_->timeSlider->setEnabled(false);
  ui_->volumeSlider->setEnabled(false);
  ui_->minVolumeButton->setEnabled(false);
  ui_->maxVolumeButton->setEnabled(false);

  ui_->backButton->setEnabled(false);
  ui_->forwardButton->setEnabled(false);
  ui_->playPauseToggleButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
  ui_->playPauseToggleButton->setEnabled(playButtonEnabled);

  // Clear the track labels
  ui_->trackInfo->setArtist("");
  ui_->trackInfo->setSongTitle("");
}

unsigned long MainWindow::nextSequenceId()
{
  return ++sequenceId_;
}

void MainWindow::sendTrackedRequest(iTCH::EnvelopePtr envelope)
{
  assert(envelope->has_request());

  // Only send request if connected
  if (client_.isConnected())
  {
    // Only add reques to pending request queue if transmission succeeded
    if (client_.sendMessage(envelope))
    {
      requests_.insert(envelope->request().seqid(), envelope);
    }
  }
}

void MainWindow::startPositionTimer(unsigned int interval)
{
  // Start the timer to peridoically request time slider position
  if (!positionTimer_.isActive())
  {
    connect(&positionTimer_, SIGNAL(timeout()), this, SLOT(requestPlayerPosition()));
    positionTimer_.start(interval);
  }
}

void MainWindow::stopPositionTimer()
{
  if (positionTimer_.isActive())
  {
    positionTimer_.stop();
    positionTimer_.disconnect();
  }
}

void MainWindow::startAutoConnect()
{
  // Start if not active, reset if active
  if (!autoConnectTimer_.isActive())
  {
    connect(&autoConnectTimer_, SIGNAL(timeout()), this, SLOT(autoConnectCountDown()));
  }

  autoConnectCount_ = autoConnectInterval_;
  autoConnectTimer_.start(1000);
}

void MainWindow::stopAutoConnect()
{
  if (autoConnectTimer_.isActive())
  {
    // Clear the status message - if auto-connect was active, client is not connected
    ui_->statusBar->showMessage(tr("Unconnected"));

    autoConnectTimer_.stop();
    autoConnectTimer_.disconnect();
  }
}
