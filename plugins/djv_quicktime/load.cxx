/*------------------------------------------------------------------------------
 plugin/djv_quicktime/load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "load.h"

namespace qt {
#if defined(DJV_WINDOWS)
#include <CFString.h>
#include <ImageCompression.h>
#endif
}

namespace djv_plugin {
namespace quicktime {

//------------------------------------------------------------------------------
// djv_image_load()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_load()
{
  return new Load;
}

}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
  _start_frame(0),
  _movie(0),
  _gworld(0)
{}

Load::~Load()
{
  _close();
}

void Load::init() throw (Error)
{
  quicktime_init();
}

void Load::del()
{
  quicktime_del();
}

plugin::Plugin * Load::copy() const
{
  Load * out = new Load;
  out->_option = _option;
  return out;
}

String Load::name() const
{
  return quicktime::name;
}

List<String> Load::extension_list() const
{
  return quicktime::extension_list;
}

bool Load::is_seq() const
{
  return false;
}

//------------------------------------------------------------------------------
// Load::option(), Load::option_list(), Load::option_value(),
// Load::option_default()
//------------------------------------------------------------------------------

bool Load::option(const String & in, String * data)
{
  const List<String> & list = option_list();
  try
  {
    if (string::compare_no_case(in, list[LOAD_START_FRAME]))
      *data >> _option.start_frame;
  }
  catch (String)
  {
    return false;
  }
  
  return true;
}

String Load::option(const String & in) const
{
  const List<String> & list = option_list();
  
  String out;
  
  if (string::compare_no_case(in, list[LOAD_START_FRAME]))
    out << _option.start_frame;
  
  return out;
}

List<String> Load::option_list() const
{
  return label_load_option();
}

List<String> Load::option_value(const String & in) const
{
  List<String> out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_load_option(), &index))
    switch (index)
    {
      case LOAD_START_FRAME: break;
    }
  
  return out;
}

String Load::option_default(const String & in) const
{
  String out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_load_option(), &index))
    switch (index)
    {
      case LOAD_START_FRAME: out << _option.start_frame; break;
    }
  
  return out;
}

//------------------------------------------------------------------------------
// Load::open(), Load::_close()
//------------------------------------------------------------------------------

namespace {

const qt::OSType media_list [] = { qt::VisualMediaCharacteristic };
const int media_list_size = 1;

/*long _frame_count(qt::Movie in)
{
  long out = 0;
  
  for (qt::TimeValue time = 0; time >= 0; ++out)
  {
    qt::TimeValue tmp = 0;
    qt::GetMovieNextInterestingTime(in, qt::nextTimeStep, media_list_size,
      media_list, time, qt_fixed1, &tmp, 0);
    time = tmp;
  }
  
  return out;
}*/

qt::TimeValue _frame_duration(qt::Movie in)
{
  qt::TimeValue out = 0;
  
  qt::GetMovieNextInterestingTime(in, qt::nextTimeStep, media_list_size,
    media_list, out, qt_fixed1, 0, &out);
  
  return out;
}

}

