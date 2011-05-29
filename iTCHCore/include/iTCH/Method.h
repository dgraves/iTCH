#ifndef ITCH_METHOD_H
#define ITCH_METHOD_H

#include <QtCore/QSharedPointer>
#include "GeneratedCode/iTCH.pb.h"

namespace iTCH
{

typedef QSharedPointer<Envelope> EnvelopePtr;

class MessageBuilder
{
public:
  //--------------- General purpose functions ---------------
  static EnvelopePtr createEnvelope() { return EnvelopePtr(new Envelope()); }


  //--------------- Make server notification RPC messages ---------------
  static EnvelopePtr makeVolumeChangedNotification(unsigned int sequenceId);

  static EnvelopePtr makePlayingStartedNotification(unsigned int sequenceId);

  static EnvelopePtr makePlayingStoppedNotification(unsigned int sequenceId);

  static EnvelopePtr makeTrackInfoChangedNotification(unsigned int sequenceId);

  static bool containsValidServerNotification(const EnvelopePtr envelope);


  //--------------- Make client request RPC messages ---------------
  static EnvelopePtr makeBackTrackRequest(unsigned int sequenceId);

  static EnvelopePtr makeFastForwardRequest(unsigned int sequenceId);

  static EnvelopePtr makeNextTrackRequest(unsigned int sequenceId);

  static EnvelopePtr makePauseRequest(unsigned int sequenceId);

  static EnvelopePtr makePlayRequest(unsigned int sequenceId);

  static EnvelopePtr makePlayPauseRequest(unsigned int sequenceId);

  static EnvelopePtr makePreviousTrackRequest(unsigned int sequenceId);

  static EnvelopePtr makeResumeRequest(unsigned int sequenceId);

  static EnvelopePtr makeRewindRequest(unsigned int sequenceId);

  static EnvelopePtr makeStopRequest(unsigned int sequenceId);

  static EnvelopePtr makeGetSoundVolumeRequest(unsigned int sequenceId);

  static EnvelopePtr makePutSoundVolumeRequest(unsigned int sequenceId, long volume);

  static EnvelopePtr makeGetMuteRequest(unsigned int sequenceId);

  static EnvelopePtr makePutMuteRequest(unsigned int sequenceId, bool isMute);

  static EnvelopePtr makeGetPlayerPositionRequest(unsigned int sequenceId);

  static EnvelopePtr makePutPlayerPositionRequest(unsigned int sequenceId, long position);

  static EnvelopePtr makeGetPlayerStateRequest(unsigned int sequenceId);

  static EnvelopePtr makeGetCurrentTrackRequest(unsigned int sequenceId);

  static EnvelopePtr makeGetCurrentPlaylistRequest(unsigned int sequenceId);

  static bool containsValidClientRequest(const EnvelopePtr envelope);


  //--------------- Make server response RPC messages ---------------
  static EnvelopePtr makeVolumeResponse(unsigned int sequenceId, long volume);

  static EnvelopePtr makeMuteResponse(unsigned int sequenceId, bool isMute);

  static EnvelopePtr makePositionResponse(unsigned int sequenceId, long position);

  static EnvelopePtr makeTrackResponse(unsigned int sequenceId, const Track &track);

  static bool containsValidServerResponse(const EnvelopePtr envelope, const ClientRequest &originalRequest);

private:
  ~MessageBuilder();
};

} // end namespace iTCH

#endif // ITCH_METHOD_H
