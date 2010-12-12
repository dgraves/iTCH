#ifndef ITCH_CONNECTION_H
#define ITCH_CONNECTION_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include "iTCH/Method.h"

namespace iTCH
{

class Connection : public QObject
{
  Q_OBJECT

public:
  Connection(QTcpSocket *socket, QDateTime time, QObject *parent = 0);

  ~Connection();

  bool isOpen() const;

  bool isConnected() const;

  QHostAddress getConnectionAddress() const;

  quint16 getConnectionPort() const;

  QDateTime getConnectionTime() const;

  void sendMethod(const Method &method);

signals:
  void disconnected(bool closedByPeer, const QString &message);
  void receivedMethod(const Method &method);
  void error(const QString &message);                             // Report errors with JSON-RC messages

protected slots:
  void receiveMethod();
  void connectionClosedByServer();
  void socketError();

protected:
  QTcpSocket *socket_;

private:
  QDateTime   time_;
};

Q_DECLARE_METATYPE(Connection *);

} // end namespace iTCH

#endif // ITCH_CONNECTION_H
