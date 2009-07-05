#ifndef ITCHCOMMAND_H
#define ITCHCOMMAND_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QRegExp>
#include <QtCore/qtconcurrentexception.h>

class iTCHMethod
{
public:
  class InvalidValueException : public QtConcurrent::Exception
  {
  public:
    InvalidValueException(const QString& message) : message_(message) { }
    ~InvalidValueException() throw() { }
    virtual const char* what() const throw() { return message_.toLocal8Bit().constData(); }
    void raise() const { throw *this; }
    Exception* clone() const { return new InvalidValueException(message_); }

  protected:
    QString message_;
  };

public:
  enum SupportedMethods
  {
    METHOD_IITUNES_BACKTRACK     = 1,
    METHOD_IITUNES_FASTFORWARD   = 2,
    METHOD_IITUNES_NEXTTRACK     = 3,
    METHOD_IITUNES_PAUSE         = 4,
    METHOD_IITUNES_PLAY          = 5,
    METHOD_IITUNES_PLAYPAUSE     = 6,
    METHOD_IITUNES_PREVIOUSTRACK = 7,
    METHOD_IITUNES_RESUME        = 8,
    METHOD_IITUNES_REWIND        = 9,
    METHOD_IITUNES_STOP          = 10
  };

public:
  typedef QMap<SupportedMethods, QString> SupportedMethodsMap;

public:
  iTCHMethod();

  iTCHMethod(const QString& method, const QStringList& params, unsigned int id);

  iTCHMethod(const QString& json);

  void setMethod(const QString& method);

  QString getMethod() const { return method_; }

  void setParams(const QStringList& params) { params_ = params; }

  QStringList getParams() const { return params_; }

  void setID(unsigned int id) { id_ = id; }

  unsigned int getID() const { return id_; }

  QString toJsonRpc() const;

  void fromJsonRpc(const QString& json);

protected:
  QString method_;
  QStringList params_;                       // Place strings inside doublequotes, to distinguish between strings and numbers (for JSON)
  unsigned int id_;
  SupportedMethodsMap supportedMethods_;
  QRegExp jsonMethodRegExp_;

protected:
  void createSupportedMethodsMap();
};

#endif // ITCHCOMMAND_H
