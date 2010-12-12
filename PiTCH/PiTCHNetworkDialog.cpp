#include "PiTCHNetworkDialog.h"
#include "ui_PiTCHNetworkDialog.h"

PiTCHNetworkDialog::PiTCHNetworkDialog(const iTCH::NetworkInfo &info, bool autoConnect, unsigned int autoConnectInterval, QWidget *parent) :
  QDialog(parent),
  ui_(new Ui::PiTCHNetworkDialog)
{
  ui_->setupUi(this);
  setNetworkInfo(info);
  setAutoConnect(autoConnect);
  setAutoConnectInterval(autoConnectInterval);
}

PiTCHNetworkDialog::~PiTCHNetworkDialog()
{
  delete ui_;
}

void PiTCHNetworkDialog::setNetworkInfo(const iTCH::NetworkInfo &info)
{
  ui_->hostnameLineEdit->setText(info.getHostname());
  ui_->portSpinBox->setValue(info.getPort());
}

void PiTCHNetworkDialog::setAutoConnect(bool autoConnect)
{
  ui_->networkBox->setChecked(autoConnect);
}

void PiTCHNetworkDialog::setAutoConnectInterval(unsigned int interval)
{
  ui_->intervalSpinBox->setValue(interval);
}

iTCH::NetworkInfo PiTCHNetworkDialog::getNetworkInfo() const
{
  return iTCH::NetworkInfo(ui_->hostnameLineEdit->text(), ui_->portSpinBox->value());
}

bool PiTCHNetworkDialog::getAutoConnect() const
{
  return ui_->networkBox->isChecked();
}

unsigned int PiTCHNetworkDialog::getAutoConnectInterval() const
{
  return ui_->intervalSpinBox->value();
}

void PiTCHNetworkDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type())
  {
  case QEvent::LanguageChange:
    ui_->retranslateUi(this);
    break;
  default:
    break;
  }
}
