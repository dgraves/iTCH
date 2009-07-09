#ifndef ITCHCLIENT_H
#define ITCHCLIENT_H

#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include "iTCHMethod.h"
#include "iTCHNetworkInfo.h"

class iTCHClient : public QObject
{
  Q_OBJECT

public:
  iTCHClient(QObject *parent = 0);

  ~iTCHClient();

  bool isConnected() const;

  void openConnection(const iTCHNetworkInfo &info);

  void closeConnection();

  void sendMethod(const iTCHMethod &method);

signals:
  void hostnameResolved();
  void connected();
  void disconnected(bool closedByServer, const QString &message);
  void receivedMethod(const iTCHMethod &method);
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

#endif // ITCHCLIENT_H
