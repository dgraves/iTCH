#ifndef ITCH_CONNECTION_H
#define ITCH_CONNECTION_H

#include <QtCore/QDateTime>
#include "iTCH/Client.h"

namespace iTCH
{

class Connection : public Client
{
  Q_OBJECT

public:
  Connection(QTcpSocket *socket, QDateTime time, QObject *parent = 0);

  virtual ~Connection();

  QDateTime getConnectionTime() const;

  QString getAddress() const;

private:
  QDateTime   time_;
  QString     address_;
};

Q_DECLARE_METATYPE(Connection *);

} // end namespace iTCH

#endif // ITCH_CONNECTION_H
