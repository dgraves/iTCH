#ifndef STITCHDIALOG_H
#define STITCHDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>
#include <QtGui/QSystemTrayIcon>
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
  void closeEvent(QCloseEvent *e);

  void changeEvent(QEvent *e);

  void addConnectionToList(iTCHConnection *connection);

  void removeConnectionFromList(iTCHConnection *connection);

  void setupServer();

  void setIcon();

public slots:
  void appendLogMessage(const QString &message);

protected slots:
  // Slots to handle signals from iTCHServer
  void connectionReceived(iTCHConnection *connection);
  void connectionLost(iTCHConnection *connection, bool closedByPeer, const QString &message);
  void processMethod(iTCHConnection *connection, const iTCHMethod &method);
  void communicationError(iTCHConnection *connection, const QString &message);                    // Receive JSON error messages

  // Slots to handle signals from iTCHController
  void createdInstance();
  void destroyedInstance();

  // Slots to handle signals from widgets
  void connectController();
  void disconnectController();
  void serverSettingsChanged();
  void disconnectButtonClicked();
  void updateDisconnectButton();
  void setMaxLogEntries(int maxEntries);
  void accept();
  void reject();
  void apply(QAbstractButton *button);
  void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
  void createTrayIcon();
  void initializeConnectionList();
  void fillInterfaceBox();

private:
  Ui::STiTCHDialog                    *ui_;
  iTCHServer                           server_;
  iTCHController                       controller_;
  QSystemTrayIcon                     *trayIcon_;
  QMenu                               *trayIconMenu_;
  QStandardItemModel                  *model_;
  QMap<iTCHConnection *, QModelIndex>  connectionIndexes_;
};

#endif // STITCHDIALOG_H
