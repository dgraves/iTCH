#ifndef ITCHCLIENT_H
#define ITCHCLIENT_H

#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include "iTCHMethod.h"

class iTCHClient : public QObject
{
  Q_OBJECT

public:
  iTCHClient(QObject *parent = 0);

  ~iTCHClient();

  bool isConnected() const;

  void openConnection(const QString &hostname, unsigned short port);

  void closeConnection();

  void sendMethod(const iTCHMethod &method);

signals:
  void connected();
  void disconnected(bool closedByServer);
  void receivedMethod(const iTCHMethod &method);
  void error(const QString &message);

protected slots:
  void connectedToServer();
  void receiveMethod();
  void connectionClosedByServer();
  void socketError();

protected:
  QTcpSocket socket_;
};

#endif // ITCHCLIENT_H
