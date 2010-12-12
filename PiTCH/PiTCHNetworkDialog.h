#ifndef PITCHNETWORKDIALOG_H
#define PITCHNETWORKDIALOG_H

#include <QtGui/QDialog>
#include "iTCH/NetworkInfo.h"

namespace Ui {
  class PiTCHNetworkDialog;
}

class PiTCHNetworkDialog : public QDialog {
  Q_OBJECT
public:
  PiTCHNetworkDialog(const iTCH::NetworkInfo &info, bool autoConnect, unsigned int autoConnectInterval = 10, QWidget *parent = 0);

  ~PiTCHNetworkDialog();

  void setNetworkInfo(const iTCH::NetworkInfo &info);

  void setAutoConnect(bool autoConnect);

  void setAutoConnectInterval(unsigned int interval);

  iTCH::NetworkInfo getNetworkInfo() const;

  bool getAutoConnect() const;

  unsigned int getAutoConnectInterval() const;

protected:
  void changeEvent(QEvent *e);

private:
  Ui::PiTCHNetworkDialog *ui_;
};

#endif // PITCHNETWORKDIALOG_H