void Load::open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::open");
  DJV_DEBUG_PRINT("in = " << in);

  _close();

  // Open.

  CFStringRef cf_string_ref(file::path_absolute(in));

  qt::QTNewMoviePropertyElement prop_list [10];
  qt::ItemCount prop_size = 0;

  prop_list[prop_size].propClass = qt::kQTPropertyClass_DataLocation;
  prop_list[prop_size].propID =
    qt::kQTDataLocationPropertyID_CFStringNativePath;
    //qt::kQTDataLocationPropertyID_CFStringPosixPath;
  prop_list[prop_size].propValueSize = sizeof(cf_string_ref.p);
  prop_list[prop_size].propValueAddress = (void *)&cf_string_ref.p;
  prop_list[prop_size].propStatus = 0;
  ++prop_size;

  qt::Boolean unresolved = true;
  prop_list[prop_size].propClass = qt::kQTPropertyClass_MovieInstantiation;
  prop_list[prop_size].propID =
    qt::kQTMovieInstantiationPropertyID_DontAskUnresolvedDataRefs;
  prop_list[prop_size].propValueSize = sizeof(unresolved);
  prop_list[prop_size].propValueAddress = &unresolved;
  prop_list[prop_size].propStatus = 0;
  ++prop_size;

  qt::Boolean active = true;
  prop_list[prop_size].propClass = qt::kQTPropertyClass_NewMovieProperty;
  prop_list[prop_size].propID = qt::kQTNewMoviePropertyID_Active;
  prop_list[prop_size].propValueSize = sizeof(active);
  prop_list[prop_size].propValueAddress = &active;
  prop_list[prop_size].propStatus = 0;
  ++prop_size;

  qt::Boolean no_interact = true;
  prop_list[prop_size].propClass = qt::kQTPropertyClass_NewMovieProperty;
  prop_list[prop_size].propID = qt::kQTNewMoviePropertyID_DontInteractWithUser;
  prop_list[prop_size].propValueSize = sizeof(no_interact);
  prop_list[prop_size].propValueAddress = &no_interact;
  prop_list[prop_size].propStatus = 0;
  ++prop_size;

  qt::QTVisualContextRef visual = 0;
  prop_list[prop_size].propClass = qt::kQTPropertyClass_Context;
  prop_list[prop_size].propID = qt::kQTContextPropertyID_VisualContext;
  prop_list[prop_size].propValueSize = sizeof(qt::QTVisualContextRef);
  prop_list[prop_size].propValueAddress = &visual;
  prop_list[prop_size].propStatus = 0;
  ++prop_size;

  qt::QTAudioContextRef audio = 0;
  prop_list[prop_size].propClass = qt::kQTPropertyClass_Context;
  prop_list[prop_size].propID = qt::kQTContextPropertyID_AudioContext;
  prop_list[prop_size].propValueSize = sizeof(qt::QTAudioContextRef);
  prop_list[prop_size].propValueAddress = &audio;
  prop_list[prop_size].propStatus = 0;
  ++prop_size;

  qt::OSErr err = qt::NewMovieFromProperties(
    prop_size,
    prop_list,
    0,
    0,
    &_movie
  );
  if (err != qt::noErr)
    io::throw_error(name(), string::Format(error_open).arg(in).arg(err));
  
  // Information.
  
  _info.file_name = in;
  
  qt::Rect rect;
  qt::GetMovieBox(_movie, &rect);
  _info.size = V2i(rect.right - rect.left, rect.bottom - rect.top);
  _info.mirror.y = true;
  _info.pixel = pixel::RGBA_U8;
  //_info.bgr = true;
 
  const qt::TimeScale time_scale = qt::GetMovieTimeScale(_movie);
  const qt::TimeValue time_duration = qt::GetMovieDuration(_movie);
  //const long frame_count = _frame_count(_movie);
  const qt::TimeValue frame_duration = _frame_duration(_movie);
  //const TimeValue frame_duration = time_duration / frame_count;
  _start_frame = _option.start_frame;
  
  DJV_DEBUG_PRINT("time scale = " << static_cast<int>(time_scale));
  DJV_DEBUG_PRINT("time duration = " << static_cast<int>(time_duration));
  //DJV_DEBUG_PRINT("frame count = " << static_cast<int>(frame_count));
  DJV_DEBUG_PRINT("frame duration = " << static_cast<int>(frame_duration));
  DJV_DEBUG_PRINT("start frame = " << _start_frame);

  _info.time = djv_image::Time(
    _start_frame,
    frame_duration ?
      (_start_frame + (time_duration / frame_duration - 1)) :
      _start_frame,
    djv_image::time::Speed(time_scale, frame_duration)
  );
  
  DJV_DEBUG_PRINT("time = " << _info.time);
  
  info = _info;
  
  // Temporary buffers.
  
  _image.set(_info);
  
  // XXX

  unsigned long qt_format =
#if defined(DJV_MACOSX)
    qt::k32ARGBPixelFormat;
#else
    qt::k32RGBAPixelFormat;
#endif

  err = qt::NewGWorldFromPtr(
    &_gworld,
    qt_format,
    &rect,
    0,
    0,
    0,
    (char *)_image.data(0, 0),
    _info.size.x * 4
  );
  if (err != qt::noErr)
    io::throw_error(name(), string::Format(error_gworld).arg(in).arg(err));

  qt::SetMovieGWorld(_movie, _gworld, 0);
}

void Load::_close()
{
  DJV_DEBUG("Load::_close");
  
  if (_movie)
  {
    qt::DisposeMovie(_movie);
    _movie = 0;
  }
  
  if (_gworld)
  {
    qt::DisposeGWorld(_gworld);
    _gworld = 0;
  }
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

const Image * Load::get(const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Load::get");
  DJV_DEBUG_PRINT("frame = " << frame);

  int _frame = frame.frame;
  if (-1 == _frame)
    _frame = 0;
  else
    _frame -= _start_frame;

  // Read.
  
  const qt::TimeValue time = _frame * _info.time.speed.duration;
  DJV_DEBUG_PRINT("time = " << static_cast<int>(time));
  qt::SetMovieTimeValue(_movie, time);
  qt::MoviesTask(_movie, 0);
  qt::UpdateMovie(_movie);
  
  Info info = _info;
  if (frame.proxy)
  {
    info.size = data::proxy_scale(info.size, frame.proxy);
    info.proxy = frame.proxy;
    _tmp.set(info);
    
    data::proxy_scale(_image, &_tmp, frame.proxy);
  }
 
  // XXX ARGB -> RGB

#if defined(DJV_MACOSX)

  uint32_t * p = (uint32_t *)(frame.proxy ? _tmp.data() : _image.data());
  const uint32_t * const end = p + info.size.x * info.size.y;
  
  switch (memory::endian())
  {
    case memory::MSB:
      for (; p < end; ++p)
        *p = (*p) << 8;
      break;
    
    case memory::LSB:
      for (; p < end; ++p)
        *p = (*p) >> 8;
      break;
  }
#endif

  DJV_DEBUG_PRINT("image = " << _image);
  
  return frame.proxy ? &_tmp : &_image;
}

//------------------------------------------------------------------------------

}
}

