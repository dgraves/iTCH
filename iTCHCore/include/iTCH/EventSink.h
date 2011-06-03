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
#ifndef ITCH_EVENTSINK_H
#define ITCH_EVENTSINK_H

// Event sink for iTunes COM SDK for Windows
// Based on code from http://www.codeproject.com/useritems/iTunes_interact.asp

#include "iTunesCOMInterface.h"

namespace iTCH
{

class Controller;

class EventSink : public _IiTunesEvents
{
public:
  EventSink(Controller *controller);

  virtual ~EventSink();

  // Initialize COM state
  bool create();

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);

  ULONG STDMETHODCALLTYPE AddRef();

  ULONG STDMETHODCALLTYPE Release();

  HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);

  HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID, ITypeInfo **ppTInfo);

  HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID, DISPID *rgDispId);

  HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *, EXCEPINFO *, UINT *puArgErr);

private:
  Controller     *controller_;
  ITypeInfo      *typeInfo_;
  ULONG           refCount_;
};

} // end namespace iTCH

#endif // ITCH_EVENTSINK_H
