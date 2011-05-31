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
#ifndef ITCH_SERVER_H
#define ITCH_SERVER_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtNetwork/QTcpServer>
#include "iTCH/MessageBuilder.h"

namespace iTCH
{

class Connection;

class Server : public QObject
{
  Q_OBJECT

public:
  Server(QObject *parent = 0);

  virtual ~Server();

  QHostAddress getServerAddress() const;

  quint16 getServerPort() const;

  bool isListening();

  bool listen(const QHostAddress &address, quint16 port);

  void close();

  void closeConnection(Connection* connection);

  QString errorString() const;

signals:
  void connectionReceived(iTCH::Connection *connection);                                           // Report acceptance of a new connection
  void connectionLost(iTCH::Connection *connection, bool closedByHost, const QString &message);    // Report lost connection, either through communication error or closed by host
  void receivedMessage(iTCH::Connection *connection, const iTCH::EnvelopePtr envelope);            // Report receipt of a message from a connection
  void error(iTCH::Connection *connection, const QString &message);                                // Report an error with a received message

protected slots:
  // Slots to handle signals from iTCH::Connection
  void disconnectedFromPeer(bool closedByHost, const QString &message);
  void receiveMessage(const iTCH::EnvelopePtr envelope);
  void connectionError(const QString &message);

  // Slots to handle signals from QTcpServer
  void acceptConnection();

protected:
  QTcpServer server_;
};

} // end namespace iTCH

#endif // ITCH_SERVER_H
