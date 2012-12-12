/*------------------------------------------------------------------------------
 plugin/djv_quicktime/etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "base.h"

#include "load.h"
#include "save.h"

namespace qt {
#if defined(DJV_MACOSX)
#include <QuickTime/QTML.h>
#elif defined(DJV_WINDOWS)
#include <QTML.h>
#endif
}

namespace djv_plugin {
namespace quicktime {

//------------------------------------------------------------------------------
// Handle
//------------------------------------------------------------------------------

Handle::Handle(qt::Handle in) :
  p(in)
{}

Handle::~Handle()
{
  if (p)
    qt::DisposeHandle(p);
}

void Handle::init(long in) throw (Error)
{
  p = qt::NewHandle(in);
}

//------------------------------------------------------------------------------
// CFStringRef
//------------------------------------------------------------------------------

CFStringRef::CFStringRef(const String & in) :
  p(qt::CFStringCreateWithCString(0, in.c_str(), 0))
{}
  
CFStringRef::~CFStringRef()
{ 
  qt::CFRelease(p);
}

//------------------------------------------------------------------------------
// quicktime_init()
//------------------------------------------------------------------------------

namespace {

int _ref = 0;
bool _init_qtml = false;
bool _enter_movies = false;

}

void quicktime_init() throw (Error)
{
  ++_ref;
  if (_ref > 1)
    return;

  DJV_DEBUG("quicktime_init");

  qt::OSErr err = qt::noErr;

#if defined(DJV_WINDOWS)

  DJV_DEBUG_PRINT("initialize QTML");

  err = qt::InitializeQTML(0); //qt::kInitializeQTMLNoSoundFlag);
  if (err != qt::noErr)
    io::throw_error(name, string::Format(error_init).arg(err));

  _init_qtml = true;

#endif // DJV_WINDOWS

  DJV_DEBUG_PRINT("get version");

  long version = 0;
  err = qt::Gestalt(qt::gestaltQuickTime, &version);
  if (err != qt::noErr)
    io::throw_error(name, string::Format(error_version).arg(err));

  DJV_DEBUG_PRINT("version = " << version);
  
  DJV_DEBUG_PRINT("enter movies");

  err = qt::EnterMovies();
  if (err != qt::noErr)
    io::throw_error(name, string::Format(error_init).arg(err));

  _enter_movies = true;
}

//------------------------------------------------------------------------------
// quicktime_del()
//------------------------------------------------------------------------------

void quicktime_del()
{
  --_ref;
  if (_ref)
    return;
  
  DJV_DEBUG("quicktime_del");

  if (_enter_movies)
  {
    DJV_DEBUG_PRINT("exit movies");

    qt::ExitMovies();

  _enter_movies = false;
  }

#if defined(DJV_WINDOWS)

  if (_init_qtml)
  {
    DJV_DEBUG_PRINT("terminate QTML");

  qt::TerminateQTML();

  _init_qtml = false;
  }

#endif // DJV_WINDOWS
}

//------------------------------------------------------------------------------
// quicktime_codec()
//------------------------------------------------------------------------------

qt::CodecType quicktime_codec(CODEC in)
{
  switch (in)
  {
    case CODEC_RAW: return qt::kRawCodecType;
    case CODEC_JPEG: return qt::kJPEGCodecType;
    case CODEC_MJPEGA: return qt::kMotionJPEGACodecType;
    case CODEC_MJPEGB: return qt::kMotionJPEGBCodecType;
    case CODEC_H263: return qt::kH263CodecType;
    case CODEC_H264: return qt::kH264CodecType;
    case CODEC_DVCNTSC: return qt::kDVCNTSCCodecType;
    case CODEC_DVCPAL: return qt::kDVCPALCodecType;
  }
  
  return 0;
}

//------------------------------------------------------------------------------
// quicktime_quality()
//------------------------------------------------------------------------------

qt::CodecQ quicktime_quality(QUALITY in)
{
  switch (in)
  {
    case QUALITY_LOSSLESS: return qt::codecLosslessQuality;
    case QUALITY_MIN: return qt::codecMinQuality;
    case QUALITY_MAX: return qt::codecMaxQuality;
    case QUALITY_LOW: return qt::codecLowQuality;
    case QUALITY_NORMAL: return qt::codecNormalQuality;
    case QUALITY_HIGH: return qt::codecHighQuality;
  }
  
  return 0;
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(CODEC, label_codec())
_DJV_STRING_OPERATOR_LABEL(QUALITY, label_quality())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_codec()
{
  static const List<String> data = List<String>() <<
    "Raw" <<
    "JPEG" <<
    "MJPEG-A" <<
    "MJPEG-B" <<
    "H263" <<
    "H264" <<
    "DVC-NTSC" <<
    "DVC-PAL";
  
  DJV_ASSERT(data.size() == _CODEC_SIZE);
  
  return data;
}

const List<String> & label_quality()
{
  static const List<String> data = List<String>() <<
    "Lossless" <<
    "Min" <<
    "Max" <<
    "Low" <<
    "Normal" <<
    "High";
  
  DJV_ASSERT(data.size() == _QUALITY_SIZE);
  
  return data;
}

const List<String> & label_load_option()
{
  static const List<String> data = List<String>() <<
    "Start frame";

  DJV_ASSERT(data.size() == _LOAD_OPTION_SIZE);

  return data;
}

const List<String> & label_save_option()
{
  static const List<String> data = List<String>() <<
    "Codec" <<
    "Quality";
  
  DJV_ASSERT(data.size() == _SAVE_OPTION_SIZE);
  
  return data;
}

const String
  error_init = "Cannot initialize (#%%)",
  error_version = "Incompatible version (#%%)",
  error_open = "Cannot open: %% (#%%)",
  error_gworld = "Cannot create GWorld: %% (#%%)",
  error_data_ref = "Cannot create data reference: %% (#%%)",
  error_track = "Cannot create track: %% (#%%)",
  error_media = "Cannot create media: %% (#%%)",
  error_media_begin = "Cannot begin editing media: %% (#%%)",
  error_media_end = "Cannot end editing media: %% (#%%)",
  error_media_add = "Cannot add media: %% (#%%)",
  error_media_sample = "Cannot add frame %%: %% (#%%)",
  error_movie_add = "Cannot add movie: %% (#%%)",
  error_compress = "Cannot compress frame %%: %% (#%%)";

extern const String
  label_start_frame_group = "Start Frame";

extern const String
  label_codec_group = "Codec",
  label_quality_group = "Quality";

//------------------------------------------------------------------------------

}
}

