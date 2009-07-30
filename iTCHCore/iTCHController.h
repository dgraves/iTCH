#ifndef ITCHCONTROLLER_H
#define ITCHCONTROLLER_H

#include <QtCore/QObject>
#include "iTCHMethod.h"

struct IiTunes;
struct IConnectionPoint;

class iTCHController : public QObject
{
  Q_OBJECT

  // Creating a friend class to receive iTunes events for the controller instead
  // of using multiple inheritance to handle events directly
  friend class iTCHEventSink;

public:
  iTCHController();

  ~iTCHController();

  bool hasInstance() const;

  void callMethod(const iTCHMethod& method);

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
  IiTunes       *itunes_;
  iTCHEventSink *events_;
  IConnectionPoint *eventsConnectionPoint_;
  unsigned long  eventsCookie_;
};

#endif // ITCHCONTROLLER_H
