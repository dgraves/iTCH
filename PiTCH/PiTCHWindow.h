#ifndef PITCHWINDOW_H
#define PITCHWINDOW_H

#include <QtGui/QMainWindow>
#include "iTCH/Method.h"
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

  ~PiTCHWindow();

protected:
  void changeEvent(QEvent *e);

protected slots:
  // Slots to handle signals from iTCHClient
  void resolvedHostname();
  void connectedToServer();
  void disconnectedFromServer(bool closedByServer, const QString &message);
  void processMethod(const iTCH::Method &method);
  void error(const QString &message);

  // Slots to handle signals from widgets
  void timeSliderValueChanged(int);
  void backButtonPressed();
  void backButtonReleased();
  void forwardButtonPressed();
  void forwardButtonReleased();
  void playPauseButtonClicked();
  void muteButtonClicked();
  void fullVolumeButtonClicked();
  void volumeSliderValueChanged(int);
  void networkButtonClicked();

protected:
  iTCH::Client      client_;
  iTCH::NetworkInfo serverInfo_;
  unsigned int      autoConnectInterval_;
  bool              autoConnect_;
  bool              buttonHeld_;

private:
  Ui::PiTCHWindow  *ui_;
};

#endif // PITCHWINDOW_H
