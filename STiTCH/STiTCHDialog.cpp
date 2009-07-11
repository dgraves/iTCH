#include <QtCore/QDateTime>
#include <QtNetwork/QNetworkInterface>
#include "STiTCHDialog.h"
#include "ui_STiTCHDialog.h"

STiTCHDialog::STiTCHDialog(QWidget *parent) :
  QDialog(parent),
  ui_(new Ui::STiTCHDialog)
{
  ui_->setupUi(this);

  initializeConnectionList();

  fillInterfaceBox();
}

STiTCHDialog::~STiTCHDialog()
{
  delete ui_;
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

void STiTCHDialog::initializeConnectionList()
{
  // Setup the list columns
  QStandardItemModel *model = new QStandardItemModel(0, 3, this);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Host"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Port"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Connection Date"));

  ui_->connectionsList->setModel(model);
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

void STiTCHDialog::addConnectionToList()
{
  QAbstractItemModel * model = ui_->connectionsList->model();

  model->insertRow(model->rowCount());
  model->setData(model->index(0, 0), "dakinemobile");
  model->setData(model->index(0, 1), "Test");
  model->setData(model->index(0, 2), QDateTime::currentDateTime());

  ui_->connectionsList->resizeColumnToContents(0);
}

void STiTCHDialog::serverSettingsChanged()
{
}

void STiTCHDialog::disconnectButtonClicked()
{
}
