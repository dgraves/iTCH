#ifndef ITCH_CLIENT_H
#define ITCH_CLIENT_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include "iTCH/Method.h"
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
  void disconnected(bool closedByPeer, const QString &message);
  void receivedMessage(const iTCH::EnvelopePtr envelope);
  void error(const QString &message);                             // Report errors with JSON-RC messages

protected slots:
  void resolvedHostname();
  void connectedToServer();
  void receiveMessage();
  void connectionClosedByServer();
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
