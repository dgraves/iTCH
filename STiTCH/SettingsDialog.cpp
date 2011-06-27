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
#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
#include <QtGui/QStandardItem>
#include <QtNetwork/QNetworkInterface>
#include "iTCH/Connection.h"
#include "iTCH/MessageBuilder.h"
#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

enum AclMode
{
  BlackList = 0,
  WhiteList = 1
};

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent),
  ui_(new Ui::SettingsDialog)
{
  ui_->setupUi(this);

  createTrayIcon();
  initializeConnectionList();
  fillInterfaceBox();

  loadSettings();
  setMaxLogEntries(ui_->maxLogEntriesSpinBox->value());  // Default number of log entries

  connect(&server_, SIGNAL(connectionReceived(iTCH::Connection*)), this, SLOT(connectionReceived(iTCH::Connection*)));
  connect(&server_, SIGNAL(connectionLost(iTCH::Connection*,bool,QString)), this, SLOT(connectionLost(iTCH::Connection*,bool,QString)));
  connect(&server_, SIGNAL(receivedMessage(iTCH::Connection*,iTCH::EnvelopePtr)), this, SLOT(processMessage(iTCH::Connection*,iTCH::EnvelopePtr)));
  connect(&server_, SIGNAL(protocolError(iTCH::Connection*,QString)), this, SLOT(processProtocolError(iTCH::Connection*,QString)));

  connect(&controller_, SIGNAL(createdInstance()), this, SLOT(createdInstance()));
  connect(&controller_, SIGNAL(destroyedInstance()), this, SLOT(destroyedInstance()));
  connect(&controller_, SIGNAL(statusChanged(iTCH::EnvelopePtr)), this, SLOT(sendMessage(iTCH::EnvelopePtr)));
  connect(&controller_, SIGNAL(generatedResponse(iTCH::Connection*,iTCH::EnvelopePtr)), this, SLOT(sendMessage(iTCH::Connection*,iTCH::EnvelopePtr)));
  connect(ui_->connectionsList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateBlockDisconnectButtons()));
  connect(ui_->blacklistPage->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateAclRemoveButton()));
  connect(ui_->whitelistPage->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateAclRemoveButton()));
  connect(ui_->aclModeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAclRemoveButton()));
  connect(ui_->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(trayIcon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

  // Create iTunes instance
  controller_.createInstance();

  // Start server listening on the default port
  setupServer();

  trayIcon_->show();
}

SettingsDialog::~SettingsDialog()
{
  QList<iTCH::Connection *> keys = connectionIndexes_.keys();
  for (QList<iTCH::Connection *>::iterator iter = keys.begin(); iter != keys.end(); ++iter)
  {
    server_.closeConnection(*iter);
  }

  if (server_.isListening())
  {
    server_.close();
  }

  if (controller_.hasInstance())
  {
    controller_.destroyInstance();
  }

  saveSettings();

  trayIcon_->hide();
  delete trayIconMenu_;
  delete trayIcon_;
  delete ui_;
}

void SettingsDialog::closeEvent(QCloseEvent *e)
{
  hide();
  e->ignore();
}

void SettingsDialog::changeEvent(QEvent *e)
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

void SettingsDialog::addConnectionToList(iTCH::Connection *connection)
{
  QStandardItem *item = new QStandardItem(0, 2);
  item->setData(QVariant::fromValue(connection));
  model_->appendRow(item);
  model_->setData(model_->index(item->row(), 0), connection->getAddress());
  model_->setData(model_->index(item->row(), 1), connection->getConnectionTime());

  ui_->connectionsList->resizeColumnToContents(0);
  ui_->connectionsList->resizeColumnToContents(1);

  connectionIndexes_.insert(connection, item->index());

  // Update the icon
  setIcon();
}

void SettingsDialog::removeConnectionFromList(iTCH::Connection *connection)
{
  QMap<iTCH::Connection *, QModelIndex>::iterator index = connectionIndexes_.find(connection);
  if (index != connectionIndexes_.end())
  {
    model_->removeRow((*index).row());
    connectionIndexes_.erase(index);
  }

  // Update the icon
  setIcon();
}

void SettingsDialog::setupServer()
{
  QString ifstring = ui_->interfaceComboBox->currentText();
  QHostAddress iface = (ifstring == "Any") ? QHostAddress::Any : QHostAddress(ifstring);
  quint16 port = ui_->portSpinBox->value();

  if (server_.isListening())
  {
    server_.close();
  }

  if (!server_.listen(iface, port))
  {
    QMessageBox::critical(this, tr("Server Error"), QString(tr("The server could not be started: %1")).arg(server_.errorString()));

    appendLogMessage(QString(tr("Failed to start server listening on port %1 and interface %2: %3")).arg(port).arg(ifstring).arg(server_.errorString()));

    // Ensure the apply button is enabled so that the user can try to connect to this address again
    ui_->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
  }
  else
  {
    appendLogMessage(QString(tr("Server listening on port %1 and interface %2")).arg(port).arg(ifstring));

    // Apply button is disabled when listening and the server settings have not changed
    ui_->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
  }

  // Update the icon
  setIcon();
}

void SettingsDialog::setIcon()
{
  // Default to the server error icon
  QIcon icon = QIcon(":/tray/icons/noserver.svg");
  QString message = tr("Server failed to open port and can not receive connections");

  if (server_.isListening())
  {
    if (controller_.hasInstance() && !connectionIndexes_.empty())
    {
      // Have client connections and iTunes is active
      icon = QIcon(":/tray/icons/active_connections.svg");
      message = tr("iTunes link is active and clients are connected");
    }
    else if (controller_.hasInstance())
    {
      // Have no client connections, but iTunes is active
      icon = QIcon(":/tray/icons/active_noconnections.svg");
      message = tr("iTunes link is active but no clients are connected");
    }
    else if (!connectionIndexes_.empty())
    {
      icon = QIcon(":/tray/icons/inactive_connections.svg");
      message = tr("Clients are connected but iTunes link is inactive");
    }
    else
    {
      icon = QIcon(":/tray/icons/inactive_noconnections.svg");
      message = tr("No clients are connected and iTunes link is inactive");
    }
  }    

  trayIcon_->setIcon(icon);
  trayIcon_->setToolTip(message);
  setWindowIcon(icon);
}

void SettingsDialog::appendLogMessage(const QString &message)
{
  ui_->logTextEdit->append(QString("(%1) %2")
                           .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
                           .arg(message));
}

void SettingsDialog::connectionReceived(iTCH::Connection *connection)
{
  addConnectionToList(connection);

  // Add log message
  appendLogMessage(QString(tr("Connection received from %1")).arg(connection->getAddress()));

  // Show task tray notification
  if (ui_->connectCheckBox->isChecked())
  {
    trayIcon_->showMessage(tr("Client Connected"), QString(tr("Connection received from %1")).arg(connection->getAddress()), QSystemTrayIcon::Information, ui_->durationSpinBox->value() * 1000);
  }
}

void SettingsDialog::connectionLost(iTCH::Connection *connection, bool closedByHost, const QString &message)
{
  removeConnectionFromList(connection);

  // Add log message
  if (closedByHost)
  {
    appendLogMessage(QString(tr("Connection from %1 closed by server")).arg(connection->getAddress()).arg(message));
  }
  else
  {
    appendLogMessage(QString(tr("Connection from %1 lost: %2")).arg(connection->getAddress()).arg(message));
  }

  // Show task tray notification
  if (ui_->disconnectCheckBox->isChecked())
  {
    trayIcon_->showMessage(tr("Client Disconnected"), QString(tr("Client from %1 has disconnected")).arg(connection->getAddress()), QSystemTrayIcon::Information, ui_->durationSpinBox->value() * 1000);
  }
}

void SettingsDialog::processMessage(iTCH::Connection *connection, const iTCH::EnvelopePtr envelope)
{
  // Attempt to recreate an inactive instance, if configuration dictates
  if (!controller_.hasInstance() && ui_->activationCheckBox->isChecked())
  {
    controller_.createInstance();
  }

  if (controller_.hasInstance() && iTCH::MessageBuilder::containsValidClientRequest(envelope))
  {
    if (ui_->logAllCheckBox->isChecked())
    {
      appendLogMessage(QString("%1 %2 -> %3")
        .arg(tr("Message from client"))
        .arg(connection->getAddress())
        .arg(envelope->DebugString().c_str()));
    }

    controller_.processRequest(envelope->request(), connection);
  }
  else
  {
    if (ui_->logAllCheckBox->isChecked())
    {
      appendLogMessage(QString("%1 %2 -> %3")
        .arg(tr("Invalid message from client"))
        .arg(connection->getAddress())
        .arg(envelope->DebugString().c_str()));
    }

    // Send error response to client if an invalid request was received
    if (envelope->has_request())
    {
      connection->sendMessage(iTCH::MessageBuilder::makeFailedResponse(envelope->request().seqid(), "Request is invalid"));
    }
  }
}

void SettingsDialog::processProtocolError(iTCH::Connection *connection, const QString &message)
{
  appendLogMessage(QString("%1 %2 -> %3")
    .arg(tr("Error communicating with client"))
    .arg(connection->getAddress())
    .arg(message));
}

void SettingsDialog::createdInstance()
{
  appendLogMessage(tr("Created connection to iTunes"));
  ui_->actionConnect->setEnabled(false);
  ui_->actionDisconnect->setEnabled(true);
  ui_->connectPlayerButton->setEnabled(false);
  ui_->disconnectPlayerButton->setEnabled(true);
  setIcon();
}

void SettingsDialog::destroyedInstance()
{
  appendLogMessage(tr("Destroyed connection to iTunes"));
  ui_->actionConnect->setEnabled(true);
  ui_->actionDisconnect->setEnabled(false);
  ui_->connectPlayerButton->setEnabled(true);
  ui_->disconnectPlayerButton->setEnabled(false);
  setIcon();
}

void SettingsDialog::sendMessage(iTCH::EnvelopePtr envelope)
{
  if (ui_->logAllCheckBox->isChecked())
  {
    appendLogMessage(QString("%1 -> %3")
      .arg(tr("Message to all clients"))
      .arg(envelope->DebugString().c_str()));
  }

  QList<iTCH::Connection *> connections = connectionIndexes_.keys();
  for (QList<iTCH::Connection *>::iterator iter = connections.begin(); iter != connections.end(); ++iter)
  {
    (*iter)->sendMessage(envelope);
  }
}

void SettingsDialog::sendMessage(iTCH::Connection *connection, iTCH::EnvelopePtr envelope)
{
  if (ui_->logAllCheckBox->isChecked())
  {
    appendLogMessage(QString("%1 %2 -> %3")
      .arg(tr("Message to client"))
      .arg(connection->getAddress())
      .arg(envelope->DebugString().c_str()));
  }

  connection->sendMessage(envelope);
}

void SettingsDialog::processComError(const QString &message)
{
  appendLogMessage(QString("%1 %2")
    .arg(tr("Error communicating with iTunes:"))
    .arg(message));
}

void SettingsDialog::connectController()
{
  controller_.createInstance();
}

void SettingsDialog::disconnectController()
{
  controller_.destroyInstance();
}

void SettingsDialog::serverSettingsChanged()
{
  QString ifstring = ui_->interfaceComboBox->currentText();
  QHostAddress iface = (ifstring == "Any") ? QHostAddress::Any : QHostAddress(ifstring);
  quint16 port = ui_->portSpinBox->value();
  bool serverSettingsChanged = true;

  // If the user has just reset the UI values to the current values, the server settings changed flag is unasserted
  if ((port == server_.getServerPort()) && (iface == server_.getServerAddress()))
  {
    serverSettingsChanged = false;
  }

  // Update the apply button when the server settings are changed (is only disabled when server is running and there is no change)
  if (server_.isListening() && !serverSettingsChanged)
  {
    ui_->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
  }
  else
  {
    ui_->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
  }
}

void SettingsDialog::blockButtonClicked()
{
  // Update access control lists with selected connections and disconnect all selected connections
  QModelIndexList indexes = ui_->connectionsList->selectionModel()->selectedRows();
  for (QModelIndexList::iterator index = indexes.begin(); index != indexes.end(); ++index)
  {
    QStandardItem *item = model_->item((*index).row());
    iTCH::Connection *connection = item->data().value<iTCH::Connection *>();

    QString address = connection->getAddress();

    // Add to blacklist
    if (!blacklist_.contains(address))
    {
      blacklist_.insert(address);
      ui_->blacklistPage->insertAddress(address);
    }

    // Remove from whitelist
    if (whitelist_.contains(address))
    {
      whitelist_.remove(address);
      ui_->whitelistPage->removeAddress(address);
    }

    server_.closeConnection(connection);
  }
}

void SettingsDialog::disconnectButtonClicked()
{
  // Remove all selected connections from the list and from the server
  QModelIndexList indexes = ui_->connectionsList->selectionModel()->selectedRows();
  for (QModelIndexList::iterator index = indexes.begin(); index != indexes.end(); ++index)
  {
    QStandardItem *item = model_->item((*index).row());
    iTCH::Connection *connection = item->data().value<iTCH::Connection *>();
    server_.closeConnection(connection);
  }
}

void SettingsDialog::updateBlockDisconnectButtons()
{
  if (ui_->connectionsList->selectionModel()->hasSelection())
  {
    ui_->blockButton->setEnabled(true);
    ui_->disconnectButton->setEnabled(true);
  }
  else
  {
    ui_->blockButton->setEnabled(false);
    ui_->disconnectButton->setEnabled(false);
  }
}

// Add and remove funtions for performing the same operation on different lists
namespace
{
  inline void addToAclList(QWidget *parent, QSet<QString> *list, AclWidget *widget)
  {
    QString address;

    // Get the IP address
    for (;;)
    {
      address = QInputDialog::getText(parent, "IP Address Entry", "Enter IP Address to be added:");
      QHostAddress validator(address);

      if (!validator.isNull())
      {
        break;
      }

      QMessageBox::critical(parent, "Invalid Address", "The specified IP address is not valid.");
    }

    if (!address.isEmpty())
    {
      list->insert(address);
      widget->insertAddress(address);
    }
  }

  inline void removeFromAclList(QSet<QString> *list, AclWidget *widget)
  {
    QStringList addresses = widget->getSelectedAddresses();
    for (QStringList::iterator iter = addresses.begin(); iter != addresses.end(); ++iter)
    {
      list->remove(*iter);
    }

    widget->removeSelectedAddresses();
  }
}

void SettingsDialog::aclAddButtonClicked()
{
  // Determine the ACL type
  if (ui_->aclModeBox->currentIndex() == BlackList)
  {
    // Add to blacklist
    addToAclList(this, &blacklist_, ui_->blacklistPage);
  }
  else
  {
    // Add to whitelist
    addToAclList(this, &whitelist_, ui_->whitelistPage);
  }
}

void SettingsDialog::aclRemoveButtonClicked()
{
  // Determine the ACL type
  if (ui_->aclModeBox->currentIndex() == BlackList)
  {
    // Remove from blacklist
    removeFromAclList(&blacklist_, ui_->blacklistPage);
  }
  else
  {
    // Remove to whitelist
    removeFromAclList(&whitelist_, ui_->whitelistPage);
  }
}

void SettingsDialog::updateAclRemoveButton()
{
  AclWidget *list = NULL;

  // Determine the ACL type
  if (ui_->aclModeBox->currentIndex() == BlackList)
  {
    list = ui_->blacklistPage;
  }
  else
  {
    list = ui_->whitelistPage;
  }

  if (list->selectionModel()->hasSelection())
  {
    ui_->aclRemoveButton->setEnabled(true);
  }
  else
  {
    ui_->aclRemoveButton->setEnabled(false);
  }
}

void SettingsDialog::setMaxLogEntries(int maxEntries)
{
  ui_->logTextEdit->document()->setMaximumBlockCount(maxEntries);
}

void SettingsDialog::accept()
{
  setupServer();
  close();
}

void SettingsDialog::reject()
{
  close();
}

void SettingsDialog::apply(QAbstractButton *button)
{
  // Determine if apply button was clicked and apply the server settings
  if (ui_->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
  {
    setupServer();
  }
}

void SettingsDialog::showWindow()
{
  if (isVisible())
  {
    activateWindow();
    raise();
  }
  else
  {
    show();
  }
}

void SettingsDialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason)
  {
  case QSystemTrayIcon::DoubleClick:
    showWindow();
    break;
  default:
    break;
  }
}

