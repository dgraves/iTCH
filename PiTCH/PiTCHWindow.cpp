#include "PiTCHWindow.h"
#include "ui_PiTCHWindow.h"

PiTCHWindow::PiTCHWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::PiTCHWindow)
{
  ui->setupUi(this);
}

PiTCHWindow::~PiTCHWindow()
{
    delete ui;
}

void PiTCHWindow::connectedToServer()
{
}

void PiTCHWindow::disconnectedFromServer(bool closedByServer)
{
}

void PiTCHWindow::receivedMethod(const iTCHMethod &method)
{
}

void PiTCHWindow::error(const QString &message)
{
}

void PiTCHWindow::timeSliderValueChanged()
{
  QStringList params;
  params.append(QString::number(ui->timeSlider->sliderPosition()));
  client_.sendMethod(iTCHMethod(iTCHMethod::METHOD_IITUNES_PUT_PLAYERPOSITION, params, 0));
}

void PiTCHWindow::backButtonPressed()
{
}

void PiTCHWindow::backButtonReleased()
{
}

void PiTCHWindow::forwardButtonPressed()
{
}
void PiTCHWindow::forwardButtonReleased()
{
}

void PiTCHWindow::playPauseButtonToggled(bool)
{
}

void PiTCHWindow::muteButtonClicked()
{
}

void PiTCHWindow::fullVolumeButtonClicked()
{
}

void PiTCHWindow::volumeSliderValueChanged()
{
}
