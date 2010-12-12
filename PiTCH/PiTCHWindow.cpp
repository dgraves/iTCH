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
  connect(&client_, SIGNAL(disconnected(bool,QString)), this, SLOT(disconnectedFromServer(bool,QString)));
  connect(&client_, SIGNAL(receivedMethod(iTCH::Method)), this, SLOT(processMethod(iTCH::Method)));
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

void PiTCHWindow::processMethod(const iTCH::Method &method)
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
  client_.sendMethod(iTCH::Method(iTCH::Method::METHOD_IITUNES_PUT_PLAYERPOSITION, params, 0));
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
    client_.sendMethod(iTCH::Method(iTCH::Method::METHOD_IITUNES_BACKTRACK, QStringList(), 0));
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
    client_.sendMethod(iTCH::Method(iTCH::Method::METHOD_IITUNES_NEXTTRACK, QStringList(), 0));
  }
}

void PiTCHWindow::playPauseButtonClicked()
{
  client_.sendMethod(iTCH::Method(iTCH::Method::METHOD_IITUNES_PLAYPAUSE, QStringList(), 0));
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
  client_.sendMethod(iTCH::Method(iTCH::Method::METHOD_IITUNES_PUT_SOUNDVOLUME, params, 0));
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
