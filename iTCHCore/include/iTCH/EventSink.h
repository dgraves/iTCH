// Event sink for iTunes COM SDK for Windows
// Based on code from http://www.codeproject.com/useritems/iTunes_interact.asp
#ifndef ITCH_EVENTSINK_H
#define ITCH_EVENTSINK_H

#include "iTunesCOMInterface.h"

namespace iTCH
{

class Controller;

class EventSink : public _IiTunesEvents
{
public:
  EventSink(Controller *controller);

  ~EventSink();

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);

  ULONG STDMETHODCALLTYPE AddRef();

  ULONG STDMETHODCALLTYPE Release();

  HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);

  HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID, ITypeInfo **ppTInfo);

  HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID, DISPID *rgDispId);

  HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *, EXCEPINFO *, UINT *puArgErr);

  HRESULT STDMETHODCALLTYPE OnDatabaseChangedEvent(VARIANT deletedObjectIDs, VARIANT changedObjectIDs);

  HRESULT STDMETHODCALLTYPE OnPlayerPlayEvent(VARIANT iTrack);

  HRESULT STDMETHODCALLTYPE OnPlayerStopEvent(VARIANT iTrack);

  HRESULT STDMETHODCALLTYPE OnPlayerPlayingTrackChangedEvent(VARIANT iTrack);

  HRESULT STDMETHODCALLTYPE OnCOMCallsDisabledEvent(ITCOMDisabledReason reason);

  HRESULT STDMETHODCALLTYPE OnCOMCallsEnabledEvent();

  HRESULT STDMETHODCALLTYPE OnQuittingEvent();

  HRESULT STDMETHODCALLTYPE OnAboutToPromptUserToQuitEvent();

  HRESULT STDMETHODCALLTYPE OnSoundVolumeChangedEvent(long newVolume);

private:
  Controller     *controller_;
  ITypeInfo      *typeInfo_;
  ULONG           refCount_;
};

} // end namespace iTCH

#endif // ITCH_EVENTSINK_H
