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
#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QtGui/QDialog>
#include "iTCH/NetworkInfo.h"

namespace Ui {
  class NetworkDialog;
}

class NetworkDialog : public QDialog {
  Q_OBJECT
public:
  NetworkDialog(const iTCH::NetworkInfo &info, bool autoConnect, unsigned int autoConnectInterval = 10, QWidget *parent = 0);

  virtual ~NetworkDialog();

  void setNetworkInfo(const iTCH::NetworkInfo &info);

  void setAutoConnect(bool autoConnect);

  void setAutoConnectInterval(unsigned int interval);

  iTCH::NetworkInfo getNetworkInfo() const;

  bool getAutoConnect() const;

  unsigned int getAutoConnectInterval() const;

protected:
  void changeEvent(QEvent *e);

private:
  Ui::NetworkDialog *ui_;
};

#endif // PITCHNETWORKDIALOG_H
