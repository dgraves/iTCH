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
#ifndef ITCH_CONTROLLER_H
#define ITCH_CONTROLLER_H

#include <QtCore/QTimer>
#include <QtCore/QObject>
#include "iTCH/MessageBuilder.h"

struct IiTunes;
struct IITTrack;
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
  // Static utility functions for iTunesCOMInterface to protobuf type conversion
  static void Controller::convertTrack(IITTrack *iittrack, Track *track);

public:
  Controller();

  virtual ~Controller();

  bool hasInstance() const;

  void processRequest(const ClientRequest &request, Connection *connection);

  void createInstance();

  void destroyInstance();

signals:
  void createdInstance();
  void destroyedInstance();
  void statusChanged(const iTCH::EnvelopePtr envelope);  // Status message to send to clients

protected slots:
  // Slots to handle events from iTunes - called by EventSink friend class
  void play(const Track &track);
  void stop();
  void playingTrackChanged(const Track &track);
  void volumeChanged(long newVolume);
  void aboutToQuit();
  void quitting();

private:
  void sendPlayerPosition(unsigned long sequenceId);  // Generate statusChanged signal for player position

private:
  IiTunes          *itunes_;
  EventSink        *events_;
  IConnectionPoint *eventsConnectionPoint_;
  unsigned long     eventsCookie_;
};

} // end namespace iTCH

#endif // ITCH_CONTROLLER_H
