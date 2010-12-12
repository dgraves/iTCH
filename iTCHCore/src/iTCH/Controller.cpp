#include "iTunesCOMInterface.h"
#include "iTCH/EventSink.h"
#include "iTCH/Controller.h"

using namespace iTCH;

Controller::Controller() :
  itunes_(NULL),
  events_(NULL),
  eventsConnectionPoint_(NULL),
  eventsCookie_(0)
{
}

Controller::~Controller()
{
  if (hasInstance())
  {
    destroyInstance();
  }
}

bool Controller::hasInstance() const
{
  return itunes_ != NULL;
}

void Controller::callMethod(const Method &method)
{
  if (!hasInstance())
  {
    // throw exception
  }

  switch (method.getMethod())
  {
  case Method::METHOD_IITUNES_BACKTRACK:                        // No value is returned
    itunes_->BackTrack();
    break;
  case Method::METHOD_IITUNES_FASTFORWARD:                      // No value is returned
    itunes_->FastForward();
    break;
  case Method::METHOD_IITUNES_NEXTTRACK:                        // No value is returned
    itunes_->NextTrack();
    break;
  case Method::METHOD_IITUNES_PAUSE:                            // No value is returned
    itunes_->Pause();
    break;
  case Method::METHOD_IITUNES_PLAY:                             // No value is returned
    itunes_->Play();
    break;
  case Method::METHOD_IITUNES_PLAYPAUSE:                        // No value is returned
    itunes_->PlayPause();
    break;
  case Method::METHOD_IITUNES_PREVIOUSTRACK:                    // No value is returned
    itunes_->PreviousTrack();
    break;
  case Method::METHOD_IITUNES_RESUME:                           // No value is returned
    itunes_->Resume();
    break;
  case Method::METHOD_IITUNES_REWIND:                           // No value is returned
    itunes_->Rewind();
    break;
  case Method::METHOD_IITUNES_STOP:                             // No value is returned
    itunes_->Stop();
    break;
  case Method::METHOD_IITUNES_GET_SOUNDVOLUME:                  // Returns a long (0-100%)
    break;
  case Method::METHOD_IITUNES_PUT_SOUNDVOLUME:                  // Takes a long (0-100%); No value is returned
    itunes_->put_SoundVolume(method.getParams()[0].toLong());
    break;
  case Method::METHOD_IITUNES_GET_MUTE:                         // Returns a bool
    break;
  case Method::METHOD_IITUNES_PUT_MUTE:                         // Takes a bool; No value is returned
    break;
  case Method::METHOD_IITUNES_GET_PLAYERPOSITION:               // Returns a long (0-100%)
    break;
  case Method::METHOD_IITUNES_PUT_PLAYERPOSITION:               // Takes a long (0-100%); No value is returned
    itunes_->put_PlayerPosition(method.getParams()[0].toLong());
    break;
  case Method::METHOD_IITCHSERVER_GET_PLAYERSTATE:              // Returns an iTCHPlayerState enumeration value (generated from ITPlayserState)
    break;
  case Method::METHOD_IITCHSERVER_GET_CURRENTTRACK:             // Returns an iTCHTrack object (generated from IITTrack)
    break;
  case Method::METHOD_IITCHSERVER_GET_CURRENTPLAYLIST:          // Returns an iTCHPlayList object (generated from IITPlayList)
    break;
  default:
    // throw exception
    break;
  }
}

void Controller::createInstance()
{
  if (hasInstance())
  {
    // Throw exception
  }

  HRESULT result = ::CoInitialize(0);
  if(FAILED(result))
  {
    // throw exception
      int t;
      t = 0;
  }

  result = ::CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&itunes_);
  if(FAILED(result))
  {
    ::CoUninitialize();
    itunes_ = NULL;
    // throw exception
      int t;
      t = 0;
  }

  try
  {
    IConnectionPointContainer *icpc;
    result = itunes_->QueryInterface(IID_IConnectionPointContainer, (PVOID *)&icpc);
    if(FAILED(result))
    {
      // throw exception
      int t;
      t = 0;
    }

    result = icpc->FindConnectionPoint(DIID__IiTunesEvents, &eventsConnectionPoint_);
    icpc->Release();
    if(FAILED(result))
    {
      // throw exception
      int t;
      t = 0;
    }

    events_ = new EventSink(this);
    events_->AddRef();
    result = eventsConnectionPoint_->Advise((IUnknown *)events_, &eventsCookie_);
    if(FAILED(result))
    {
      events_->Release();
      events_ = NULL;

      // throw exception
      int t;
      t = 0;
    }
  }
  catch(...)
  {
    itunes_->Release();
    ::CoUninitialize();

    itunes_ = NULL;

    // Rethrow exception
  }

  createdInstance();
}

void Controller::destroyInstance()
{
  if (!hasInstance())
  {
    // Throw exception
  }

  eventsConnectionPoint_->Unadvise(eventsCookie_);
  eventsConnectionPoint_->Release();
  events_->Release();
  itunes_->Release();
  ::CoUninitialize();

  eventsConnectionPoint_ = NULL;
  events_ = NULL;
  itunes_ = NULL;

  destroyedInstance();
}

void Controller::play()
{
}

void Controller::stop()
{
}

void Controller::playingTrackChanged()
{
}

void Controller::volumeChanged(long newVolume)
{
}

void Controller::aboutToQuit()
{
  if (hasInstance())
  {
    destroyInstance();
  }
}

void Controller::quitting()
{
  if (hasInstance())
  {
    destroyInstance();
  }
}
