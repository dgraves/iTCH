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
#include <cassert>
#include <QtCore/QtEndian>
#include "iTCH/Client.h"

using namespace iTCH;

Client::Client(QObject *parent) :
  QObject(parent)
{
  socket_ = SocketPtr(new QTcpSocket);
  initializeSignals();
}

Client::Client(SocketPtr socket, QObject *parent) :
  QObject(parent),
  socket_(socket)
{
  assert(!socket.isNull());
  initializeSignals();
}

Client::~Client()
{
}

bool Client::isOpen() const
{
  return socket_->isOpen();
}

bool Client::isConnected() const
{
  return (socket_->state() == QAbstractSocket::ConnectedState);
}

QHostAddress Client::getPeerAddress() const
{
  return socket_->peerAddress();
}

quint16 Client::getPeerPort() const
{
  return socket_->peerPort();
}

void Client::openConnection(const NetworkInfo &info)
{
  socket_->connectToHost(info.getHostname(), info.getPort());
}

void Client::closeConnection()
{
  socket_->close();
}

void Client::sendMessage(const EnvelopePtr envelope)
{
  QByteArray data;
  if (packMessage(&data, envelope))
  {
    if (socket_->write(data) == -1)
    {
      socketError();
    }
  }
  else
  {
    error(QString("Failed to prepare network message for transmission: %1").arg(envelope->DebugString().c_str()));
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

void Client::receiveMessage()
{
  // Add new data to buffer, which may already contain a partial message
  buffer_.append(socket_->readAll());

  // Process the buffer if it at least contains a message size
  int size = buffer_.size();
  const char *data = buffer_.constData();
  int pos = 0;   // Current position in buffer
  while ((pos < size) && (size - pos > sizeof(qint32)))
  {
    // Get the message size from the current buffer position
    qint32 messageSize = qFromLittleEndian<qint32>(reinterpret_cast<const uchar *>(&data[pos]));
    pos += sizeof(qint32);

    // Only extract a complete message
    if (size - pos < messageSize)
    {
      // Make sure messageSize stays in the buffer when processed data is cleared
      pos -= sizeof(qint32);
      break;
    }
    else
    {
      // Get the message
      EnvelopePtr envelope = MessageBuilder::createEnvelope();

      if (unpackMessage(envelope, &data[pos], messageSize))
      {
        receivedMessage(envelope);
      }
      else
      {
        error("Failed to process network message");
      }

      pos += messageSize;
    }
  }

  // Remove the data that has been processed
  // Make sure the length is preserved (subtract from pos)
  buffer_.remove(0, pos);
}

void Client::connectionClosedByServer()
{
  disconnected(true, socket_->errorString());
}

void Client::socketError()
{
  QString errorString = socket_->errorString();
  socket_->close();
  disconnected(false, errorString);
}

void Client::initializeSignals()
{
  // Connect signals and slots
  connect(socket_.data(), SIGNAL(hostFound()), this, SLOT(resolvedHostname()));
  connect(socket_.data(), SIGNAL(connected()), this, SLOT(connectedToServer()));
  connect(socket_.data(), SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
  connect(socket_.data(), SIGNAL(readyRead()), this, SLOT(receiveMessage()));
  connect(socket_.data(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
}

bool Client::packMessage(QByteArray *buffer, const EnvelopePtr envelope)
{
  qint32 messageSize = envelope->ByteSize();

  buffer->resize(sizeof(messageSize) + messageSize);

  // Packed messages use little endian byte order for size
  qToLittleEndian(messageSize, reinterpret_cast<uchar *>(buffer->data()));
  return envelope->SerializeToArray(buffer->data() + sizeof(messageSize), buffer->size() - sizeof(messageSize));
}

bool Client::unpackMessage(EnvelopePtr envelope, const char *buffer, qint32 len)
{
  return envelope->ParseFromArray(buffer, len);
}
