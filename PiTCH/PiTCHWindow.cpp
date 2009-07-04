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

void PiTCHWindow::timeSliderValueChanged()
{
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
