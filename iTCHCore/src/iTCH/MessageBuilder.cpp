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
#include <cassert>
#include "GeneratedCode/iTCH.pb.cc"
#include "iTCH/MessageBuilder.h"

using namespace iTCH;

namespace
{
  EnvelopePtr makeServerNotification(ServerNotification::Type type)
  {
    EnvelopePtr envelope = MessageBuilder::createEnvelope();
    envelope->set_type(Envelope::SERVERNOTIFICATION);

    ServerNotification *message = envelope->mutable_notification();
    assert(message);
    message->set_type(type);

    return envelope;
  }

  EnvelopePtr makeClientRequest(unsigned int sequenceId, ClientRequest::Type type)
  {
    EnvelopePtr envelope = MessageBuilder::createEnvelope();
    envelope->set_type(Envelope::CLIENTREQUEST);

    ClientRequest *message = envelope->mutable_request();
    assert(message);
    message->set_seqid(sequenceId);
    message->set_type(type);

    return envelope;
  }

  EnvelopePtr makeServerResponse(unsigned int sequenceId, bool success)
  {
    EnvelopePtr envelope = MessageBuilder::createEnvelope();
    envelope->set_type(Envelope::SERVERRESPONSE);

    ServerResponse *message = envelope->mutable_response();
    assert(message);
    message->set_seqid(sequenceId);
    message->set_success(success);

    return envelope;
  }
}

EnvelopePtr MessageBuilder::makeVolumeChangedNotification()
{
  return makeServerNotification(ServerNotification::VOLUMECHANGED);
}

EnvelopePtr MessageBuilder::makePlayingStartedNotification()
{
  return makeServerNotification(ServerNotification::PLAYINGSTARTED);
}

EnvelopePtr MessageBuilder::makePlayingStoppedNotification()
{
  return makeServerNotification(ServerNotification::PLAYINGSTOPPED);
}

EnvelopePtr MessageBuilder::makeTrackInfoChangedNotification()
{
  return makeServerNotification(ServerNotification::TRACKINFOCHANGED);
}

EnvelopePtr MessageBuilder::makePlayerQuittingNotification()
{
  return makeServerNotification(ServerNotification::PLAYERQUITTING);
}

bool MessageBuilder::containsValidServerNotification(const EnvelopePtr envelope)
{
  // Make sure the envelope contains a ServerNotification object
  if (Envelope::Type_IsValid(envelope->type()) && (envelope->type() == Envelope::SERVERNOTIFICATION) && envelope->has_notification())
  {
    // Check notification type for valid value; no params to check for this message
    const ServerNotification &message = envelope->notification();
    return ServerNotification::Type_IsValid(message.type());
  }
  return false;
}

EnvelopePtr MessageBuilder::makeBackTrackRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::BACKTRACK);
}

EnvelopePtr MessageBuilder::makeFastForwardRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::FASTFORWARD);
}

EnvelopePtr MessageBuilder::makeNextTrackRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::NEXTTRACK);
}

EnvelopePtr MessageBuilder::makePauseRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::PAUSE);
}

EnvelopePtr MessageBuilder::makePlayRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::PLAY);
}

EnvelopePtr MessageBuilder::makePlayPauseRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::PLAYPAUSE);
}

EnvelopePtr MessageBuilder::makePreviousTrackRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::PREVIOUSTRACK);
}

EnvelopePtr MessageBuilder::makeResumeRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::RESUME);
}

EnvelopePtr MessageBuilder::makeRewindRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::REWIND);
}

EnvelopePtr MessageBuilder::makeStopRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::STOP);
}

EnvelopePtr MessageBuilder::makeGetSoundVolumeRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::GET_SOUNDVOLUME);
}

EnvelopePtr MessageBuilder::makePutSoundVolumeRequest(unsigned int sequenceId, long volume)
{
  EnvelopePtr envelope = makeClientRequest(sequenceId, ClientRequest::PUT_SOUNDVOLUME);
  ClientRequest *request = envelope->mutable_request();
  request->mutable_value()->set_type(ClientRequest::Value::VOLUME);
  request->mutable_value()->set_volume(volume);
  return envelope;
}

EnvelopePtr MessageBuilder::makeGetMuteRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::GET_MUTE);
}

