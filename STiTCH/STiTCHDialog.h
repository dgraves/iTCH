#ifndef STITCHDIALOG_H
#define STITCHDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QMap>
#include "iTCHServer.h"
#include "iTCHController.h"

class QAbstractButton;
class iTCHConnection;

namespace Ui {
  class STiTCHDialog;
}

class STiTCHDialog : public QDialog
{
  Q_OBJECT

protected:
  class iTCHConnectionItem : public QStandardItem
  {
  public:
    iTCHConnectionItem(iTCHConnection *connection) : QStandardItem(0, 2), connection_(connection) { }

    iTCHConnection * getConnection() const { return connection_; }

  protected:
    iTCHConnection *connection_;
  };

public:
  STiTCHDialog(QWidget *parent = 0);

  ~STiTCHDialog();

protected:
  void changeEvent(QEvent *e);

  void initializeConnectionList();

  void fillInterfaceBox();

  void addConnectionToList(iTCHConnection *connection);

  void removeConnectionFromList(iTCHConnection *connection);

  void setupServer();

protected slots:
  // Slots to handle signals from iTCHServer
  void connectionReceived(iTCHConnection *connection);
  void connectionLost(iTCHConnection *connection, bool closedByPeer, const QString &message);
  void processMethod(iTCHConnection *connection, const iTCHMethod &method);
  void connectionError(iTCHConnection *connection, const QString &message);

  // Slots to handle signals from widgets
  void serverSettingsChanged();
  void disconnectButtonClicked();
  void updateDisconnectButton();
  void accept();
  void rejet();
  void apply(QAbstractButton *button);

protected:
  iTCHServer                           server_;
  iTCHController                       controller_;
  QStandardItemModel                  *model_;
  QMap<iTCHConnection *, QModelIndex>  connectionIndexes_;
  bool                                 serverSettingsChanged_;

private:
  Ui::STiTCHDialog *ui_;
};

#endif // STITCHDIALOG_H
