#include "iTCH/Client.h"

using namespace iTCH;

Client::Client(QObject *parent) :
  QObject(parent)
{
  // Connect signals and slots
  connect(&socket_, SIGNAL(hostFound()), this, SLOT(resolvedHostname()));
  connect(&socket_, SIGNAL(connected()), this, SLOT(connectedToServer()));
  connect(&socket_, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
  connect(&socket_, SIGNAL(readyRead()), this, SLOT(receiveMethod()));
  connect(&socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
}

Client::~Client()
{
}

bool Client::isOpen() const
{
  return socket_.isOpen();
}

bool Client::isConnected() const
{
  return (socket_.state() == QAbstractSocket::ConnectedState);
}

void Client::openConnection(const NetworkInfo &info)
{
  socket_.connectToHost(info.getHostname(), info.getPort());
}

void Client::closeConnection()
{
  socket_.close();
}

void Client::sendMethod(const Method &method)
{
  QString message = QString("%1\r\n").arg(method.toJsonRpc());
  if (socket_.write(message.toAscii().constData()) == -1)
  {
    socketError();
  }
}

void Client::resolvedHostname()
{
  hostnameResolved();
}

void Client::connectedToServer()
{
  connected();
}

void Client::receiveMethod()
{
  while (socket_.canReadLine())
  {
    QByteArray message = socket_.readLine();

    if (message.size() > 0)
    {
      try
      {
        receivedMethod(Method(QString(message)));
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

void Client::connectionClosedByServer()
{
  disconnected(true, socket_.errorString());
}

void Client::socketError()
{
  QString errorString = socket_.errorString();
  closeConnection();
  disconnected(false, errorString);
}