EnvelopePtr MessageBuilder::makePutMuteRequest(unsigned int sequenceId, bool isMute)
{
  EnvelopePtr envelope = makeClientRequest(sequenceId, ClientRequest::PUT_MUTE);
  ClientRequest *request = envelope->mutable_request();
  request->mutable_value()->set_type(ClientRequest::Value::MUTE);
  request->mutable_value()->set_mute(isMute);
  return envelope;
}

EnvelopePtr MessageBuilder::makeGetPlayerPositionRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::GET_PLAYERPOSITION);
}

EnvelopePtr MessageBuilder::makePutPlayerPositionRequest(unsigned int sequenceId, long position)
{
  EnvelopePtr envelope = makeClientRequest(sequenceId, ClientRequest::PUT_PLAYERPOSITION);
  ClientRequest *request = envelope->mutable_request();
  request->mutable_value()->set_type(ClientRequest::Value::POSITION);
  request->mutable_value()->set_position(position);
  return envelope;
}

EnvelopePtr MessageBuilder::makeGetPlayerStateRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::GET_PLAYERSTATE);
}

EnvelopePtr MessageBuilder::makeGetCurrentTrackRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::GET_CURRENTTRACK);
}

EnvelopePtr MessageBuilder::makeGetCurrentPlaylistRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::GET_CURRENTPLAYLIST);
}

EnvelopePtr MessageBuilder::makeGetPlayerButtonsStateRequest(unsigned int sequenceId)
{
  return makeClientRequest(sequenceId, ClientRequest::GET_PLAYERBUTTONSSTATE);
}

bool MessageBuilder::containsValidClientRequest(const EnvelopePtr envelope)
{
  // Make sure the envelope contains a ServerNotification object
  if (Envelope::Type_IsValid(envelope->type()) && (envelope->type() == Envelope::CLIENTREQUEST) && envelope->has_request())
  {
    const ClientRequest &message = envelope->request();

    // First make sure that the request type is valid
    if (ClientRequest::Type_IsValid(message.type()))
    {
      // Make sure that the action types with associated parameters have those parameters
      switch(message.type())
      {
      case ClientRequest::PUT_SOUNDVOLUME:
        if (message.has_value() &&
            (message.value().type() == ClientRequest::Value::VOLUME) &&
            message.value().has_volume())
        {
          return true;
        }
        break;
      case ClientRequest::PUT_MUTE:
        if (message.has_value() &&
            (message.value().type() == ClientRequest::Value::MUTE) &&
            message.value().has_mute())
        {
          return true;
        }
        break;
      case ClientRequest::PUT_PLAYERPOSITION:
        if (message.has_value() &&
            (message.value().type() == ClientRequest::Value::POSITION) &&
            message.value().has_position())
        {
          return true;
        }
        break;
      default:
        // This action should not have a value
        if (!message.has_value())
        {
          return true;
        }
        break;
      }
    }
  }

  return false;
}

EnvelopePtr MessageBuilder::makeFailedResponse(unsigned int sequenceId, const std::string &message)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId, false);
  ServerResponse *response = envelope->mutable_response();
  response->set_error_message(message);
  return envelope;
}

EnvelopePtr MessageBuilder::makeSuccessfulResponse(unsigned int sequenceId)
{
  return makeServerResponse(sequenceId, true);
}

EnvelopePtr MessageBuilder::makeSoundVolumeResponse(unsigned int sequenceId, long volume)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId, true);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::VOLUME);
  response->mutable_value()->set_volume(volume);
  return envelope;
}

EnvelopePtr MessageBuilder::makeMuteResponse(unsigned int sequenceId, bool isMute)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId, true);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::MUTE);
  response->mutable_value()->set_mute(isMute);
  return envelope;
}

EnvelopePtr MessageBuilder::makePlayerPositionResponse(unsigned int sequenceId, long position)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId, true);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::POSITION);
  response->mutable_value()->set_position(position);
  return envelope;
}

EnvelopePtr MessageBuilder::makePlayerStateResponse(unsigned int sequenceId, PlayerState state)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId, true);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::STATE);
  response->mutable_value()->set_state(state);
  return envelope;
}

