/********************************************************************
 * The iTCH project - Copyright (c) 2007-2011 Dustin Graves
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
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

void Server::disconnectedFromPeer(bool closedByHost, const QString &message)
{
  Connection *connection = dynamic_cast<Connection *>(sender());
  if (connection)
  {
    connectionLost(connection, closedByHost, message);
    connection->deleteLater();  // Delete the connection after signal handling is complete
  }
}

void Server::receiveMessage(const EnvelopePtr envelope)
{
  Connection *connection = dynamic_cast<Connection *>(sender());
  if (connection)
  {
    receivedMessage(connection, envelope);
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
  connect(connection, SIGNAL(receivedMessage(iTCH::EnvelopePtr)), this, SLOT(receiveMessage(iTCH::EnvelopePtr)));
  connect(connection, SIGNAL(error(QString)), this, SLOT(connectionError(QString)));

  // Signal receipt of new connection
  connectionReceived(connection);
}
