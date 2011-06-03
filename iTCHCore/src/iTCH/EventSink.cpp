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
#include "iTCH/Controller.h"
#include "iTCH/EventSink.h"

using namespace iTCH;

EventSink::EventSink(Controller *controller) :
  controller_(controller),
  typeInfo_(NULL),
  refCount_(0)
{
}

EventSink::~EventSink()
{
}

bool EventSink::create()
{
  // Retrieve the type information for the _IiTiunesEvents base
  // Requesting iTunes COM SDK version 1.12
  VARIANT_BOOL compatible;
  HRESULT result = controller_->itunes_->CheckVersion(1, 12, &compatible);
  if (FAILED(result))
  {
    controller_->comError("Cannot query iTunes COM SDK version");
    return false;
  }

  if (!compatible)
  {
    controller_->comError("iTunes is not compatible with iTunes COM SDK version 1.2 (it is too old)");
    return false;
  }

  ITypeLib *typeLib = NULL ;
  result = ::LoadRegTypeLib(LIBID_iTunesLib, 1, 12, 0, &typeLib);
  if (FAILED(result))
  {
    controller_->comError("Could not load iTunes COM SDK version 1.2");
    return false;
  }

  result = typeLib->GetTypeInfoOfGuid(DIID__IiTunesEvents, &typeInfo_) ;
  typeLib->Release();
  if (FAILED(result))
  {
    controller_->comError("Could not retrieve iTunes COM SDK type info");
    return false;
  }

  return true;
}

HRESULT STDMETHODCALLTYPE EventSink::QueryInterface(REFIID riid, void **ppvObject)
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

ULONG STDMETHODCALLTYPE EventSink::AddRef()
{
  return ++refCount_;
}

ULONG STDMETHODCALLTYPE EventSink::Release()
{
  // COM specification requires that an object free itself when ref count is 0
  if (--refCount_ == 0)
  {
    delete this;
  }

  return refCount_;
}

HRESULT STDMETHODCALLTYPE EventSink::GetTypeInfoCount(UINT *pctinfo)
{
  *pctinfo = 1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE EventSink::GetTypeInfo(UINT iTInfo, LCID, ITypeInfo **ppTInfo)
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

HRESULT STDMETHODCALLTYPE EventSink::GetIDsOfNames(REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID, DISPID *rgDispId)
{
  if (riid != IID_NULL)
  {
    return DISP_E_UNKNOWNINTERFACE;
  }

  return ::DispGetIDsOfNames(typeInfo_, rgszNames, cNames, rgDispId);
}

HRESULT STDMETHODCALLTYPE EventSink::Invoke(DISPID dispIdMember, REFIID riid, LCID, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *, EXCEPINFO *, UINT *puArgErr)
{
  UINT uArgErr;
  VARIANTARG varg;
  IITTrack* iittrack = NULL;

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
    controller_->play();
    break;
  case ITEventPlayerStop:
    // Raised whenever a track stops playing, either when user clicks
    // stop button or track plays to completion
    controller_->stop();
    break;
  case ITEventPlayerPlayingTrackChanged:
      controller_->playingTrackChanged();
    break;
  case ITEventAboutToPromptUserToQuit:
    controller_->aboutToQuit();
  case ITEventQuitting:
    controller_->quitting();
    break;
  case ITEventSoundVolumeChanged:
    controller_->volumeChanged();
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
