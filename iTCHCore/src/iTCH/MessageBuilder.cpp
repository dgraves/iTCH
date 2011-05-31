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

  EnvelopePtr makeServerStatus(unsigned int sequenceId, ServerStatus::Type type)
  {
    EnvelopePtr envelope = MessageBuilder::createEnvelope();
    envelope->set_type(Envelope::SERVERSTATUS);

    ServerStatus *message = envelope->mutable_status();
    assert(message);
    message->set_seqid(sequenceId);
    message->set_type(type);

    return envelope;
  }
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

EnvelopePtr MessageBuilder::makeSoundVolumeStatus(unsigned int sequenceId, long volume)
{
  EnvelopePtr envelope = makeServerStatus(sequenceId, ServerStatus::SOUNDVOLUME);
  ServerStatus *status = envelope->mutable_status();
  status->mutable_value()->set_type(ServerStatus::Value::VOLUME);
  status->mutable_value()->set_volume(volume);
  return envelope;
}

EnvelopePtr MessageBuilder::makeMuteStatus(unsigned int sequenceId, bool isMute)
{
  EnvelopePtr envelope = makeServerStatus(sequenceId, ServerStatus::MUTE);
  ServerStatus *status = envelope->mutable_status();
  status->mutable_value()->set_type(ServerStatus::Value::MUTE);
  status->mutable_value()->set_mute(isMute);
  return envelope;
}

EnvelopePtr MessageBuilder::makePlayerPositionStatus(unsigned int sequenceId, long position)
{
  EnvelopePtr envelope = makeServerStatus(sequenceId, ServerStatus::PLAYERPOSITION);
  ServerStatus *status = envelope->mutable_status();
  status->mutable_value()->set_type(ServerStatus::Value::POSITION);
  status->mutable_value()->set_position(position);
  return envelope;
}

EnvelopePtr MessageBuilder::makePlayerStateStatus(unsigned int sequenceId, PlayerState state)
{
  EnvelopePtr envelope = makeServerStatus(sequenceId, ServerStatus::PLAYERSTATE);
  ServerStatus *status = envelope->mutable_status();
  status->mutable_value()->set_type(ServerStatus::Value::STATE);
  status->mutable_value()->set_state(state);
  return envelope;
}

EnvelopePtr MessageBuilder::makeCurrentTrackStatus(unsigned int sequenceId, const Track &track)
{
  EnvelopePtr envelope = makeServerStatus(sequenceId,ServerStatus::CURRENTTRACK);
  ServerStatus *status = envelope->mutable_status();
  status->mutable_value()->set_type(ServerStatus::Value::TRACK);
  status->mutable_value()->add_track()->CopyFrom(track);
  return envelope;
}

bool MessageBuilder::containsValidServerStatus(const EnvelopePtr envelope)
{
  // Make sure the envelope contains a ServerNotification object
  if (Envelope::Type_IsValid(envelope->type()) && (envelope->type() == Envelope::SERVERSTATUS) && envelope->has_status())
  {
    const ServerStatus &message = envelope->status();

    // First make sure that the status type is valid
    if (ServerStatus::Type_IsValid(message.type()))
    {
      // Make sure that the action types with associated parameters have those parameters
      switch(message.type())
      {
      case ServerStatus::SOUNDVOLUME:
        if (message.has_value() &&
            (message.value().type() == ServerStatus::Value::VOLUME) &&
            message.value().has_volume())
        {
          return true;
        }
        break;
      case ServerStatus::MUTE:
        if (message.has_value() &&
            (message.value().type() == ServerStatus::Value::MUTE) &&
            message.value().has_mute())
        {
          return true;
        }
        break;
      case ServerStatus::PLAYERPOSITION:
        if (message.has_value() &&
            (message.value().type() == ServerStatus::Value::POSITION) &&
            message.value().has_position())
        {
          return true;
        }
        break;
      case ServerStatus::PLAYERSTATE:
        if (message.has_value() &&
            (message.value().type() == ServerStatus::Value::STATE) &&
            message.value().has_state() &&
            iTCH::PlayerState_IsValid(message.value().state()))
        {
          return true;
        }
        break;
      case ServerStatus::CURRENTTRACK:
        // Should have only one track
        if (message.has_value() &&
            (message.value().type() == ServerStatus::Value::TRACK) &&
            (message.value().track_size() == 1))
        {
          return true;
        }
        break;
      case ServerStatus::CURRENTPLAYLIST:
        // Treat playlist as having 0 or more tracks
        if (message.has_value() &&
            (message.value().type() == ServerStatus::Value::TRACK))
        {
          return true;
        }
        break;
      }
    }
  }

  return false;
}

bool MessageBuilder::containsValidServerResponse(const EnvelopePtr envelope, const ClientRequest &originalRequest)
{
  // Make sure the envelope contains a valid ServerStatus object
  if (containsValidServerStatus(envelope))
  {
    const ServerStatus &message = envelope->status();

    // Make sure that the server response has the appropriate return value for the original request
    // First make sure that the request type is valid and sequence IDs match
    if (ClientRequest::Type_IsValid(originalRequest.type()) && (message.seqid() == originalRequest.seqid()))
    {
      // Make sure that the action types with associated parameters have those parameters
      switch(originalRequest.type())
      {
      case ClientRequest::GET_SOUNDVOLUME:
        if (message.type() == ServerStatus::SOUNDVOLUME)
        {
          return true;
        }
        break;
      case ClientRequest::GET_MUTE:
        if (message.type() == ServerStatus::MUTE)
        {
          return true;
        }
        break;
      case ClientRequest::GET_PLAYERPOSITION:
        if (message.type() == ServerStatus::PLAYERPOSITION)
        {
          return true;
        }
        break;
      case ClientRequest::GET_PLAYERSTATE:
        if (message.type() == ServerStatus::PLAYERSTATE)
        {
          return true;
        }
        break;
      case ClientRequest::GET_CURRENTTRACK:
        if (message.type() == ServerStatus::CURRENTTRACK)
        {
          return true;
        }
        break;
      case ClientRequest::GET_CURRENTPLAYLIST:
        if (message.type() == ServerStatus::CURRENTPLAYLIST)
        {
          return true;
        }
        break;
      }
    }
  }

  return false;
}
