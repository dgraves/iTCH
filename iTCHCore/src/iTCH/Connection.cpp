#include <cassert>
#include "iTCH/Connection.h"

using namespace iTCH;

Connection::Connection(QTcpSocket *socket, QDateTime time, QObject *parent) :
  Client(Client::SocketPtr(socket), parent),
  time_(time),
  address_(socket->peerAddress().toString())
{
  assert(socket != NULL);
}

Connection::~Connection()
{
}

QDateTime Connection::getConnectionTime() const
{
  return time_;
}

QString Connection::getAddress() const
{
  return address_;
}
