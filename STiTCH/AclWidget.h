#ifndef ACLWIDGET_H
#define ACLWIDGET_H

#include <QListWidget>

class AclWidget : public QListWidget
{
  Q_OBJECT
public:
  explicit AclWidget(QWidget *parent = 0);

  bool containsAddress(const QString &address) const;

  QStringList getAddresses() const;

  QStringList getSelectedAddresses() const;

  void setAddresses(const QStringList &addresses);  

  void insertAddress(const QString &address);       // Add IP address to list, in sorted order, if not already in list

  void removeAddress(const QString &address);

  void removeSelectedAddresses();
};

#endif // ACLWIDGET_H
