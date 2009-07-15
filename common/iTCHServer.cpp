#include "iTCHConnection.h"
#include "iTCHServer.h"

iTCHServer::iTCHServer(QObject *parent) :
  QObject(parent)
{
  // Slots to handle signals from QTcpServer
  connect(&server_, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

iTCHServer::~iTCHServer()
{
  if (isListening())
  {
    close();
  }
}

QHostAddress iTCHServer::getServerAddress() const
{
  return server_.serverAddress();
}

quint16 iTCHServer::getServerPort() const
{
  return server_.serverPort();
}

bool iTCHServer::isListening()
{
  return server_.isListening();
}

bool iTCHServer::listen(const QHostAddress &address, quint16 port)
{
  return server_.listen(address, port);
}

void iTCHServer::close()
{
  server_.close();
}

void iTCHServer::closeConnection(iTCHConnection *connection)
{
  delete connection;
}

QString iTCHServer::errorString() const
{
  return server_.errorString();
}

void iTCHServer::disconnectedFromPeer(bool closedByPeer, const QString &message)
{
  iTCHConnection *connection = dynamic_cast<iTCHConnection *>(sender());
  if (connection)
  {
    connectionLost(connection, closedByPeer, message);
    delete connection;
  }
}

void iTCHServer::receiveMethod(const iTCHMethod &method)
{
  iTCHConnection *connection = dynamic_cast<iTCHConnection *>(sender());
  if (connection)
  {
    receivedMethod(connection, method);
  }
}

void iTCHServer::connectionError(const QString &message)
{
  iTCHConnection *connection = dynamic_cast<iTCHConnection *>(sender());
  if (connection)
  {
    error(connection, message);
  }
}

void iTCHServer::acceptConnection()
{
  iTCHConnection *connection = new iTCHConnection(server_.nextPendingConnection(), QDateTime::currentDateTime(), this);

  connect(connection, SIGNAL(disconnected(bool,QString)), this, SLOT(disconnectedFromPeer(bool,QString)));
  connect(connection, SIGNAL(receivedMethod(iTCHMethod)), this, SLOT(receiveMethod(iTCHMethod)));
  connect(connection, SIGNAL(error(QString)), this, SLOT(connectionError(QString)));

  // Signal receipt of new connection
  connectionReceived(connection);
}
