#ifndef ITCH_NETWORKINFO_H
#define ITCH_NETWORKINFO_H

#include <QtCore/QString>

namespace iTCH
{

class NetworkInfo
{
public:
  NetworkInfo(const QString &hostname, unsigned short port) : hostname_(hostname), port_(port) { }

  QString getHostname() const { return hostname_; }

  unsigned short getPort() const { return port_; }

  void setHostname(const QString &hostname) { hostname_ = hostname; }

  void setPort(unsigned short port) { port_ = port; }

protected:
  QString        hostname_;
  unsigned short port_;
};

} // end namespace iTCH

#endif // ITCH_NETWORKINFO_H