EnvelopePtr MessageBuilder::makeCurrentTrackResponse(unsigned int sequenceId, const Track &track)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId, true);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::TRACK);
  response->mutable_value()->mutable_track()->CopyFrom(track);
  return envelope;
}

EnvelopePtr MessageBuilder::makePlayerButtonsStateResponse(unsigned int sequenceId, const PlayerButtonsState &buttons)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId, true);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::BUTTONS);
  response->mutable_value()->mutable_buttons()->CopyFrom(buttons);
  return envelope;
}

bool MessageBuilder::containsValidServerResponse(const EnvelopePtr envelope)
{
  // Make sure the envelope contains a ServerNotification object
  if (Envelope::Type_IsValid(envelope->type()) && (envelope->type() == Envelope::SERVERRESPONSE) && envelope->has_response())
  {
    const ServerResponse &message = envelope->response();

    // If response indicates failure, it must have an error message
    if (!message.success() && message.has_error_message())
    {
      return true;
    }
    else
    {
      // If response does not have an optional value, it has already passed all validity tests that apply to it
      if (!message.has_value())
      {
        return true;
      }

      // This response has a value, check validity of the value
      if (ServerResponse::Value::Type_IsValid(message.value().type()))
      {
        // Make sure that the action types with associated parameters have those parameters
        switch(message.value().type())
        {
        case ServerResponse::Value::VOLUME:
          if (message.value().has_volume())
          {
            return true;
          }
          break;
        case ServerResponse::Value::MUTE:
          if (message.value().has_mute())
          {
            return true;
          }
          break;
        case ServerResponse::Value::POSITION:
          if (message.value().has_position())
          {
            return true;
          }
          break;
        case ServerResponse::Value::STATE:
          if (message.value().has_state() && iTCH::PlayerState_IsValid(message.value().state()))
          {
            return true;
          }
          break;
        case ServerResponse::Value::TRACK:
          if (message.value().has_track())
          {
            return true;
          }
          break;
        case ServerResponse::Value::PLAYLIST:
          // Playlist can have 0 or more tracks
          return true;
        case ServerResponse::Value::BUTTONS:
          if (message.value().has_buttons())
          {
            return true;
          }
          break;
        }
      }
    }
  }

  return false;
}

bool MessageBuilder::containsValidServerResponse(const EnvelopePtr envelope, const ClientRequest &originalRequest)
{
  // Make sure the envelope contains a valid ServerStatus object
  if (containsValidServerResponse(envelope))
  {
    const ServerResponse &message = envelope->response();

    // Make sure that the server response has the appropriate return value for the original request
    // First make sure that the request type is valid and sequence IDs match
    if (ClientRequest::Type_IsValid(originalRequest.type()) && (message.seqid() == originalRequest.seqid()))
    {
      // We don't check the value type if request failed, failure is technically a valid response
      if (!message.success())
      {
        return true;
      }

      // Make sure that the action types with associated parameters have those parameters
      switch(originalRequest.type())
      {
      case ClientRequest::GET_SOUNDVOLUME:
        if (message.has_value() && message.value().type() == ServerResponse::Value::VOLUME)
        {
          return true;
        }
        break;
      case ClientRequest::GET_MUTE:
        if (message.has_value() && message.value().type() == ServerResponse::Value::MUTE)
        {
          return true;
        }
        break;
      case ClientRequest::GET_PLAYERPOSITION:
        if (message.has_value() && message.value().type() == ServerResponse::Value::POSITION)
        {
          return true;
        }
        break;
      case ClientRequest::GET_PLAYERSTATE:
        if (message.has_value() && message.value().type() == ServerResponse::Value::STATE)
        {
          return true;
        }
        break;
      case ClientRequest::GET_CURRENTTRACK:
        if (message.has_value() && message.value().type() == ServerResponse::Value::TRACK)
        {
          return true;
        }
        break;
      case ClientRequest::GET_CURRENTPLAYLIST:
        if (message.has_value() && message.value().type() == ServerResponse::Value::PLAYLIST)
        {
          return true;
        }
        break;
      case ClientRequest::GET_PLAYERBUTTONSSTATE:
        if (message.has_value() && message.value().type() == ServerResponse::Value::BUTTONS)
        {
          return true;
        }
        break;
      }
    }
  }

  return false;
}
