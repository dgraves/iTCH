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
#ifndef ITCH_CLIENT_H
#define ITCH_CLIENT_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include "iTCH/MessageBuilder.h"
#include "iTCH/NetworkInfo.h"

namespace iTCH
{

class Client : public QObject
{
  Q_OBJECT

public:
  typedef QSharedPointer<QTcpSocket> SocketPtr;

public:
  Client(QObject *parent = 0);

  Client(SocketPtr socket, QObject *parent = 0);

  virtual ~Client();

  bool isOpen() const;

  bool isConnected() const;

  QHostAddress getPeerAddress() const;

  quint16 getPeerPort() const;

  void openConnection(const NetworkInfo &info);

  void closeConnection();

  void sendMessage(const EnvelopePtr envelope);

signals:
  void hostnameResolved();
  void connected();
  void disconnected(bool closedByHost, const QString &message);   // closedByHost is true if socket closed by closeConnection(), false for error condition (includes closed normally by remote peer)
  void receivedMessage(const iTCH::EnvelopePtr envelope);
  void error(const QString &message);                             // Report errors with JSON-RC messages

protected slots:
  void resolvedHostname();
  void connectedToServer();
  void receiveMessage();
  void connectionClosed();
  void socketError();

private:
  void initializeSignals();

  // Packs serialized Envelope into a QByteArray. The length
  // of the serialized data is inserted at the start of the
  // packed message.
  bool packMessage(QByteArray *buffer, const EnvelopePtr envelope);

  // Unpacks the QByteArray data into the Envelope.  The
  // QByteArray data contains the serialized Envelope data
  // only.  The length is not incuded.
  bool unpackMessage(EnvelopePtr envelope, const char *buffer, qint32 len);

protected:
  SocketPtr socket_;

private:
  QByteArray  buffer_;        // Buffer containing partially received messages
};

} // end namespace iTCH

#endif // ITCH_CLIENT_H
