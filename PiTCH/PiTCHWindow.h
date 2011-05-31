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
  void disconnectedFromServer(bool closedByServer, const QString &message);
  void processMessage(const iTCH::EnvelopePtr envelope);
  void error(const QString &message);

  // Slots to handle signals from widgets
  void timeSliderValueChanged(int);
  void backButtonPressed();
  void backButtonReleased();
  void forwardButtonPressed();
  void forwardButtonReleased();
  void playPauseButtonClicked();
  void minVolumeButtonClicked();
  void maxVolumeButtonClicked();
  void volumeSliderValueChanged(int);
  void networkButtonClicked();

  // Slots to handle status updates from server
  void setSoundVolume(int);
  void setMute(bool);
  void setPlayerPosition(int);
  void setPlayerState(iTCH::PlayerState);
  void setCurrentTrack(const iTCH::Track &);

private:
  void createStandardIcons();
  unsigned long nextSequenceId();

private:
  Ui::PiTCHWindow  *ui_;
  iTCH::Client      client_;
  iTCH::NetworkInfo serverInfo_;
  unsigned int      autoConnectInterval_;
  bool              autoConnect_;
  bool              buttonHeld_;
  unsigned long     sequenceId_;
  iTCH::Track       currentTrack_;
};

#endif // PITCHWINDOW_H
