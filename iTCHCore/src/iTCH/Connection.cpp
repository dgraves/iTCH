#include "iTCH/Connection.h"

using namespace iTCH;

Connection::Connection(QTcpSocket *socket, QDateTime time, QObject *parent) :
  QObject(parent),
  socket_(socket),
  time_(time)
{
  connect(socket_, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
  connect(socket_, SIGNAL(readyRead()), this, SLOT(receiveMethod()));
  connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
}

Connection::~Connection()
{
  delete socket_;
}

bool Connection::isOpen() const
{
  return socket_->isOpen();
}

bool Connection::isConnected() const
{
  return (socket_->state() == QAbstractSocket::ConnectedState);
}

QHostAddress Connection::getConnectionAddress() const
{
  return socket_->peerAddress();
}

quint16 Connection::getConnectionPort() const
{
  return socket_->peerPort();
}

QDateTime Connection::getConnectionTime() const
{
  return time_;
}

void Connection::sendMethod(const Method &method)
{
  QString message = QString("%1\r\n").arg(method.toJsonRpc());
  if (socket_->write(message.toAscii().constData()) == -1)
  {
    socketError();
  }
}

void Connection::receiveMethod()
{
  while (socket_->canReadLine())
  {
    QByteArray message = socket_->readLine();

    if (message.size() > 0)
    {
      try
      {
        // Trim CRLF from end of message before processing
        receivedMethod(Method(QString(message).trimmed()));
      }
      catch(Method::InvalidValueException ex)
      {
        // The method could not be decoded
        error(ex.what());
      }
    }
    else
    {
      socketError();
    }
  }
}

void Connection::connectionClosedByServer()
{
  disconnected(true, socket_->errorString());
}

void Connection::socketError()
{
  QString errorString = socket_->errorString();
  socket_->close();
  disconnected(false, errorString);
}
