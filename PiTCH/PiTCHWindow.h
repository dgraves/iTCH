#ifndef PITCHWINDOW_H
#define PITCHWINDOW_H

#include <QtGui/QMainWindow>
#include "iTCHMethod.h"
#include "iTCHClient.h"

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

protected slots:
  // Slots to handle signals from iTCHClient
  void connectedToServer();
  void disconnectedFromServer(bool closedByServer);
  void receivedMethod(const iTCHMethod &method);
  void error(const QString &message);

  // Slots to handle signals from widgets
  void timeSliderValueChanged();
  void backButtonPressed();
  void backButtonReleased();
  void forwardButtonPressed();
  void forwardButtonReleased();
  void playPauseButtonToggled(bool);
  void muteButtonClicked();
  void fullVolumeButtonClicked();
  void volumeSliderValueChanged();

protected:
  Ui::PiTCHWindow *ui;
  iTCHClient client_;
};

#endif // PITCHWINDOW_H
