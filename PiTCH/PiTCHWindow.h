#ifndef PITCHWINDOW_H
#define PITCHWINDOW_H

#include <QtGui/QMainWindow>

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
};

#endif // PITCHWINDOW_H
