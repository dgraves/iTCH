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
