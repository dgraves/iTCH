#include "iTunesCOMInterface.h"
#include "iTCHController.h"

iTCHController::iTCHController() :
  itunes_(NULL)
{
  createInstance();
}

iTCHController::~iTCHController()
{
  if (itunes_)
  {
    destroyInstance();
  }
}

void iTCHController::callMethod(const iTCHMethod &method)
{
  if (!itunes_)
  {
    // throw exception
  }

  switch (method.getMethod())
  {
  case iTCHMethod::METHOD_IITUNES_BACKTRACK:                        // No value is returned
    itunes_->BackTrack();
    break;
  case iTCHMethod::METHOD_IITUNES_FASTFORWARD:                      // No value is returned
    itunes_->FastForward();
    break;
  case iTCHMethod::METHOD_IITUNES_NEXTTRACK:                        // No value is returned
    itunes_->NextTrack();
    break;
  case iTCHMethod::METHOD_IITUNES_PAUSE:                            // No value is returned
    itunes_->Pause();
    break;
  case iTCHMethod::METHOD_IITUNES_PLAY:                             // No value is returned
    itunes_->Play();
    break;
  case iTCHMethod::METHOD_IITUNES_PLAYPAUSE:                        // No value is returned
    itunes_->PlayPause();
    break;
  case iTCHMethod::METHOD_IITUNES_PREVIOUSTRACK:                    // No value is returned
    itunes_->PreviousTrack();
    break;
  case iTCHMethod::METHOD_IITUNES_RESUME:                           // No value is returned
    itunes_->Resume();
    break;
  case iTCHMethod::METHOD_IITUNES_REWIND:                           // No value is returned
    itunes_->Rewind();
    break;
  case iTCHMethod::METHOD_IITUNES_STOP:                             // No value is returned
    itunes_->Stop();
    break;
  case iTCHMethod::METHOD_IITUNES_GET_SOUNDVOLUME:                  // Returns a long (0-100%)
    break;
  case iTCHMethod::METHOD_IITUNES_PUT_SOUNDVOLUME:                  // Takes a long (0-100%); No value is returned
    break;
  case iTCHMethod::METHOD_IITUNES_GET_MUTE:                         // Returns a bool
    break;
  case iTCHMethod::METHOD_IITUNES_PUT_MUTE:                         // Takes a bool; No value is returned
    break;
  case iTCHMethod::METHOD_IITUNES_GET_PLAYERPOSITION:               // Returns a long (0-100%)
    break;
  case iTCHMethod::METHOD_IITUNES_PUT_PLAYERPOSITION:               // Takes a long (0-100%); No value is returned
    break;
  case iTCHMethod::METHOD_IITCHSERVER_GET_PLAYERSTATE:              // Returns an iTCHPlayerState enumeration value (generated from ITPlayserState)
    break;
  case iTCHMethod::METHOD_IITCHSERVER_GET_CURRENTTRACK:             // Returns an iTCHTrack object (generated from IITTrack)
    break;
  case iTCHMethod::METHOD_IITCHSERVER_GET_CURRENTPLAYLIST:          // Returns an iTCHPlayList object (generated from IITPlayList)
    break;
  default:
    // throw exception
    break;
  }
}

/*
HRESULT hres;
DWORD d;
IConnectionPoint * icp;
IConnectionPointContainer * icpc;
hres = iITunes->QueryInterface(IID_IConnectionPointContainer,(PVOID *)&icpc);
hres = icpc->FindConnectionPoint(DIID__IiTunesEvents,&icp);
eventSink = new ITunesEventSink();
hres = icp->Advise((IUnknown*)&eventSink,&d);
*/

void iTCHController::createInstance()
{
  HRESULT result = ::CoInitialize(0);
  if(FAILED(result))
  {
    // throw exception
  }

  result = ::CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&itunes_);
  if(FAILED(result))
  {
    // throw exception
  }
}

void iTCHController::destroyInstance()
{
  itunes_->Release();
  ::CoUninitialize();

  itunes_ = NULL;
}
