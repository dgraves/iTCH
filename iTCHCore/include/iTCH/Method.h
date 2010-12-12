#ifndef ITCH_METHOD_H
#define ITCH_METHOD_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/qtconcurrentexception.h>

namespace iTCH
{

class Method
{
public:
  class InvalidValueException : public QtConcurrent::Exception
  {
  public:
    InvalidValueException(const QString &message) : message_(message) { }
    ~InvalidValueException() throw() { }
    virtual const char * what() const throw() { return message_.toLocal8Bit().constData(); }
    void raise() const { throw *this; }
    Exception * clone() const { return new InvalidValueException(message_); }

  protected:
    QString message_;
  };

public:
  enum SupportedMethods
  {
    METHOD_IITUNES_BACKTRACK               = 1,     // No value is returned
    METHOD_IITUNES_FASTFORWARD             = 2,     // No value is returned
    METHOD_IITUNES_NEXTTRACK               = 3,     // No value is returned
    METHOD_IITUNES_PAUSE                   = 4,     // No value is returned
    METHOD_IITUNES_PLAY                    = 5,     // No value is returned
    METHOD_IITUNES_PLAYPAUSE               = 6,     // No value is returned
    METHOD_IITUNES_PREVIOUSTRACK           = 7,     // No value is returned
    METHOD_IITUNES_RESUME                  = 8,     // No value is returned
    METHOD_IITUNES_REWIND                  = 9,     // No value is returned
    METHOD_IITUNES_STOP                    = 10,    // No value is returned
    METHOD_IITUNES_GET_SOUNDVOLUME         = 11,    // Returns a long (0-100%)
    METHOD_IITUNES_PUT_SOUNDVOLUME         = 12,    // Takes a long (0-100%); No value is returned
    METHOD_IITUNES_GET_MUTE                = 13,    // Returns a bool
    METHOD_IITUNES_PUT_MUTE                = 14,    // Takes a bool; No value is returned
    METHOD_IITUNES_GET_PLAYERPOSITION      = 15,    // Returns a long (0-100%)
    METHOD_IITUNES_PUT_PLAYERPOSITION      = 16,    // Takes a long (0-100%); No value is returned
    METHOD_IITCHSERVER_GET_PLAYERSTATE     = 17,    // Returns an iTCHPlayerState enumeration value (generated from ITPlayserState)
    METHOD_IITCHSERVER_GET_CURRENTTRACK    = 18,    // Returns an iTCHTrack object (generated from IITTrack)
    METHOD_IITCHSERVER_GET_CURRENTPLAYLIST = 19,    // Returns an iTCHPlayList object (generated from IITPlayList)
    METHOD_IITCHCLIENT_VOLUMECHANGED       = 20,    // Sends a volume changed message to client
    METHOD_IITCHCLIENT_PLAYINGSTARTED      = 21,    // Sends a playing started notification to client
    METHOD_IITCHCLIENT_PLAYINGSTOPPED      = 22,    // Sends a playing stopped notification to client
    METHOD_IITCHCLIENT_TRACKINFOCHANGED    = 23     // Sends a notification that the currently playing track's info has changed to client
  };

public:
  Method(const Method& method);

  Method(SupportedMethods method, const QStringList &params, unsigned int id);

  Method(const QString &json);

  void setMethod(SupportedMethods method);

  SupportedMethods getMethod() const { return method_; }

  void setParams(const QStringList &params) { params_ = params; }

  QStringList getParams() const { return params_; }

  void setID(unsigned int id) { id_ = id; }

  unsigned int getID() const { return id_; }

  QString toJsonRpc() const;

  void fromJsonRpc(const QString &json);

private:
  SupportedMethods method_;
  QStringList params_;                       // Place strings inside doublequotes, to distinguish between strings and numbers (for JSON)
  unsigned int id_;
};

} // end namespace iTCH

#endif // ITCH_METHOD_H
