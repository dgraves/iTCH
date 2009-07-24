#include "iTCHConnection.h"

iTCHConnection::iTCHConnection(QTcpSocket *socket, QDateTime time, QObject *parent) :
  QObject(parent),
  socket_(socket),
  time_(time)
{
  connect(socket_, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
  connect(socket_, SIGNAL(readyRead()), this, SLOT(receiveMethod()));
  connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
}

iTCHConnection::~iTCHConnection()
{
  delete socket_;
}

bool iTCHConnection::isOpen() const
{
  return socket_->isOpen();
}

bool iTCHConnection::isConnected() const
{
  return (socket_->state() == QAbstractSocket::ConnectedState);
}

QHostAddress iTCHConnection::getConnectionAddress() const
{
  return socket_->peerAddress();
}

quint16 iTCHConnection::getConnectionPort() const
{
  return socket_->peerPort();
}

QDateTime iTCHConnection::getConnectionTime() const
{
  return time_;
}

void iTCHConnection::sendMethod(const iTCHMethod &method)
{
  QString message = QString("%1\r\n").arg(method.toJsonRpc());
  if (socket_->write(message.toAscii().constData()) == -1)
  {
    socketError();
  }
}

void iTCHConnection::receiveMethod()
{
  while (socket_->canReadLine())
  {
    QByteArray message = socket_->readLine();

    if (message.size() > 0)
    {
      try
      {
        // Trim CRLF from end of message before processing
        receivedMethod(iTCHMethod(QString(message).trimmed()));
      }
      catch(iTCHMethod::InvalidValueException ex)
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

void iTCHConnection::connectionClosedByServer()
{
  disconnected(true, socket_->errorString());
}

void iTCHConnection::socketError()
{
  QString errorString = socket_->errorString();
  socket_->close();
  disconnected(false, errorString);
}
