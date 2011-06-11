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
#include "NetworkDialog.h"
#include "ui_NetworkDialog.h"

NetworkDialog::NetworkDialog(const iTCH::NetworkInfo &info, bool autoConnect, unsigned int autoConnectInterval, QWidget *parent) :
  QDialog(parent),
  ui_(new Ui::NetworkDialog)
{
  ui_->setupUi(this);
  setNetworkInfo(info);
  setAutoConnect(autoConnect);
  setAutoConnectInterval(autoConnectInterval);
}

NetworkDialog::~NetworkDialog()
{
  delete ui_;
}

void NetworkDialog::setNetworkInfo(const iTCH::NetworkInfo &info)
{
  ui_->hostnameLineEdit->setText(info.getHostname());
  ui_->portSpinBox->setValue(info.getPort());
}

void NetworkDialog::setAutoConnect(bool autoConnect)
{
  ui_->connectionBox->setChecked(autoConnect);
}

void NetworkDialog::setAutoConnectInterval(unsigned int interval)
{
  ui_->intervalSpinBox->setValue(interval);
}

iTCH::NetworkInfo NetworkDialog::getNetworkInfo() const
{
  return iTCH::NetworkInfo(ui_->hostnameLineEdit->text(), ui_->portSpinBox->value());
}

bool NetworkDialog::getAutoConnect() const
{
  return ui_->connectionBox->isChecked();
}

unsigned int NetworkDialog::getAutoConnectInterval() const
{
  return ui_->intervalSpinBox->value();
}

void NetworkDialog::changeEvent(QEvent *e)
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
