#ifndef ITCH_CLIENT_H
#define ITCH_CLIENT_H

#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include "iTCH/Method.h"
#include "iTCH/NetworkInfo.h"

namespace iTCH
{

class Client : public QObject
{
  Q_OBJECT

public:
  Client(QObject *parent = 0);

  ~Client();

  bool isOpen() const;

  bool isConnected() const;

  void openConnection(const NetworkInfo &info);

  void closeConnection();

  void sendMethod(const Method &method);

signals:
  void hostnameResolved();
  void connected();
  void disconnected(bool closedByServer, const QString &message);
  void receivedMethod(const iTCH::Method &method);
  void error(const QString &message);                                       // Report errors with JSON-RC messages

protected slots:
  void resolvedHostname();
  void connectedToServer();
  void receiveMethod();
  void connectionClosedByServer();
  void socketError();

protected:
  QTcpSocket socket_;
};

} // end namespace iTCH

#endif // ITCH_CLIENT_H
