// Event sink for iTunes COM SDK for Windows
// Based on code from http://www.codeproject.com/useritems/iTunes_interact.asp
#ifndef ITCHEVENTSINK_H
#define ITCHEVENTSINK_H

class iTCHController;
struct ITypeInfo;

class iTCHEventSink : public _IiTunesEvents
{
public:
  iTCHEventSink(iTCHController *controller);

  ~iTCHEventSink();

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
  iTCHController *controller_;
  ITypeInfo      *typeInfo_;
  ULONG           refCount_;
};

#endif // ITCHEVENTSINK_H
