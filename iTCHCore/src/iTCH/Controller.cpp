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
  // Create a server response success or failure message for requests that
  // do not receive a return value, with an error message on failure
  inline EnvelopePtr createNoValueResponse(unsigned int seqid, HRESULT result)
  {
    if (result == S_OK)
    {
      return MessageBuilder::makeSuccessfulResponse(seqid);
    }
    else
    {
      return MessageBuilder::makeFailedResponse(seqid, "An unexpected error occurred");
    }
  }

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

void Controller::processRequest(const ClientRequest &request, Connection *connection)
{
  if (!hasInstance())
  {
    connection->sendMessage(MessageBuilder::makeFailedResponse(request.seqid(),
      "Server is not connected to an iTunes instace"));
  }

  HRESULT result;

  switch (request.type())
  {
  case ClientRequest::BACKTRACK:                        // No value is returned
    result = itunes_->BackTrack();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::FASTFORWARD:                      // No value is returned
    result = itunes_->FastForward();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::NEXTTRACK:                        // No value is returned
    result = itunes_->NextTrack();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::PAUSE:                            // No value is returned
    result = itunes_->Pause();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::PLAY:                             // No value is returned
    result = itunes_->Play();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::PLAYPAUSE:                        // No value is returned
    result = itunes_->PlayPause();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::PREVIOUSTRACK:                    // No value is returned
    result = itunes_->PreviousTrack();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::RESUME:                           // No value is returned
    result = itunes_->Resume();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::REWIND:                           // No value is returned
    result = itunes_->Rewind();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::STOP:                             // No value is returned
    result = itunes_->Stop();
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::GET_SOUNDVOLUME:                  // Returns a long (0-100%)
    {
      long volume = 0;
      result = itunes_->get_SoundVolume(&volume);
      if (result == S_OK)
      {
        connection->sendMessage(iTCH::MessageBuilder::makeSoundVolumeResponse(
          request.seqid(), volume));
      }
      else
      {
        connection->sendMessage(createNoValueResponse(request.seqid(), result));
      }
    }
    break;
  case ClientRequest::PUT_SOUNDVOLUME:                  // Takes a long (0-100%); No value is returned
    result = itunes_->put_SoundVolume(request.value().volume());
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::GET_MUTE:                         // Returns a bool
    {
      VARIANT_BOOL isMute = FALSE;
      result = itunes_->get_Mute(&isMute);
      if (result == S_OK)
      {
        connection->sendMessage(iTCH::MessageBuilder::makeMuteResponse(
          request.seqid(), isMute == VARIANT_TRUE ? true : false));
      }
      else
      {
        connection->sendMessage(createNoValueResponse(request.seqid(), result));
      }
    }
    break;
  case ClientRequest::PUT_MUTE:                         // Takes a bool; No value is returned
    result = itunes_->put_Mute(request.value().mute());
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::GET_PLAYERPOSITION:               // Returns a long (0-100%)
    {
      long position = 0;
      result = itunes_->get_PlayerPosition(&position);
      if (result == S_OK)
      {
        connection->sendMessage(iTCH::MessageBuilder::makePlayerPositionResponse(
          request.seqid(), position));
      }
      else
      {
        connection->sendMessage(createNoValueResponse(request.seqid(), result));
      }
    }
    break;
  case ClientRequest::PUT_PLAYERPOSITION:               // Takes a long (0-100%); No value is returned
    result = itunes_->put_PlayerPosition(request.value().position());
    connection->sendMessage(createNoValueResponse(request.seqid(), result));
    break;
  case ClientRequest::GET_PLAYERSTATE:                  // Returns an iTCHPlayerState enumeration value (generated from ITPlayserState)
    {
      ITPlayerState state = ITPlayerStateStopped;
      result = itunes_->get_PlayerState(&state);
      if (result == S_OK)
      {
        connection->sendMessage(iTCH::MessageBuilder::makePlayerStateResponse(
          request.seqid(), convertState(state)));
      }
      else
      {
        connection->sendMessage(createNoValueResponse(request.seqid(), result));
      }
    }
    break;
  case ClientRequest::GET_CURRENTTRACK:                 // Returns an iTCHTrack object (generated from IITTrack)
    {
      IITTrack *iittrack = 0;
      result = itunes_->get_CurrentTrack(&iittrack);
      if (result == S_OK)
      {
        Track track;
        convertTrack(iittrack, &track);
        if (iittrack != NULL)
        {
          iittrack->Release();
        }

        connection->sendMessage(iTCH::MessageBuilder::makeCurrentTrackResponse(
          request.seqid(), track));
      }
      else
      {
        connection->sendMessage(createNoValueResponse(request.seqid(), result));
      }
    }
    break;
  case ClientRequest::GET_CURRENTPLAYLIST:              // Returns an iTCHPlayList object (generated from IITPlayList)
    break;
  default:
    // throw exception
    break;
  }
}

bool Controller::createInstance()
{
  if (!hasInstance())
  {
    HRESULT result = ::CoInitialize(0);
    if(FAILED(result))
    {
      comError("Failed to initialize COM library");
      return false;
    }

    result = ::CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&itunes_);
    if(FAILED(result))
    {
      comError("Failed to create IiTunes COM instance");
      ::CoUninitialize();
      itunes_ = NULL;
      return false;
    }

    try
    {
      IConnectionPointContainer *icpc;
      result = itunes_->QueryInterface(IID_IConnectionPointContainer, (PVOID *)&icpc);
      if(FAILED(result))
      {
        throw 0;
      }

      result = icpc->FindConnectionPoint(DIID__IiTunesEvents, &eventsConnectionPoint_);
      icpc->Release();
      if(FAILED(result))
      {
        throw 0;
      }

      events_ = new EventSink(this);
      if (!events_->create())
      {
        throw 0;
      }

      events_->AddRef();
      result = eventsConnectionPoint_->Advise((IUnknown *)events_, &eventsCookie_);
      if(FAILED(result))
      {
        events_->Release();
        events_ = NULL;
        throw 0;
      }
    }
    catch(...)
    {
      itunes_->Release();
      itunes_ = NULL;

      ::CoUninitialize();

      return false;
    }

    createdInstance();
  }

  return true;
}

void Controller::destroyInstance()
{
  if (hasInstance())
  {
    // Need to send stopped notification to clients
    statusChanged(iTCH::MessageBuilder::makePlayingStoppedNotification());

    // Disconnect from player
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
}

void Controller::play()
{
  // Send play notification and information for playing track
  statusChanged(MessageBuilder::makePlayingStartedNotification());
}

void Controller::stop()
{
  statusChanged(MessageBuilder::makePlayingStoppedNotification());
}

void Controller::playingTrackChanged()
{
  statusChanged(MessageBuilder::makeTrackInfoChangedNotification());
}

void Controller::volumeChanged()
{
  statusChanged(MessageBuilder::makeVolumeChangedNotification());
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
