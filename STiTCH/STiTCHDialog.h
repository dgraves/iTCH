#ifndef STITCHDIALOG_H
#define STITCHDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QStandardItemModel>
#include <QtGui/QSystemTrayIcon>
#include <QtCore/QModelIndex>
#include <QtCore/QMap>
#include "iTCH/Server.h"
#include "iTCH/Controller.h"

class QAbstractButton;

namespace iTCH {
  class Connection;
}

namespace Ui {
  class STiTCHDialog;
}

class STiTCHDialog : public QDialog
{
  Q_OBJECT

public:
  STiTCHDialog(QWidget *parent = 0);

  virtual ~STiTCHDialog();

protected:
  void closeEvent(QCloseEvent *e);

  void changeEvent(QEvent *e);

  void addConnectionToList(iTCH::Connection *connection);

  void removeConnectionFromList(iTCH::Connection *connection);

  void setupServer();

  void setIcon();

public slots:
  void appendLogMessage(const QString &message);

protected slots:
  // Slots to handle signals from iTCHServer
  void connectionReceived(iTCH::Connection *connection);
  void connectionLost(iTCH::Connection *connection, bool closedByPeer, const QString &message);
  void processMessage(iTCH::Connection *connection, const iTCH::EnvelopePtr envelope);
  void communicationError(iTCH::Connection *connection, const QString &message);                    // Receive JSON error messages

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
  Ui::STiTCHDialog                      *ui_;
  iTCH::Server                           server_;
  iTCH::Controller                       controller_;
  QSystemTrayIcon                       *trayIcon_;
  QMenu                                 *trayIconMenu_;
  QStandardItemModel                    *model_;
  QMap<iTCH::Connection *, QModelIndex>  connectionIndexes_;    // Map connections to items in connection list; quick lookup for list item removal when connection closes
};

#endif // STITCHDIALOG_H
