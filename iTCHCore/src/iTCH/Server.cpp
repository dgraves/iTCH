#include "iTCH/Connection.h"
#include "iTCH/Server.h"

using namespace iTCH;

Server::Server(QObject *parent) :
  QObject(parent)
{
  // Slots to handle signals from QTcpServer
  connect(&server_, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

Server::~Server()
{
  if (isListening())
  {
    close();
  }
}

QHostAddress Server::getServerAddress() const
{
  return server_.serverAddress();
}

quint16 Server::getServerPort() const
{
  return server_.serverPort();
}

bool Server::isListening()
{
  return server_.isListening();
}

bool Server::listen(const QHostAddress &address, quint16 port)
{
  return server_.listen(address, port);
}

void Server::close()
{
  server_.close();
}

void Server::closeConnection(Connection *connection)
{
  delete connection;
}

QString Server::errorString() const
{
  return server_.errorString();
}

void Server::disconnectedFromPeer(bool closedByPeer, const QString &message)
{
  Connection *connection = dynamic_cast<Connection *>(sender());
  if (connection)
  {
    connectionLost(connection, closedByPeer, message);
    connection->deleteLater();  // Delete the connection after signal handling is complete
  }
}

void Server::receiveMethod(const Method &method)
{
  Connection *connection = dynamic_cast<Connection *>(sender());
  if (connection)
  {
    receivedMethod(connection, method);
  }
}

void Server::connectionError(const QString &message)
{
  Connection *connection = dynamic_cast<Connection *>(sender());
  if (connection)
  {
    error(connection, message);
  }
}

void Server::acceptConnection()
{
  Connection *connection = new Connection(server_.nextPendingConnection(), QDateTime::currentDateTime(), this);

  connect(connection, SIGNAL(disconnected(bool,QString)), this, SLOT(disconnectedFromPeer(bool,QString)));
  connect(connection, SIGNAL(receivedMethod(Method)), this, SLOT(receiveMethod(Method)));
  connect(connection, SIGNAL(error(QString)), this, SLOT(connectionError(QString)));

  // Signal receipt of new connection
  connectionReceived(connection);
}
