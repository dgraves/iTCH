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


  //--------------- Make server notification RPC messages ---------------
  static EnvelopePtr makeVolumeChangedNotification();

  static EnvelopePtr makePlayingStartedNotification();

  static EnvelopePtr makePlayingStoppedNotification();

  static EnvelopePtr makeTrackInfoChangedNotification();

  static EnvelopePtr makePlayerQuittingNotification();

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

  static EnvelopePtr makeGetPlayerButtonsStateRequest(unsigned int sequenceId);

  static bool containsValidClientRequest(const EnvelopePtr envelope);


  //--------------- Make server status RPC messages ---------------
  // For failure of any request
  static EnvelopePtr makeFailedResponse(unsigned int sequenceId, const std::string &message);

  // For reporting success of requests that do not return a value
  static EnvelopePtr makeSuccessfulResponse(unsigned int sequenceId);

  static EnvelopePtr makeSoundVolumeResponse(unsigned int sequenceId, long volume);

  static EnvelopePtr makeMuteResponse(unsigned int sequenceId, bool isMute);

  static EnvelopePtr makePlayerPositionResponse(unsigned int sequenceId, long position);

  static EnvelopePtr makePlayerStateResponse(unsigned int sequenceId, PlayerState state);

  static EnvelopePtr makeCurrentTrackResponse(unsigned int sequenceId, const Track &track);

  static EnvelopePtr makePlayerButtonsStateResponse(unsigned int sequenceId, const PlayerButtonsState &buttons);

  static bool containsValidServerResponse(const EnvelopePtr envelope);

  static bool containsValidServerResponse(const EnvelopePtr envelope, const ClientRequest &originalRequest);

private:
  ~MessageBuilder();
};

} // end namespace iTCH

#endif // ITCH_METHOD_H
