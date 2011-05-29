#include <cassert>
#include "GeneratedCode/iTCH.pb.cc"
#include "iTCH/Method.h"

using namespace iTCH;

namespace
{
  EnvelopePtr makeServerNotification(unsigned int sequenceId, ServerNotification::Type type)
  {
    EnvelopePtr envelope = MessageBuilder::createEnvelope();
    envelope->set_type(Envelope::SERVERNOTIFICATION);

    ServerNotification *message = envelope->mutable_notification();
    assert(message);
    message->set_seqid(sequenceId);
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

  EnvelopePtr makeServerResponse(unsigned int sequenceId)
  {
    EnvelopePtr envelope = MessageBuilder::createEnvelope();
    envelope->set_type(Envelope::SERVERRESPONSE);

    ServerResponse *message = envelope->mutable_response();
    assert(message);
    message->set_seqid(sequenceId);

    return envelope;
  }
}

EnvelopePtr MessageBuilder::makeVolumeChangedNotification(unsigned int sequenceId)
{
  return makeServerNotification(sequenceId, ServerNotification::VOLUMECHANGED);
}

EnvelopePtr MessageBuilder::makePlayingStartedNotification(unsigned int sequenceId)
{
  return makeServerNotification(sequenceId, ServerNotification::PLAYINGSTARTED);
}

EnvelopePtr MessageBuilder::makePlayingStoppedNotification(unsigned int sequenceId)
{
  return makeServerNotification(sequenceId, ServerNotification::PLAYINGSTOPPED);
}

EnvelopePtr MessageBuilder::makeTrackInfoChangedNotification(unsigned int sequenceId)
{
  return makeServerNotification(sequenceId, ServerNotification::TRACKINFOCHANGED);
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

EnvelopePtr MessageBuilder::makeVolumeResponse(unsigned int sequenceId, long volume)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::VOLUME);
  response->mutable_value()->set_volume(volume);
  return envelope;
}

EnvelopePtr MessageBuilder::makeMuteResponse(unsigned int sequenceId, bool isMute)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::MUTE);
  response->mutable_value()->set_mute(isMute);
  return envelope;
}

EnvelopePtr MessageBuilder::makePositionResponse(unsigned int sequenceId, long position)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::POSITION);
  response->mutable_value()->set_position(position);
  return envelope;
}

EnvelopePtr MessageBuilder::makeTrackResponse(unsigned int sequenceId, const Track &track)
{
  EnvelopePtr envelope = makeServerResponse(sequenceId);
  ServerResponse *response = envelope->mutable_response();
  response->mutable_value()->set_type(ServerResponse::Value::TRACK);
  response->mutable_value()->add_track()->CopyFrom(track);
  return envelope;
}

bool MessageBuilder::containsValidServerResponse(const EnvelopePtr envelope, const ClientRequest &originalRequest)
{
  // Make sure the envelope contains a ServerNotification object
  if (Envelope::Type_IsValid(envelope->type()) && (envelope->type() == Envelope::SERVERRESPONSE) && envelope->has_response())
  {
    const ServerResponse &message = envelope->response();

    // Make sure that the server response has the appropriate return value for the original request
    // Should this also check the sequence id? it currently does not
    // First make sure that the request type is valid
    if (ClientRequest::Type_IsValid(originalRequest.type()))
    {
      // Make sure that the action types with associated parameters have those parameters
      switch(originalRequest.type())
      {
      case ClientRequest::GET_SOUNDVOLUME:
        if (message.has_value() &&
            (message.value().type() == ServerResponse::Value::VOLUME) &&
            message.value().has_volume())
        {
          return true;
        }
        break;
      case ClientRequest::GET_MUTE:
        if (message.has_value() &&
            (message.value().type() == ClientRequest::Value::MUTE) &&
            message.value().has_mute())
        {
          return true;
        }
        break;
      case ClientRequest::GET_PLAYERPOSITION:
        if (message.has_value() &&
            (message.value().type() == ServerResponse::Value::POSITION) &&
            message.value().has_position())
        {
          return true;
        }
        break;
      case ClientRequest::GET_PLAYERSTATE:
        if (message.has_value() &&
            (message.value().type() == ServerResponse::Value::STATE) &&
            message.value().has_state() &&
            iTCH::PlayerState_IsValid(message.value().state()))
        {
          return true;
        }
        break;
      case ClientRequest::GET_CURRENTTRACK:
        // Should have only one track
        if (message.has_value() &&
            (message.value().type() == ServerResponse::Value::TRACK) &&
            (message.value().track_size() == 1))
        {
          return true;
        }
        break;
      case ClientRequest::GET_CURRENTPLAYLIST:
        // Treat playlist as having 0 or more tracks
        if (message.has_value() &&
            (message.value().type() == ServerResponse::Value::TRACK))
        {
          return true;
        }
        break;
      default:
        // Response to this action should not have a value
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
