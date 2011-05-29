#ifndef ITCH_CONTROLLER_H
#define ITCH_CONTROLLER_H

#include <QtCore/QObject>
#include "iTCH/MessageBuilder.h"

struct IiTunes;
struct IConnectionPoint;

namespace iTCH
{

class Connection;

class Controller : public QObject
{
  Q_OBJECT

  // Creating a friend class to receive iTunes events for the controller instead
  // of using multiple inheritance to handle events directly
  friend class EventSink;

public:
  Controller();

  virtual ~Controller();

  bool hasInstance() const;

  void processRequest(const ClientRequest &request, iTCH::Connection *connection);

  void createInstance();

  void destroyInstance();

signals:
  void createdInstance();
  void destroyedInstance();

protected slots:
  // Slots to handle events from iTunes
  void play();
  void stop();
  void playingTrackChanged();
  void volumeChanged(long newVolume);
  void aboutToQuit();
  void quitting();

private:
  IiTunes          *itunes_;
  EventSink        *events_;
  IConnectionPoint *eventsConnectionPoint_;
  unsigned long     eventsCookie_;
};

} // end namespace iTCH

#endif // ITCH_CONTROLLER_H
