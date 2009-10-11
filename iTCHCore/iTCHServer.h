#ifndef ITCHSERVER_H
#define ITCHSERVER_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtNetwork/QTcpServer>
#include "iTCHMethod.h"

class iTCHConnection;

class iTCHServer : public QObject
{
  Q_OBJECT

public:
  iTCHServer(QObject *parent = 0);

  ~iTCHServer();

  QHostAddress getServerAddress() const;

  quint16 getServerPort() const;

  bool isListening();

  bool listen(const QHostAddress &address, quint16 port);

  void close();

  void closeConnection(iTCHConnection* connection);

  QString errorString() const;

signals:
  void connectionReceived(iTCHConnection *connection);                                           // Report acceptance of a new connection
  void connectionLost(iTCHConnection *connection, bool closedByPeer, const QString &message);    // Report lost connection, either through communication error or closed by peer
  void receivedMethod(iTCHConnection *connection, const iTCHMethod &method);                     // Report receipt of a method from a connection
  void error(iTCHConnection *connection, const QString &message);                                // Report a JSON-RPC error from received message

protected slots:
  // Slots to handle signals from iTCHConnection
  void disconnectedFromPeer(bool closedByPeer, const QString &message);
  void receiveMethod(const iTCHMethod &method);
  void connectionError(const QString &message);

  // Slots to handle signals from QTcpServer
  void acceptConnection();

protected:
  QTcpServer server_;
};

#endif // ITCHSERVER_H
