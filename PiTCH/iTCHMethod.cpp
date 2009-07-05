#include "iTCHMethod.h"

#define JSON_METHOD_REG_EXP "\\{\\s*\"method\":\\s*\"(.*)\"\\s*,\\s*\"params\":\\s*\\[(.*)\\]\\s*,\\s*\"id\":\\s*(.*)\\}"

iTCHMethod::iTCHMethod()
: id_(0),
  jsonMethodRegExp_(JSON_METHOD_REG_EXP, Qt::CaseSensitive, QRegExp::RegExp2)
{
  createSupportedMethodsMap();
}

iTCHMethod::iTCHMethod(const QString& method, const QStringList& params, unsigned int id)
: method_(method),
  params_(params),
  id_(id),
  jsonMethodRegExp_(JSON_METHOD_REG_EXP, Qt::CaseSensitive, QRegExp::RegExp2)
{
  createSupportedMethodsMap();
}

iTCHMethod::iTCHMethod(const QString& json)
: id_(0),
  jsonMethodRegExp_(JSON_METHOD_REG_EXP, Qt::CaseSensitive, QRegExp::RegExp2)
{
  createSupportedMethodsMap();
  fromJsonRpc(json);
}

void iTCHMethod::createSupportedMethodsMap()
{
  supportedMethods_[METHOD_IITUNES_BACKTRACK] = "IiTunes::BackTrack";
  supportedMethods_[METHOD_IITUNES_FASTFORWARD] = "IiTunes::FastForward";
  supportedMethods_[METHOD_IITUNES_NEXTTRACK] = "IiTunes::NextTrack";
  supportedMethods_[METHOD_IITUNES_PAUSE] = "IiTunes::Pause";
  supportedMethods_[METHOD_IITUNES_PLAY] = "IiTunes::Play";
  supportedMethods_[METHOD_IITUNES_PLAYPAUSE] = "IiTunes::PlayPause";
  supportedMethods_[METHOD_IITUNES_PREVIOUSTRACK] = "IiTunes::PreviousTrack";
  supportedMethods_[METHOD_IITUNES_RESUME] = "IiTunes::Resume";
  supportedMethods_[METHOD_IITUNES_REWIND] = "IiTunes::Rewind";
  supportedMethods_[METHOD_IITUNES_STOP] = "IiTunes::Stop";
}

void iTCHMethod::setMethod(const QString& method)
{
  // Check for supported method name
  if (!supportedMethods_.values().contains(method))
  {
    throw InvalidValueException(QString("Unsupported method: %1").arg(method));
  }

  method_ = method;
}

QString iTCHMethod::toJsonRpc() const
{
  // Initialize JSON-RPC string with method name
  QString json = QString("{ \"method\": %1, \"params\": [").arg(method_);

  // Add paramters to JSON-RPC params list
  if (!params_.empty())
  {
    QStringList::const_iterator paramsIterator = params_.constBegin();

    // Add first parameter to list
    json += (*paramsIterator);

    // Add remaining parameters with comma separation
    for (; paramsIterator != params_.constEnd(); ++paramsIterator)
    {
      json += QString(", %1").arg(*paramsIterator);
    }
  }

  // Append the id number
  json += QString("], \"id\": %1}").arg((id_ > 0) ? QString::number(id_) : "nil");

  return json;
}

void iTCHMethod::fromJsonRpc(const QString& json)
{
  // Extract method, parameters, and id
  int pos = jsonMethodRegExp_.indexIn(json);

  // Check for valid JSON RPC message
  if (pos < 0)
  {
    throw InvalidValueException(QString("Invalid JSON-RPC message: not correctly formatted"));
  }

  int n = jsonMethodRegExp_.numCaptures();
  if (n != 0)
  {
    throw InvalidValueException(QString("Invalid JSON-RPC message: incorrect number of parameters (had %1, needs 3)").arg(n));
  }

  // Extract the method name (checks for supported method)
  setMethod(jsonMethodRegExp_.cap(1));

  // Extract the
  QString params = jsonMethodRegExp_.cap(2);
  params_ = params.split(" ,", QString::SkipEmptyParts);

  QString id = jsonMethodRegExp_.cap(3);
  bool valid;
  id_ = (id == "nil") ? 0 : id.toInt(&valid);
}
