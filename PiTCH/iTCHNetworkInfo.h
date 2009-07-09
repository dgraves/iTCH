#ifndef ITCHNETWORKINFO_H
#define ITCHNETWORKINFO_H

#include <QtCore/QObject>
#include <QtCore/QString>

class iTCHNetworkInfo : public QObject
{
public:
  iTCHNetworkInfo(const QString &hostname, unsigned short port, QObject *parent = 0) : QObject(parent), hostname_(hostname), port_(port) { }

  QString getHostname() const { return hostname_; }

  unsigned short getPort() const { return port_; }

  void setHostname(const QString &hostname) { hostname_ = hostname; }

  void setPort(unsigned short port) { port_ = port; }

protected:
  QString        hostname_;
  unsigned short port_;
};

#endif // ITCHNETWORKINFO_H
