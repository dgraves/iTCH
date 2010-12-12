#include <QtCore/QMap>
#include <QtCore/QRegExp>
#include "iTCH/Method.h"

using namespace iTCH;

namespace {
  // Initialize static values for JSON RPC conversion and checking
  const char JSON_METHOD_REG_EXP[] = "\\{\\s*\"method\":\\s*\"(.*)\"\\s*,\\s*\"params\":\\s*\\[(.*)\\]\\s*,\\s*\"id\":\\s*(.*)\\s*\\}";
  const QRegExp __jsonMethodRegExp(JSON_METHOD_REG_EXP, Qt::CaseSensitive, QRegExp::RegExp2);

  class __SupportedMethodsMap : public QMap<Method::SupportedMethods, QString>
  {
  public:
    __SupportedMethodsMap()
    {
      (*this)[Method::METHOD_IITUNES_BACKTRACK] = "IiTunes::BackTrack";
      (*this)[Method::METHOD_IITUNES_FASTFORWARD] = "IiTunes::FastForward";
      (*this)[Method::METHOD_IITUNES_NEXTTRACK] = "IiTunes::NextTrack";
      (*this)[Method::METHOD_IITUNES_PAUSE] = "IiTunes::Pause";
      (*this)[Method::METHOD_IITUNES_PLAY] = "IiTunes::Play";
      (*this)[Method::METHOD_IITUNES_PLAYPAUSE] = "IiTunes::PlayPause";
      (*this)[Method::METHOD_IITUNES_PREVIOUSTRACK] = "IiTunes::PreviousTrack";
      (*this)[Method::METHOD_IITUNES_RESUME] = "IiTunes::Resume";
      (*this)[Method::METHOD_IITUNES_REWIND] = "IiTunes::Rewind";
      (*this)[Method::METHOD_IITUNES_STOP] = "IiTunes::Stop";
      (*this)[Method::METHOD_IITUNES_GET_SOUNDVOLUME] = "IiTunes::get_SoundVolume";
      (*this)[Method::METHOD_IITUNES_PUT_SOUNDVOLUME] = "IiTunes::put_SoundVolume";
      (*this)[Method::METHOD_IITUNES_GET_MUTE] = "IiTunes::get_Mute";
      (*this)[Method::METHOD_IITUNES_PUT_MUTE] = "IiTunes::put_Mute";
      (*this)[Method::METHOD_IITUNES_GET_PLAYERPOSITION] = "IiTunes::get_playerPosition";
      (*this)[Method::METHOD_IITUNES_PUT_PLAYERPOSITION] = "IiTunes::put_PlayerPosition";
      (*this)[Method::METHOD_IITCHSERVER_GET_PLAYERSTATE] = "iTCHServer::getPlayserState";
      (*this)[Method::METHOD_IITCHSERVER_GET_CURRENTTRACK] = "iTCHServer::getCurrentTrack";
      (*this)[Method::METHOD_IITCHSERVER_GET_CURRENTPLAYLIST] = "iTCHServer::getCurrentPlaylist";
      (*this)[Method::METHOD_IITCHCLIENT_VOLUMECHANGED] = "iTCHClient::volumeChanged";
      (*this)[Method::METHOD_IITCHCLIENT_PLAYINGSTARTED] = "iTCHClient::playingStarted";
      (*this)[Method::METHOD_IITCHCLIENT_PLAYINGSTOPPED] = "iTCHClient::playingStopped";
      (*this)[Method::METHOD_IITCHCLIENT_TRACKINFOCHANGED] = "iTCHClient::trackInfoChanged";
    }
  } __supportedMethods;
}

Method::Method(const Method& method) :
  method_(method.method_),
  params_(method.params_),
  id_(method.id_)
{
}

Method::Method(SupportedMethods method, const QStringList &params, unsigned int id) :
  params_(params),
  id_(id)
{
  setMethod(method);
}

Method::Method(const QString &json) :
  id_(0)
{
  fromJsonRpc(json);
}

void Method::setMethod(SupportedMethods method)
{
  // Check for supported method name
  if (!__supportedMethods.contains(method))
  {
    throw InvalidValueException(QString("Unsupported method: %1").arg(method));
  }

  method_ = method;
}

QString Method::toJsonRpc() const
{
  // Initialize JSON-RPC string with method name
  if (!__supportedMethods.contains(method_))
  {
    throw InvalidValueException(QString("Unsupported method: %1").arg(method_));
  }

  QString json = QString("{ \"method\": \"%1\", \"params\": [").arg(__supportedMethods[method_]);

  // Add paramters to JSON-RPC params list
  if (!params_.empty())
  {
    QStringList::const_iterator paramsIterator = params_.constBegin();

    // Add first parameter to list
    json += (*paramsIterator++);

    // Add remaining parameters with comma separation
    for (; paramsIterator != params_.constEnd(); ++paramsIterator)
    {
      json += QString(", %1").arg(*paramsIterator);
    }
  }

  // Append the id number
  json += QString("], \"id\": %1}").arg((id_ > 0) ? QString::number(id_) : "null");

  return json;
}

void Method::fromJsonRpc(const QString &json)
{
  // Extract method, parameters, and id
  int pos = __jsonMethodRegExp.indexIn(json);

  // Check for valid JSON RPC message
  if (pos < 0)
  {
    throw InvalidValueException(QString("Invalid JSON-RPC message: not correctly formatted"));
  }

  int n = __jsonMethodRegExp.numCaptures();
  if (n != 3)
  {
    throw InvalidValueException(QString("Invalid JSON-RPC message: incorrect number of parameters (had %1, needs 3)").arg(n));
  }

  // Extract the method name (checks for supported method)
  QString method = __jsonMethodRegExp.cap(1);
  if (!__supportedMethods.values().contains(method))
  {
    throw InvalidValueException(QString("Invalid JSON-RPC message: unsupported method %1)").arg(method));
  }

  // Get key for method
  method_ = __supportedMethods.key(method);

  // Extract the
  QString params = __jsonMethodRegExp.cap(2);
  params_ = params.split(QRegExp("\\s*,\\s*"), QString::SkipEmptyParts);

  QString id = __jsonMethodRegExp.cap(3);
  bool valid;
  id_ = (id == "null") ? 0 : id.toInt(&valid);
}
