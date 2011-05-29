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
#include "iTunesCOMInterface.h"
#include "iTCH/EventSink.h"
#include "iTCH/Controller.h"

using namespace iTCH;

Controller::Controller() :
  itunes_(NULL),
  events_(NULL),
  eventsConnectionPoint_(NULL),
  eventsCookie_(0)
{
}

Controller::~Controller()
{
  if (hasInstance())
  {
    destroyInstance();
  }
}

bool Controller::hasInstance() const
{
  return itunes_ != NULL;
}

void Controller::processRequest(const ClientRequest &request, iTCH::Connection *connection)
{
  if (!hasInstance())
  {
    // throw exception
  }

  switch (request.type())
  {
  case ClientRequest::BACKTRACK:                        // No value is returned
    itunes_->BackTrack();
    break;
  case ClientRequest::FASTFORWARD:                      // No value is returned
    itunes_->FastForward();
    break;
  case ClientRequest::NEXTTRACK:                        // No value is returned
    itunes_->NextTrack();
    break;
  case ClientRequest::PAUSE:                            // No value is returned
    itunes_->Pause();
    break;
  case ClientRequest::PLAY:                             // No value is returned
    itunes_->Play();
    break;
  case ClientRequest::PLAYPAUSE:                        // No value is returned
    itunes_->PlayPause();
    break;
  case ClientRequest::PREVIOUSTRACK:                    // No value is returned
    itunes_->PreviousTrack();
    break;
  case ClientRequest::RESUME:                           // No value is returned
    itunes_->Resume();
    break;
  case ClientRequest::REWIND:                           // No value is returned
    itunes_->Rewind();
    break;
  case ClientRequest::STOP:                             // No value is returned
    itunes_->Stop();
    break;
  case ClientRequest::GET_SOUNDVOLUME:                  // Returns a long (0-100%)
    break;
  case ClientRequest::PUT_SOUNDVOLUME:                  // Takes a long (0-100%); No value is returned
    itunes_->put_SoundVolume(request.value().volume());
    break;
  case ClientRequest::GET_MUTE:                         // Returns a bool
    break;
  case ClientRequest::PUT_MUTE:                         // Takes a bool; No value is returned
    itunes_->put_Mute(request.value().mute());
    break;
  case ClientRequest::GET_PLAYERPOSITION:               // Returns a long (0-100%)
    break;
  case ClientRequest::PUT_PLAYERPOSITION:               // Takes a long (0-100%); No value is returned
    itunes_->put_PlayerPosition(request.value().position());
    break;
  case ClientRequest::GET_PLAYERSTATE:                  // Returns an iTCHPlayerState enumeration value (generated from ITPlayserState)
    break;
  case ClientRequest::GET_CURRENTTRACK:                 // Returns an iTCHTrack object (generated from IITTrack)
    break;
  case ClientRequest::GET_CURRENTPLAYLIST:              // Returns an iTCHPlayList object (generated from IITPlayList)
    break;
  default:
    // throw exception
    break;
  }
}

void Controller::createInstance()
{
  if (hasInstance())
  {
    // Throw exception
  }

  HRESULT result = ::CoInitialize(0);
  if(FAILED(result))
  {
    // throw exception
      int t;
      t = 0;
  }

  result = ::CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&itunes_);
  if(FAILED(result))
  {
    ::CoUninitialize();
    itunes_ = NULL;
    // throw exception
      int t;
      t = 0;
  }

  try
  {
    IConnectionPointContainer *icpc;
    result = itunes_->QueryInterface(IID_IConnectionPointContainer, (PVOID *)&icpc);
    if(FAILED(result))
    {
      // throw exception
      int t;
      t = 0;
    }

    result = icpc->FindConnectionPoint(DIID__IiTunesEvents, &eventsConnectionPoint_);
    icpc->Release();
    if(FAILED(result))
    {
      // throw exception
      int t;
      t = 0;
    }

    events_ = new EventSink(this);
    events_->AddRef();
    result = eventsConnectionPoint_->Advise((IUnknown *)events_, &eventsCookie_);
    if(FAILED(result))
    {
      events_->Release();
      events_ = NULL;

      // throw exception
      int t;
      t = 0;
    }
  }
  catch(...)
  {
    itunes_->Release();
    ::CoUninitialize();

    itunes_ = NULL;

    // Rethrow exception
  }

  createdInstance();
}

void Controller::destroyInstance()
{
  if (!hasInstance())
  {
    // Throw exception
  }

  eventsConnectionPoint_->Unadvise(eventsCookie_);
  eventsConnectionPoint_->Release();
  events_->Release();
  itunes_->Release();
  ::CoUninitialize();

  eventsConnectionPoint_ = NULL;
  events_ = NULL;
  itunes_ = NULL;

  destroyedInstance();
}

void Controller::play()
{
}

void Controller::stop()
{
}

void Controller::playingTrackChanged()
{
}

void Controller::volumeChanged(long newVolume)
{
}

void Controller::aboutToQuit()
{
  if (hasInstance())
  {
    destroyInstance();
  }
}

void Controller::quitting()
{
  if (hasInstance())
  {
    destroyInstance();
  }
}
