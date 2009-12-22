#include "iTunesCOMInterface.h"
#include "iTCHController.h"
#include "iTCHEventSink.h"

iTCHEventSink::iTCHEventSink(iTCHController *controller) :
  controller_(controller),
  typeInfo_(NULL),
  refCount_(0)
{
  // Retrieve the type information for the _IiTiunesEvents base
  // All functionality needed by this class was provided with the
  // iTunes COM SDK version 1.2
  VARIANT_BOOL compatible;
  HRESULT result = controller_->itunes_->CheckVersion(1, 2, &compatible);
  if (FAILED(result))
  {
    // throw exception
  }

  if (!compatible)
  {
    // throw exception
  }

  ITypeLib *typeLib = NULL ;
  result = ::LoadRegTypeLib(LIBID_iTunesLib, 1, 2, 0, &typeLib);
  if (FAILED(result))
  {
    // throw exception
  }

  result = typeLib->GetTypeInfoOfGuid(DIID__IiTunesEvents, &typeInfo_) ;
  typeLib->Release();
  if (FAILED(result))
  {
    // throw exception
  }
}

iTCHEventSink::~iTCHEventSink()
{
}

HRESULT STDMETHODCALLTYPE iTCHEventSink::QueryInterface(REFIID riid, void **ppvObject)
{
  if (ppvObject == NULL)
  {
    return E_POINTER;
  }

  if (::IsEqualIID(riid, IID_IDispatch) || ::IsEqualIID(riid, DIID__IiTunesEvents) || ::IsEqualIID(riid, IID_IUnknown))
  {
    AddRef();
    *ppvObject = this;
    return S_OK;
  }
  else
  {
    *ppvObject = NULL;
    return E_NOINTERFACE;
  }
}

ULONG STDMETHODCALLTYPE iTCHEventSink::AddRef()
{
  return ++refCount_;
}

ULONG STDMETHODCALLTYPE iTCHEventSink::Release()
{
  // COM specification requires that an object free itself when ref count is 0
  if (--refCount_ == 0)
  {
    delete this;
  }

  return refCount_;
}

HRESULT STDMETHODCALLTYPE iTCHEventSink::GetTypeInfoCount(UINT *pctinfo)
{
  *pctinfo = 1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE iTCHEventSink::GetTypeInfo(UINT iTInfo, LCID, ITypeInfo **ppTInfo)
{
  if (ppTInfo == NULL)
  {
    return E_POINTER;
  }

  if (iTInfo != 0)
  {
    *ppTInfo = NULL;
    return DISP_E_BADINDEX;
  }
  else
  {
    // If anyone asks, were _IiTunesEvents
    typeInfo_->AddRef();
    *ppTInfo = typeInfo_;
    return S_OK;
  }
}

HRESULT STDMETHODCALLTYPE iTCHEventSink::GetIDsOfNames(REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID, DISPID *rgDispId)
{
  if (riid != IID_NULL)
  {
    return DISP_E_UNKNOWNINTERFACE;
  }

  return ::DispGetIDsOfNames(typeInfo_, rgszNames, cNames, rgDispId);
}

HRESULT STDMETHODCALLTYPE iTCHEventSink::Invoke(DISPID dispIdMember, REFIID riid, LCID, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *, EXCEPINFO *, UINT *puArgErr)
{
  UINT uArgErr;
  HRESULT hresult;
  VARIANTARG varg;
  IITTrack* track = NULL;

  if (!::IsEqualIID(riid, IID_NULL))
  {
    return DISP_E_UNKNOWNINTERFACE;
  }

  if (wFlags & ~(DISPATCH_METHOD|DISPATCH_PROPERTYGET|DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF))
  {
    return E_INVALIDARG;
  }

  if (puArgErr == NULL)
  {
    puArgErr = &uArgErr;
  }

  ::VariantInit(&varg);

  switch (dispIdMember)
  {
  case ITEventPlayerPlay:
    hresult = ::DispGetParam(pDispParams, 0, VT_DISPATCH, &varg, puArgErr);
    if(hresult != NOERROR)
    {
      return hresult;
    }

    hresult = V_DISPATCH(&varg)->QueryInterface(IID_IITTrack, (void **)&track);
    if(hresult != NOERROR)
    {
      return hresult;
    }

    track->Release();
    break;
  case ITEventPlayerStop:
    hresult = ::DispGetParam(pDispParams, 0, VT_DISPATCH, &varg, puArgErr);
    if(hresult != NOERROR)
    {
      return hresult;
    }
    break;
  case ITEventPlayerPlayingTrackChanged:
    hresult = ::DispGetParam(pDispParams, 0, VT_DISPATCH, &varg, puArgErr);
    if(hresult != NOERROR)
    {
      return hresult;
    }
    break;
  case ITEventAboutToPromptUserToQuit:
    controller_->aboutToQuit();
  case ITEventQuitting:
    controller_->quitting();
    break;
  case ITEventSoundVolumeChanged:
    if (pDispParams->cArgs != 1)
    {
      return DISP_E_PARAMNOTFOUND;
    }

    // Need to extract a long from the arg list
    hresult = ::DispGetParam(pDispParams, 0, VT_I4, &varg, puArgErr);
    if(hresult != NOERROR)
    {
      return hresult;
    }

    controller_->volumeChanged(V_I4(&varg));
    break;
  case ITEventDatabaseChanged:
  case ITEventCOMCallsDisabled:
  case ITEventCOMCallsEnabled:
  case ITEventUserInterfaceEnabled:
  default:
    break;
  }

  return S_OK;
}
