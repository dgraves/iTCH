#include <QtCore/QList>
#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
#include <QtGui/QCloseEvent>
#include <QtNetwork/QNetworkInterface>
#include "iTCHConnection.h"
#include "STiTCHDialog.h"
#include "ui_STiTCHDialog.h"

STiTCHDialog::STiTCHDialog(QWidget *parent) :
  QDialog(parent),
  ui_(new Ui::STiTCHDialog)
{
  ui_->setupUi(this);

  createTrayIcon();
  initializeConnectionList();
  fillInterfaceBox();
  setMaxLogEntries(ui_->maxLogEntriesSpinBox->value());  // Default number of log entries

  connect(&server_, SIGNAL(connectionReceived(iTCHConnection*)), this, SLOT(connectionReceived(iTCHConnection*)));
  connect(&server_, SIGNAL(connectionLost(iTCHConnection*,bool,QString)), this, SLOT(connectionLost(iTCHConnection*,bool,QString)));
  connect(&server_, SIGNAL(receivedMethod(iTCHConnection*,iTCHMethod)), this, SLOT(processMethod(iTCHConnection*,iTCHMethod)));
  connect(&server_, SIGNAL(error(iTCHConnection*,QString)), this, SLOT(communicationError(iTCHConnection*,QString)));

  connect(&controller_, SIGNAL(createdInstance()), this, SLOT(createdInstance()));
  connect(&controller_, SIGNAL(destroyedInstance()), this, SLOT(destroyedInstance()));
  connect(ui_->connectionsList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateDisconnectButton()));
  connect(ui_->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(trayIcon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

  // Create iTunes instance
  controller_.createInstance();

  // Start server listening on the default port
  setupServer();

  trayIcon_->show();
}

STiTCHDialog::~STiTCHDialog()
{
  QList<iTCHConnection *> keys = connectionIndexes_.keys();
  for (QList<iTCHConnection *>::iterator iter = keys.begin(); iter != keys.end(); ++iter)
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

  trayIcon_->hide();
  delete trayIconMenu_;
  delete trayIcon_;
  delete ui_;
}

void STiTCHDialog::closeEvent(QCloseEvent *e)
{
  hide();
  e->ignore();
}

void STiTCHDialog::changeEvent(QEvent *e)
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

void STiTCHDialog::addConnectionToList(iTCHConnection *connection)
{
  iTCHConnectionItem *item = new iTCHConnectionItem(connection);
  model_->appendRow(item);
  model_->setData(model_->index(item->row(), 0), connection->getConnectionAddress().toString());
  model_->setData(model_->index(item->row(), 1), connection->getConnectionTime());

  ui_->connectionsList->resizeColumnToContents(0);
  ui_->connectionsList->resizeColumnToContents(1);

  connectionIndexes_.insert(connection, item->index());

  // Update the icon
  setIcon();
}

void STiTCHDialog::removeConnectionFromList(iTCHConnection *connection)
{
  QMap<iTCHConnection *, QModelIndex>::iterator index = connectionIndexes_.find(connection);
  if (index != connectionIndexes_.end())
  {
    model_->removeRow((*index).row());
    connectionIndexes_.erase(index);
  }

  // Update the icon
  setIcon();
}

void STiTCHDialog::setupServer()
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

void STiTCHDialog::setIcon()
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

void STiTCHDialog::appendLogMessage(const QString &message)
{
  ui_->logTextEdit->append(QString("(%1) %2")
                           .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
                           .arg(message));
}

void STiTCHDialog::connectionReceived(iTCHConnection *connection)
{
  addConnectionToList(connection);

  // Add log message
  appendLogMessage(QString(tr("Connection received from %1")).arg(connection->getConnectionAddress().toString()));

  // Show task tray notification
  if (ui_->connectCheckBox->isChecked())
  {
    trayIcon_->showMessage(tr("Client Connected"), QString(tr("Connection received from %1")).arg(connection->getConnectionAddress().toString()), QSystemTrayIcon::Information, ui_->durationSpinBox->value() * 1000);
  }
}

void STiTCHDialog::connectionLost(iTCHConnection *connection, bool closedByPeer, const QString &message)
{
  removeConnectionFromList(connection);

  // Add log message
  appendLogMessage(QString(tr("Connection from %1 lost: %2")).arg(connection->getConnectionAddress().toString()).arg(message));

  // Show task tray notification
  if (ui_->disconnectCheckBox->isChecked())
  {
    trayIcon_->showMessage(tr("Client Disconnected"), QString(tr("Client from %1 has disconnected")).arg(connection->getConnectionAddress().toString()), QSystemTrayIcon::Information, ui_->durationSpinBox->value() * 1000);
  }
}

void STiTCHDialog::processMethod(iTCHConnection *connection, const iTCHMethod &method)
{
  // Attempt to recreate an inactive instance, if configuration dictates
  if (!controller_.hasInstance() && ui_->activationCheckBox->isChecked())
  {
    controller_.createInstance();
  }

  if (controller_.hasInstance())
  {
    controller_.callMethod(method);
  }
  else
  {
    // Send error response to client
  }
}

void STiTCHDialog::communicationError(iTCHConnection *connection, const QString &message)
{
  appendLogMessage(QString("%1 %2 -> %3")
                   .arg(tr("ERROR: Received invalid command from"))
                   .arg(connection->getConnectionAddress().toString())
                   .arg(message));
}

void STiTCHDialog::createdInstance()
{
  appendLogMessage(tr("Created connection to iTunes"));
  ui_->actionConnect->setEnabled(false);
  ui_->actionDisconnect->setEnabled(true);
  ui_->connectPlayerButton->setEnabled(false);
  ui_->disconnectPlayerButton->setEnabled(true);
  setIcon();
}

void STiTCHDialog::destroyedInstance()
{
  appendLogMessage(tr("Destroyed connection to iTunes"));
  ui_->actionConnect->setEnabled(true);
  ui_->actionDisconnect->setEnabled(false);
  ui_->connectPlayerButton->setEnabled(true);
  ui_->disconnectPlayerButton->setEnabled(false);
  setIcon();
}

void STiTCHDialog::connectController()
{
  controller_.createInstance();
}

void STiTCHDialog::disconnectController()
{
  controller_.destroyInstance();
}

void STiTCHDialog::serverSettingsChanged()
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

void STiTCHDialog::disconnectButtonClicked()
{
  // Remove all selected connections from the list and from the server
  QModelIndexList indexes = ui_->connectionsList->selectionModel()->selectedRows();
  for (QModelIndexList::iterator index = indexes.begin(); index != indexes.end(); ++index)
  {
    iTCHConnectionItem *item = dynamic_cast<iTCHConnectionItem *>(model_->item((*index).row()));
    server_.closeConnection(item->getConnection());
  }
}

void STiTCHDialog::updateDisconnectButton()
{
  if (ui_->connectionsList->selectionModel()->hasSelection())
  {
    ui_->disconnectButton->setEnabled(true);
  }
  else
  {
    ui_->disconnectButton->setEnabled(false);
  }
}

void STiTCHDialog::setMaxLogEntries(int maxEntries)
{
  ui_->logTextEdit->document()->setMaximumBlockCount(maxEntries);
}

void STiTCHDialog::accept()
{
  setupServer();
  close();
}

void STiTCHDialog::reject()
{
  close();
}

void STiTCHDialog::apply(QAbstractButton *button)
{
  // Determine if apply button was clicked and apply the server settings
  if (ui_->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
  {
    setupServer();
  }
}

void STiTCHDialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason)
  {
  case QSystemTrayIcon::DoubleClick:
    setVisible(true);
    break;
  default:
    break;
  }
}

void STiTCHDialog::createTrayIcon()
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

void STiTCHDialog::initializeConnectionList()
{
  // Setup the list columns
  model_ = new QStandardItemModel(0, 2, this);

  model_->setHeaderData(0, Qt::Horizontal, QObject::tr("Host"));
  model_->setHeaderData(1, Qt::Horizontal, QObject::tr("Connection Time"));

  ui_->connectionsList->setModel(model_);
}

void STiTCHDialog::fillInterfaceBox()
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
