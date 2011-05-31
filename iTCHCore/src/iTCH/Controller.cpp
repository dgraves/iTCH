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
#include <atlbase.h>

#include "iTunesCOMInterface.h"
#include "iTCH/Connection.h"
#include "iTCH/EventSink.h"
#include "iTCH/Controller.h"

using namespace iTCH;

namespace
{

inline PlayerState convertState(ITPlayerState state)
{
  switch (state)
  {
  case ITPlayerStateStopped:
    return iTCH::STOPPED;
  case ITPlayerStatePlaying:
    return iTCH::PLAYING;
  case ITPlayerStateFastForward:
    return iTCH::FASTFORWARD;
  case ITPlayerStateRewind:
    return iTCH::REWIND;
  default:
    return iTCH::UNKNOWN;
  }
}

inline QString convertKind(ITTrackKind kind)
{
  switch (kind)
  {
  case ITTrackKindFile:
    return "File track";
  case ITTrackKindCD:
    return "CD track";
  case ITTrackKindURL:
    return "URL track";
  case ITTrackKindDevice:
    return "Device track";
  case ITTrackKindSharedLibrary:
    return "Shared library track";
  case ITTrackKindUnknown:
  default:
    return "Unknown track kind";
  }
}

} // End of anonymous name space

void Controller::convertTrack(IITTrack *iittrack, Track *track)
{
  // Initialize required fields with empty values
  track->set_name("");
  track->set_artist("");
  track->set_album("");
  track->set_duration(0);
  track->set_genre("");
  track->set_year(0);

  if (iittrack != NULL)
  {
    CComBSTR bstr;
    long value = 0;
    ITTrackKind kind = ITTrackKindUnknown;

    // !bstr resolves as true if string is NULL
    if (iittrack->get_Name(&bstr) == S_OK && !bstr == false)
    {
      track->set_name(CW2A(bstr));
    }

    if (iittrack->get_Artist(&bstr) == S_OK && !bstr == false)
    {
      track->set_artist(CW2A(bstr));
    }

    if (iittrack->get_Album(&bstr) == S_OK && !bstr == false)
    {
      track->set_album(CW2A(bstr));
    }

    if (iittrack->get_Duration(&value) == S_OK)
    {
      track->set_duration(value);
    }

    if (iittrack->get_Genre(&bstr) == S_OK && !bstr == false)
    {
      track->set_genre(CW2A(bstr));
    }

    if (iittrack->get_Year(&value) == S_OK)
    {
      track->set_year(value);
    }

    // Non-required fields
    if (iittrack->get_BitRate(&value) == S_OK)
    {
      track->set_bitrate(value);
    }

    if (iittrack->get_SampleRate(&value) == S_OK)
    {
      track->set_sample_rate(value);
    }

    if (iittrack->get_Comment(&bstr) == S_OK && !bstr == false)
    {
      track->set_comment(CW2A(bstr));
    }

    if (iittrack->get_Kind(&kind) == S_OK)
    {
      track->set_kind(convertKind(kind).toStdString());
    }
  }
}

Controller::Controller() :
  itunes_(NULL),
  events_(NULL),
  eventsConnectionPoint_(NULL),
  eventsCookie_(0),
  positionTimerInterval_(1000)  // 1 second interval
{
  connect(&positionTimer_, SIGNAL(timeout()), this, SLOT(positionTimeout()));
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

void Controller::processRequest(const ClientRequest &request, Connection *connection)
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
    {
      long volume = 0;
      itunes_->get_SoundVolume(&volume);
      connection->sendMessage(iTCH::MessageBuilder::makeSoundVolumeStatus(
        request.seqid(), volume));
    }
    break;
  case ClientRequest::PUT_SOUNDVOLUME:                  // Takes a long (0-100%); No value is returned
    itunes_->put_SoundVolume(request.value().volume());
    break;
  case ClientRequest::GET_MUTE:                         // Returns a bool
    {
      VARIANT_BOOL isMute = FALSE;
      itunes_->get_Mute(&isMute);
        connection->sendMessage(iTCH::MessageBuilder::makeMuteStatus(
        request.seqid(), isMute == VARIANT_TRUE ? true : false));
    }
    break;
  case ClientRequest::PUT_MUTE:                         // Takes a bool; No value is returned
    itunes_->put_Mute(request.value().mute());
    break;
  case ClientRequest::GET_PLAYERPOSITION:               // Returns a long (0-100%)
    {
      long position = 0;
      itunes_->get_PlayerPosition(&position);
      connection->sendMessage(iTCH::MessageBuilder::makePlayerPositionStatus(
        request.seqid(), position));
    }
    break;
  case ClientRequest::PUT_PLAYERPOSITION:               // Takes a long (0-100%); No value is returned
    itunes_->put_PlayerPosition(request.value().position());
    break;
  case ClientRequest::GET_PLAYERSTATE:                  // Returns an iTCHPlayerState enumeration value (generated from ITPlayserState)
    {
      ITPlayerState state = ITPlayerStateStopped;
      itunes_->get_PlayerState(&state);
      connection->sendMessage(iTCH::MessageBuilder::makePlayerStateStatus(
        request.seqid(), convertState(state)));
    }
    break;
  case ClientRequest::GET_CURRENTTRACK:                 // Returns an iTCHTrack object (generated from IITTrack)
    {
      IITTrack *iittrack = 0;
      itunes_->get_CurrentTrack(&iittrack);

      Track track;
      convertTrack(iittrack, &track);
      if (iittrack != NULL)
      {
        iittrack->Release();
      }

      connection->sendMessage(iTCH::MessageBuilder::makeCurrentTrackStatus(
        request.seqid(), track));
    }
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

void Controller::play(const Track &track)
{
  // Send play notification and information for playing track
  statusChanged(iTCH::MessageBuilder::makePlayerStateStatus(0, PLAYING));
  statusChanged(iTCH::MessageBuilder::makeCurrentTrackStatus(0, track));

  // Send initial player position
  sendPlayerPosition(0);

  // Start the position timer to periodically transmit player position
  positionTimer_.start(positionTimerInterval_);
}

void Controller::stop()
{
  statusChanged(iTCH::MessageBuilder::makePlayerStateStatus(0, STOPPED));

  // Stop the position timer
  positionTimer_.stop();

  // Send final player position
  sendPlayerPosition(0);
}

void Controller::playingTrackChanged(const Track &track)
{
  statusChanged(iTCH::MessageBuilder::makeCurrentTrackStatus(0, track));
}

void Controller::volumeChanged(long newVolume)
{
  statusChanged(iTCH::MessageBuilder::makeSoundVolumeStatus(0, newVolume));

  // Also send mute state in case it triggered volume change, although
  // I haven't yet encountered a situtation where get_Mute yields true
  VARIANT_BOOL isMute = FALSE;
  itunes_->get_Mute(&isMute);
  statusChanged(iTCH::MessageBuilder::makeMuteStatus(0, isMute == VARIANT_TRUE ? true : false));
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

void Controller::positionTimeout()
{
  // Send current player position
  sendPlayerPosition(0);
}

void Controller::sendPlayerPosition(unsigned long sequenceId)
{
  long position = 0;
  itunes_->get_PlayerPosition(&position);
  statusChanged(iTCH::MessageBuilder::makePlayerPositionStatus(sequenceId, position));
}
