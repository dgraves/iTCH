#ifndef ITCH_MESSAGEBUILDER_H
#define ITCH_MESSAGEBUILDER_H

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


  //--------------- Make server status RPC messages ---------------
  static EnvelopePtr makeSoundVolumeStatus(unsigned int sequenceId, long volume);

  static EnvelopePtr makeMuteStatus(unsigned int sequenceId, bool isMute);

  static EnvelopePtr makePlayerPositionStatus(unsigned int sequenceId, long position);

  static EnvelopePtr makePlayerStateStatus(unsigned int sequenceId, PlayerState state);

  static EnvelopePtr makeTrackStatus(unsigned int sequenceId, const Track &track);

  static bool containsValidServerStatus(const EnvelopePtr envelope);

  static bool containsValidServerResponse(const EnvelopePtr envelope, const ClientRequest &originalRequest);

private:
  ~MessageBuilder();
};

} // end namespace iTCH

#endif // ITCH_METHOD_H
