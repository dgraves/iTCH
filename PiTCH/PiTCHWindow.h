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
#ifndef PITCHWINDOW_H
#define PITCHWINDOW_H

#include <QtCore/QTimer>
#include <QtCore/QMap>
#include <QtGui/QMainWindow>
#include "iTCH/MessageBuilder.h"
#include "iTCH/Client.h"
#include "iTCH/NetworkInfo.h"

namespace Ui
{
  class PiTCHWindow;
}

class PiTCHWindow : public QMainWindow
{
  Q_OBJECT

public:
  PiTCHWindow(QWidget *parent = 0);

  virtual ~PiTCHWindow();

protected:
  void changeEvent(QEvent *e);

protected slots:
  // Slots to handle signals from iTCHClient
  void resolvedHostname();
  void connectedToServer();
  void disconnectedFromServer(bool closedByHost, const QString &message);
  void processMessage(const iTCH::EnvelopePtr envelope);
  void processProtocolError(const QString &message);

  // Slots to handle signals from widgets
  void timeSliderPressed();
  void timeSliderReleased();
  void timeSliderValueChanged(int);
  void backButtonPressed();
  void backButtonReleased();
  void rewindTimeout();
  void forwardButtonPressed();
  void forwardButtonReleased();
  void fastForwardTimeout();
  void playPauseButtonClicked();
  void minVolumeButtonClicked();
  void maxVolumeButtonClicked();
  void volumeSliderValueChanged(int);
  void networkButtonToggled(bool);

  // Slots to handle status updates from server
  void setSoundVolume(int);
  void setMute(bool);
  void setPlayerPosition(int);
  void setPlayerState(bool playing);
  void setPlayerButtonsState(const iTCH::PlayerButtonsState &buttons);
  void setCurrentTrack(const iTCH::Track &);
  void requestPlayerPosition();

private:
  void createStandardIcons();
  void setDisconnectedState(bool playButtonEnabled);    // Disable controls when disconnected from player (could be connected to server with no player -> play/pause button enabled, or disconnected from server -> play/pause button disabled)
  void setConnectedState();                             // Enabled controls when connected
  unsigned long nextSequenceId();                       // Retrieve next valid sequence ID for request messages
  void sendTrackedRequest(iTCH::EnvelopePtr envelope);  // Store request in pending request queue and send
  void processNotification(iTCH::EnvelopePtr envelope);
  void processResponse(iTCH::EnvelopePtr envelope);
  void startPositionTimer(unsigned int interval);
  void stopPositionTimer();

private:
  // Map of client requests, keyed by sequence ID
  typedef QMap<unsigned int, iTCH::EnvelopePtr> PendingRequests;

private:
  Ui::PiTCHWindow  *ui_;
  iTCH::Client      client_;
  iTCH::NetworkInfo serverInfo_;
  unsigned int      autoConnectInterval_;
  bool              autoConnect_;
  bool              buttonHeld_;
  unsigned int      buttonHeldDelay_;
  QTimer            buttonTimer_;
  unsigned long     sequenceId_;            // Last used sequence ID
  PendingRequests   requests_;              // Requests sent to server that have not yet received a response
  unsigned int      positionInterval_;      // Interval between player time position requests
  unsigned int      positionIntervalRapid_; // Interval between player time position requests when fast-forward/rewind active
  QTimer            positionTimer_;         // Timer to request player time position
  bool              playing_;
  bool              playerDisconnected_;    // Indicates that the client is connected to the server, but the player has quit and has no COM connection to server
  iTCH::Track       currentTrack_;
};

#endif // PITCHWINDOW_H
