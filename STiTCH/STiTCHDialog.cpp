#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
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

  connect(&server_, SIGNAL(connectionReceived(iTCHConnection*)), this, SLOT(connectionReceived(iTCHConnection*)));
  connect(&server_, SIGNAL(connectionLost(iTCHConnection*,bool, QString)), this, SLOT(connectionLost(iTCHConnection*,bool,QString)));
  connect(&server_, SIGNAL(receivedMethod(iTCHConnection*,iTCHMethod)), this, SLOT(processMethod(iTCHConnection*,iTCHMethod)));
  connect(&controller_, SIGNAL(createdInstance()), this, SLOT(createdInstance()));
  connect(&controller_, SIGNAL(destroyedInstance()), this, SLOT(destroyedInstance()));
  connect(ui_->connectionsList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateDisconnectButton()));
  connect(ui_->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

  // Create iTunes instance
  controller_.createInstance();

  // Start server listening on the default port
  setupServer();

  trayIcon->show();
}

STiTCHDialog::~STiTCHDialog()
{
  QList<iTCHConnection *> keys = connectionIndexes_.keys();
  for (QList<iTCHConnection *>::iterator iter = keys.begin(); iter != keys.end(); ++iter)
  {
    server_.closeConnection(*iter);
  }

  server_.close();
  controller_.destroyInstance();
  delete ui_;
}

void STiTCHDialog::closeEvent(QCloseEvent *e)
{
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
}

void STiTCHDialog::removeConnectionFromList(iTCHConnection *connection)
{
  QMap<iTCHConnection *, QModelIndex>::iterator index = connectionIndexes_.find(connection);
  if (index != connectionIndexes_.end())
  {
    model_->removeRow((*index).row());
    connectionIndexes_.erase(index);
  }
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
    QMessageBox::critical(this, tr("Server Error"), QString(tr("The server could not be started: ")) + server_.errorString());

    // Ensure the apply button is enabled so that the user can try to connect to this address again
    ui_->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
  }
  else
  {
    // Apply button is disabled when listening and the server settings have not changed
    ui_->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
  }
}

void STiTCHDialog::connectionReceived(iTCHConnection *connection)
{
  addConnectionToList(connection);
}

void STiTCHDialog::connectionLost(iTCHConnection *connection, bool closedByPeer, const QString &message)
{
  removeConnectionFromList(connection);
}

void STiTCHDialog::processMethod(iTCHConnection *connection, const iTCHMethod &method)
{
  controller_.callMethod(method);
}

void STiTCHDialog::connectionError(iTCHConnection *connection, const QString &message)
{
}

void STiTCHDialog::createdInstance()
{
  ui_->actionConnect->setEnabled(false);
  ui_->actionDisconnect->setEnabled(true);
}

void STiTCHDialog::destroyedInstance()
{
  ui_->actionConnect->setEnabled(true);
  ui_->actionDisconnect->setEnabled(false);
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

  // If either the interface or the server port has changed, the server settings changed flag is asserted
  if ((port == server_.getServerPort()) && (iface == server_.getServerAddress()))
  {
    serverSettingsChanged_ = false;
  }
  else
  {
    serverSettingsChanged_ = true;
  }

  // Update the apply button when the server settings are changed (is only disabled when server is running and there is no change)
  if (server_.isListening() && !serverSettingsChanged_)
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
  // Determine if apply button was clicked and apply the server settings if they have changed
  if (ui_->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole && serverSettingsChanged_)
  {
    setupServer();
  }
}

void STiTCHDialog::createTrayIcon()
{
  trayIconMenu = new QMenu(this);
  trayIconMenu->addAction(ui_->actionSettings);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(ui_->actionConnect);
  trayIconMenu->addAction(ui_->actionDisconnect);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(ui_->actionQuit);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
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
  for (QList<QNetworkInterface>::iterator interface = interfaces.begin(); interface != interfaces.end(); ++interface)
  {
    if ((*interface).isValid() && ((*interface).flags() & QNetworkInterface::IsUp))
    {
      // Extract the IPv4 address for device
      QList<QNetworkAddressEntry> addresses = (*interface).addressEntries();
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
