#ifndef ITCH_SERVER_H
#define ITCH_SERVER_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtNetwork/QTcpServer>
#include "iTCH/MessageBuilder.h"

namespace iTCH
{

class Connection;

class Server : public QObject
{
  Q_OBJECT

public:
  Server(QObject *parent = 0);

  virtual ~Server();

  QHostAddress getServerAddress() const;

  quint16 getServerPort() const;

  bool isListening();

  bool listen(const QHostAddress &address, quint16 port);

  void close();

  void closeConnection(Connection* connection);

  QString errorString() const;

signals:
  void connectionReceived(iTCH::Connection *connection);                                           // Report acceptance of a new connection
  void connectionLost(iTCH::Connection *connection, bool closedByPeer, const QString &message);    // Report lost connection, either through communication error or closed by peer
  void receivedMessage(iTCH::Connection *connection, const iTCH::EnvelopePtr envelope);                    // Report receipt of a message from a connection
  void error(iTCH::Connection *connection, const QString &message);                                // Report an error with a received message

protected slots:
  // Slots to handle signals from iTCH::Connection
  void disconnectedFromPeer(bool closedByPeer, const QString &message);
  void receiveMessage(const iTCH::EnvelopePtr envelope);
  void connectionError(const QString &message);

  // Slots to handle signals from QTcpServer
  void acceptConnection();

protected:
  QTcpServer server_;
};

} // end namespace iTCH

#endif // ITCH_SERVER_H
