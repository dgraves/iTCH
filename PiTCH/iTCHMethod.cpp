#include "itchcommand.h"

iTCHCommand::iTCHCommand()
: id_(0)
{
}

iTCHCommand::iTCHCommand(const QString& method, const QStringList& params, unsigned int id)
: method_(method),
  params_(params),
  id_(id)
{
}

QString iTCHCommand::toJsonRpc() const
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
  json += QString("], \"id\": %1};").arg((id > 0) ? QString::number(id_) : "nil");
}

void fromJsonRpc(const QString& json)
{
}
