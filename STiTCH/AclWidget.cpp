#include <QtCore/QStringList>
#include "AclWidget.h"

AclWidget::AclWidget(QWidget *parent) :
  QListWidget(parent)
{
  setSelectionMode(QListWidget::ExtendedSelection);
}

bool AclWidget::containsAddress(const QString &address) const
{
  for (int i = 0; i < count(); ++i)
  {
    if (item(i)->text() == address)
    {
      return true;
    }
  }

  return false;
}

QStringList AclWidget::getAddresses() const
{
  QStringList addresses;

  for (int i = 0; i < count(); ++i)
  {
    addresses.append(item(i)->text());
  }

  return addresses;
}

QStringList AclWidget::getSelectedAddresses() const
{
  QStringList addresses;

  QList<QListWidgetItem *> selected = selectedItems();
  for (QList<QListWidgetItem *>::iterator iter = selected.begin(); iter != selected.end(); ++iter)
  {
    addresses.append((*iter)->text());
  }

  return addresses;
}

void AclWidget::setAddresses(const QStringList &addresses)
{
  clear();
  addItems(addresses);
  sortItems();
}

void AclWidget::insertAddress(const QString &address)
{
  if (!containsAddress(address))
  {
    addItem(address);
    sortItems();
  }
}

void AclWidget::removeAddress(const QString &address)
{
  for (int i = 0; i < count(); ++i)
  {
    if (item(i)->text() == address)
    {
      delete takeItem(i);
      break;
    }
  }
}

void AclWidget::removeSelectedAddresses()
{
  QList<QListWidgetItem *> selected = selectedItems();
  for (QList<QListWidgetItem *>::iterator iter = selected.begin(); iter != selected.end(); ++iter)
  {
    delete takeItem(row(*iter));
  }
}
