#ifndef ITCHCOMMAND_H
#define ITCHCOMMAND_H

class iTCHCommand
{
public:
  QString method_;
  QStringList params_;
  unsigned int id_;

public:
  iTCHCommand();

  iTCHCommand(const QString& method, const QStringList& params, unsigned int id);

  QString toJsonRpc() const;

  void fromJsonRpc(const QString& json);
};

#endif // ITCHCOMMAND_H
