#ifndef ITCHCONNECTION_H
#define ITCHCONNECTION_H

#include <QtCore\QObject>
#include <QtCore\QDateTime>
#include <QtNetwork\QTcpSocket>
#include <QtNetwork\QHostAddress>
#include "iTCHMethod.h"

class iTCHConnection : public QObject
{
  Q_OBJECT

public:
  iTCHConnection(QTcpSocket *socket, QDateTime time, QObject *parent = 0);

  ~iTCHConnection();

  bool isOpen() const;

  bool isConnected() const;

  QHostAddress getConnectionAddress() const;

  quint16 getConnectionPort() const;

  QDateTime getConnectionTime() const;

  void sendMethod(const iTCHMethod &method);

signals:
  void disconnected(bool closedByPeer, const QString &message);
  void receivedMethod(const iTCHMethod &method);
  void error(const QString &message);                             // Report errors with JSON-RC messages

protected slots:
  void receiveMethod();
  void connectionClosedByServer();
  void socketError();

protected:
  QTcpSocket *socket_;

private:
  QDateTime   time_;
};

#endif // ITCHCONNECTION_H
