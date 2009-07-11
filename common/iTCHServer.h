#ifndef ITCHSERVER_H
#define ITCHSERVER_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtNetwork/QTcpServer>
#include "iTCHMethod.h"

class iTCHServer : public QObject
{
public:
  iTCHServer();

signals:
  void acceptedConnection(QTcpSocket *, QDateTime);
  void receivedMethod(const iTCHMethod &method);

protected:
  QTcpServer server_;
};

#endif // ITCHSERVER_H