void SettingsDialog::createTrayIcon()
{
  trayIconMenu_ = new QMenu(this);
  trayIconMenu_->addAction(ui_->actionSettings);
  trayIconMenu_->addSeparator();
  trayIconMenu_->addAction(ui_->actionConnect);
  trayIconMenu_->addAction(ui_->actionDisconnect);
  trayIconMenu_->addSeparator();
  trayIconMenu_->addAction(ui_->actionQuit);

  trayIcon_ = new QSystemTrayIcon(this);
  trayIcon_->setContextMenu(trayIconMenu_);
}

void SettingsDialog::initializeConnectionList()
{
  // Setup the list columns
  model_ = new QStandardItemModel(0, 2, this);

  model_->setHeaderData(0, Qt::Horizontal, QObject::tr("Host"));
  model_->setHeaderData(1, Qt::Horizontal, QObject::tr("Connection Time"));

  ui_->connectionsList->setModel(model_);
}

void SettingsDialog::fillInterfaceBox()
{
  // Clear any existing interfaces from the box (for regenerating list when a device becomes enabled/disabled)
  ui_->interfaceComboBox->clear();

  // Fill the interface combobox with all enabled devices
  ui_->interfaceComboBox->addItem("Any");

  QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
  for (QList<QNetworkInterface>::iterator iface = interfaces.begin(); iface != interfaces.end(); ++iface)
  {
    if ((*iface).isValid() && ((*iface).flags() & QNetworkInterface::IsUp))
    {
      // Extract the IPv4 address for device
      QList<QNetworkAddressEntry> addresses = (*iface).addressEntries();
      for (QList<QNetworkAddressEntry>::iterator address = addresses.begin(); address != addresses.end(); ++address)
      {
        if ((*address).ip().toIPv4Address() != 0)
        {
          ui_->interfaceComboBox->addItem((*address).ip().toString());
        }
      }
    }
  }
}


