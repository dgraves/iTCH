#ifndef STITCHDIALOG_H
#define STITCHDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QStandardItemModel>
#include <QtNetwork/QTcpServer>

namespace Ui {
  class STiTCHDialog;
}

class STiTCHDialog : public QDialog
{
  Q_OBJECT
public:
  STiTCHDialog(QWidget *parent = 0);

  ~STiTCHDialog();

protected:
  void changeEvent(QEvent *e);

  void initializeConnectionList();

  void fillInterfaceBox();

  void addConnectionToList();

protected slots:
  void serverSettingsChanged();
  void disconnectButtonClicked();

private:
  Ui::STiTCHDialog *ui_;
  QTcpServer servevr_;
  bool serverSettingsChanged_;
};

#endif // STITCHDIALOG_H
