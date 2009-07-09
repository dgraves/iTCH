#include <QtNetwork/QHostInfo>
#include "PiTCHNetworkDialog.h"
#include "PiTCHWindow.h"
#include "ui_PiTCHWindow.h"

PiTCHWindow::PiTCHWindow(QWidget *parent) :
  QMainWindow(parent),
  ui_(new Ui::PiTCHWindow),
  serverInfo_(QHostInfo::localHostName(), 8049),
  buttonHeld_(false)
{
  ui_->setupUi(this);
  ui_->statusBar->showMessage(tr("Unconnected"));

  connect(&client_, SIGNAL(hostnameResolved()), this, SLOT(resolvedHostname()));
  connect(&client_, SIGNAL(connected()), this, SLOT(connectedToServer()));
  connect(&client_, SIGNAL(disconnected(bool, const QString &)), this, SLOT(disconnectedFromServer(bool, const QString &)));
  connect(&client_, SIGNAL(receivedMethod(const iTCHMethod &)), this, SLOT(receivedMethod(const iTCHMethod &)));
  connect(&client_, SIGNAL(error(const QString &)), this, SLOT(error(const QString &)));
}

PiTCHWindow::~PiTCHWindow()
{
  delete ui_;
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

  if (closedByServer)
  {
    ui_->statusBar->showMessage(QString("%1: %2").arg(status).arg(message));
  }
  else
  {
    ui_->statusBar->showMessage(QString("%1: %2").arg(status).arg(message));
  }
}

void PiTCHWindow::receivedMethod(const iTCHMethod &method)
{
}

void PiTCHWindow::error(const QString &message)
{
   ui_->statusBar->showMessage(message.toLocal8Bit().constData(), 5);
}

void PiTCHWindow::timeSliderValueChanged(int value)
{
  QStringList params;
  params.append(QString::number(value));
  client_.sendMethod(iTCHMethod(iTCHMethod::METHOD_IITUNES_PUT_PLAYERPOSITION, params, 0));
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
    client_.sendMethod(iTCHMethod(iTCHMethod::METHOD_IITUNES_BACKTRACK, QStringList(), 0));
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
    client_.sendMethod(iTCHMethod(iTCHMethod::METHOD_IITUNES_NEXTTRACK, QStringList(), 0));
  }
}

void PiTCHWindow::playPauseButtonToggled(bool)
{
  client_.sendMethod(iTCHMethod(iTCHMethod::METHOD_IITUNES_PLAYPAUSE, QStringList(), 0));
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
  QStringList params;
  params.append(QString::number(value));
  client_.sendMethod(iTCHMethod(iTCHMethod::METHOD_IITUNES_PUT_SOUNDVOLUME, params, 0));
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