void SettingsDialog::saveSettings()
{
  QSettings settings;

  settings.beginGroup("settingsdialog");
  settings.setValue("size", size());
  settings.setValue("pos", pos());
  settings.endGroup();

  settings.beginGroup("network");
  settings.setValue("port", ui_->portSpinBox->value());
  settings.setValue("interface", ui_->interfaceComboBox->currentText());
  settings.endGroup();

  settings.beginGroup("activity");
  settings.setValue("notifyOnConnect", ui_->connectCheckBox->isChecked());
  settings.setValue("notifyOnDisconnect", ui_->disconnectCheckBox->isChecked());
  settings.setValue("notifyDuration", ui_->durationSpinBox->value());
  settings.setValue("reconnectOnCommand", ui_->activationCheckBox->isChecked());
  settings.endGroup();

  settings.beginGroup("log");
  settings.setValue("maxEntries", ui_->maxLogEntriesSpinBox->value());
  settings.setValue("logAllCommunication", ui_->logAllCheckBox->isChecked());
  settings.endGroup();
}

void SettingsDialog::loadSettings()
{
  QSettings settings;

  settings.beginGroup("settingsdialog");
  resize(settings.value("size", size()).toSize());
  move(settings.value("pos", QPoint(50, 50)).toPoint());
  settings.endGroup();

  settings.beginGroup("network");
  ui_->portSpinBox->setValue(settings.value("port", ui_->portSpinBox->value()).toInt());
  QString interface = settings.value("interface", ui_->interfaceComboBox->currentText()).toString();
  int index = ui_->interfaceComboBox->findText(interface);
  ui_->interfaceComboBox->setCurrentIndex(index == -1 ? 0 : index);
  settings.endGroup();

  settings.beginGroup("activity");
  ui_->connectCheckBox->setChecked(settings.value("notifyOnConnect", ui_->connectCheckBox->isChecked()).toBool());
  ui_->disconnectCheckBox->setChecked(settings.value("notifyOnDisconnect", ui_->disconnectCheckBox->isChecked()).toBool());
  ui_->durationSpinBox->setValue(settings.value("notifyDuration", ui_->durationSpinBox->value()).toInt());
  ui_->activationCheckBox->setChecked(settings.value("reconnectOnCommand", ui_->activationCheckBox->isChecked()).toBool());
  settings.endGroup();

  settings.beginGroup("log");
  ui_->maxLogEntriesSpinBox->setValue(settings.value("maxEntries", ui_->maxLogEntriesSpinBox->value()).toInt());
  ui_->logAllCheckBox->setChecked(settings.value("logAllCommunication", ui_->logAllCheckBox->isChecked()).toBool());
  settings.endGroup();
}
