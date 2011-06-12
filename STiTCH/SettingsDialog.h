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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

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
  class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  SettingsDialog(QWidget *parent = 0);

  virtual ~SettingsDialog();

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
  // Slots to handle signals from iTCH::Server
  void connectionReceived(iTCH::Connection *connection);
  void connectionLost(iTCH::Connection *connection, bool closedByHost, const QString &message);
  void processMessage(iTCH::Connection *connection, const iTCH::EnvelopePtr envelope);
  void processProtocolError(iTCH::Connection *connection, const QString &message);              // Receive a message formatting error

  // Slots to handle signals from iTCH::Controller
  void createdInstance();
  void destroyedInstance();
  void sendMessage(const iTCH::EnvelopePtr envelope);                                           // Send message to all connections
  void sendMessage(iTCH::Connection *connection, const iTCH::EnvelopePtr envelope);             // Send message to all connections
  void processComError(const QString &message);                                                 // Receive a COM related error

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
  void showWindow();
  void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
  void createTrayIcon();
  void initializeConnectionList();
  void fillInterfaceBox();
  void saveSettings();
  void loadSettings();

private:
  Ui::SettingsDialog                    *ui_;
  iTCH::Server                           server_;
  iTCH::Controller                       controller_;
  QSystemTrayIcon                       *trayIcon_;
  QMenu                                 *trayIconMenu_;
  QStandardItemModel                    *model_;
  QMap<iTCH::Connection *, QModelIndex>  connectionIndexes_;    // Map connections to items in connection list; quick lookup for list item removal when connection closes
};

#endif // STITCHDIALOG_H
