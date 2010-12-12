#ifndef ITCH_CONTROLLER_H
#define ITCH_CONTROLLER_H

#include <QtCore/QObject>
#include "iTCH/Method.h"

struct IiTunes;
struct IConnectionPoint;

namespace iTCH
{

class Controller : public QObject
{
  Q_OBJECT

  // Creating a friend class to receive iTunes events for the controller instead
  // of using multiple inheritance to handle events directly
  friend class EventSink;

public:
  Controller();

  ~Controller();

  bool hasInstance() const;

  void callMethod(const Method& method);

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
