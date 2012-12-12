/*------------------------------------------------------------------------------
 plugin/djv_libquicktime/load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "load.h"

namespace djv_plugin {
namespace libquicktime {

//------------------------------------------------------------------------------
// djv_image_load()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_load()
{
  quicktime_init();

  return new Load;
}

}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
  _f(0),
  _start_frame(0),
  _frame(0)
{}

Load::~Load()
{
  _close();
}

plugin::Plugin * Load::copy() const
{
  Load * out = new Load;
  out->_option = _option;
  return out;
}

String Load::name() const
{
  return libquicktime::name;
}

List<String> Load::extension_list() const
{
  return libquicktime::extension_list;
}

bool Load::is_seq() const
{
  return false;
}

//------------------------------------------------------------------------------
// Load::option(), Load::option_list(), Load::option::value(),
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

void Load::open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::open");
  DJV_DEBUG_PRINT("in = " << in);

  _close();
  
  // Open.
  
  _f = lqt_open_read(in.get().c_str());
  if (! _f)
    io::throw_error_open(name(), in);
  
  if (! quicktime_has_video(_f))
    io::throw_error(name(), string::Format(error_video).arg(in));

  if (! quicktime_supported_video(_f, 0))
    io::throw_error_unsupported(name(), in);

  // Information.
  
  _info.file_name = in;
  
  _info.size = V2i(
    quicktime_video_width(_f, 0),
    quicktime_video_height(_f, 0)
  );

  static int cmodel_list [] =
  {
    BC_RGB888,
    BC_BGR888,
    BC_RGB161616,
    BC_RGBA8888,
    BC_RGBA16161616,
    LQT_COLORMODEL_NONE
  };
  const int cmodel = lqt_get_best_colormodel(_f, 0, cmodel_list);
  
  switch (cmodel)
  {
    case BC_BGR888: _info.bgr = true;
    case BC_RGB888: _info.pixel = pixel::RGB_U8; break;
    case BC_RGB161616: _info.pixel = pixel::RGB_U16; break;
    case BC_RGBA8888: _info.pixel = pixel::RGBA_U8; break;
    case BC_RGBA16161616: _info.pixel = pixel::RGBA_U16; break;
    default: io::throw_error_unsupported(name(), in);
  }
  
  lqt_set_cmodel(_f, 0, cmodel);
  
  int time_constant = 0;
  const int time_scale = lqt_video_time_scale(_f, 0);
  const int64_t video_duration = lqt_video_duration(_f, 0);
  const int frame_duration = lqt_frame_duration(_f, 0, &time_constant);
  _start_frame = _option.start_frame;
  
  DJV_DEBUG_PRINT("time scale = " << time_scale);
  DJV_DEBUG_PRINT("video duration = " << video_duration);
  DJV_DEBUG_PRINT("frame duration = " << frame_duration);
  DJV_DEBUG_PRINT("time constant = " << time_constant);
  DJV_DEBUG_PRINT("start frame = " << _start_frame);
  
  _info.time = djv_image::Time(
    _start_frame,
    frame_duration ?
      (_start_frame + (video_duration / frame_duration - 1)) :
      _start_frame,
    djv_image::time::Speed(time_scale, frame_duration)
  );
  
  DJV_DEBUG_PRINT("time = " << _info.time);
  
  // Image tags.
  
  const List<String> & label_tag = tag::label_tag();
  char * tag = 0;
  
  lqt_get_artist(_f);
  if (tag)
    _info.tag[label_tag[tag::CREATOR]] = tag;
  
  tag = lqt_get_comment(_f);
  if (tag)
    _info.tag[label_tag[tag::DESCRIPTION]] = tag;
  
  tag = quicktime_get_copyright(_f);
  if (tag)
    _info.tag[label_tag[tag::COPYRIGHT]] = tag;
  
  // Done.
  
  info = _info;
}

void Load::_close()
{
  DJV_DEBUG("Load::_close");
  
  if (_f)
  {
    quicktime_close(_f);
    _f = 0;
  }
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

namespace {

struct Rows
{
  Rows(Data * in) :
    p(new unsigned char * [in->h() + 1])
  {
    for (int y = 0; y < in->h(); ++y)
      p[y] = reinterpret_cast<pixel::U8_T *>(in->data(0, in->h() - 1 - y));
  }
    
  ~Rows()
  {
    delete [] p;
  }
    
  unsigned char ** p;
};

}

const Image * Load::get(const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Load::get");
  DJV_DEBUG_PRINT("frame = " << frame);
  
  int _frame = frame.frame;
  if (-1 == _frame)
    _frame = 0;
  else
    _frame -= _start_frame;

  Data * data = frame.proxy ? &_tmp : &_image;
  data->set(_info);

  // XXX
  
  if (_frame != this->_frame + 1)
  {
    const uint64_t time = _frame * _info.time.speed.duration;
    
    DJV_DEBUG_PRINT("time = " << static_cast<int>(time));
    
    lqt_seek_video(_f, 0, time);
  }
  this->_frame = _frame;
  
  lqt_decode_video(_f, Rows(data).p, 0);
  
  if (frame.proxy)
  {
    Info info = _info;
    info.size = data::proxy_scale(info.size, frame.proxy);
    info.proxy = frame.proxy;
    _image.set(info);
    
    data::proxy_scale(_tmp, &_image, frame.proxy);
  }
  
  DJV_DEBUG_PRINT("image = " << _image);
  
  return &_image;
}

//------------------------------------------------------------------------------

}
}

