#include "iTCHClient.h"

iTCHClient::iTCHClient(QObject *parent) :
  QObject(parent)
{
  // Connect signals and slots
  connect(&socket_, SIGNAL(hostFound()), this, SLOT(resolvedHostname()));
  connect(&socket_, SIGNAL(connected()), this, SLOT(connectedToServer()));
  connect(&socket_, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
  connect(&socket_, SIGNAL(readyRead()), this, SLOT(receiveMethod()));
  connect(&socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
}

iTCHClient::~iTCHClient()
{
  if (isConnected())
  {
    closeConnection();
  }
}

bool iTCHClient::isConnected() const
{
  return (socket_.state() == QAbstractSocket::ConnectedState);
}

void iTCHClient::openConnection(const iTCHNetworkInfo &info)
{
  socket_.connectToHost(info.getHostname(), info.getPort());
}

void iTCHClient::closeConnection()
{
  socket_.close();
}

void iTCHClient::sendMethod(const iTCHMethod &method)
{
  QString message = QString("%1\r\n").arg(method.toJsonRpc());
  if (socket_.write(message.toAscii().constData()) == -1)
  {
    socketError();
  }
}

void iTCHClient::resolvedHostname()
{
  hostnameResolved();
}

void iTCHClient::connectedToServer()
{
  connected();
}

void iTCHClient::receiveMethod()
{
  if (socket_.canReadLine())
  {
    QByteArray message = socket_.readLine();

    if (message.size() > 0)
    {
      try
      {
        receivedMethod(iTCHMethod(QString(message)));
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

void iTCHClient::connectionClosedByServer()
{
  disconnected(true, socket_.errorString());
}

void iTCHClient::socketError()
{
  QString errorString = socket_.errorString();
  closeConnection();
  disconnected(false, errorString);
}
