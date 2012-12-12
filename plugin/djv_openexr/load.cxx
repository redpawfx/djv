/*------------------------------------------------------------------------------
 plugin/djv_openexr/load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "load.h"

#include <djv_image/gl_data.h>
#include <ImfChannelList.h>
#include <ImfHeader.h>
#include <ImfInputFile.h>
#include <ImfRgbaYca.h>
#include <algorithm>

namespace djv_plugin {
namespace openexr {

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
  _f(0)
{}

Load::~Load()
{
  _close();
}

void Load::init() throw (Error)
{
  openexr_init();
}

void Load::del()
{
  openexr_del();
}

plugin::Plugin * Load::copy() const
{
  Load * out = new Load;
  out->_option = _option;
  return out;
}

String Load::name() const
{
  return openexr::name;
}

List<String> Load::extension_list() const
{
  return openexr::extension_list;
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
    if (string::compare_no_case(in, list[LOAD_COLOR_PROFILE]))
      *data >> _option.color_profile;
    else if (string::compare_no_case(in, list[LOAD_GAMMA]))
      *data >> _option.gamma;
    else if (string::compare_no_case(in, list[LOAD_EXPOSURE]))
      *data >> _option.exposure;
    else if (string::compare_no_case(in, list[LOAD_CHANNELS]))
      *data >> _option.channels;
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
  
  if (string::compare_no_case(in, list[LOAD_COLOR_PROFILE]))
    out << _option.color_profile;
  else if (string::compare_no_case(in, list[LOAD_GAMMA]))
    out << _option.gamma;
  else if (string::compare_no_case(in, list[LOAD_EXPOSURE]))
    out << _option.exposure;
  else if (string::compare_no_case(in, list[LOAD_CHANNELS]))
    out << _option.channels;
  
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
      case LOAD_COLOR_PROFILE: out = label_color_profile(); break;
      case LOAD_GAMMA: out += label_gamma_value; break;
      case LOAD_EXPOSURE: out += label_exposure_value; break;
      case LOAD_CHANNELS: out = label_channels(); break;
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
      case LOAD_COLOR_PROFILE: out << _option.color_profile; break;
      case LOAD_GAMMA: out << _option.gamma; break;
      case LOAD_EXPOSURE: out << _option.exposure; break;
      case LOAD_CHANNELS: out << _option.channels; break;
    }
  
  return out;
}

//------------------------------------------------------------------------------
// Load::open(), Load::_open(), Load::_close()
//------------------------------------------------------------------------------

void Load::open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::open");
  DJV_DEBUG_PRINT("in = " << in);

  _file = in;
  
  _open(_file.get(_file.seq().start()), info);
  
  if (file::SEQ == _file.type())
    info.time.list = _file.seq().list;
}

void Load::_open(const String & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::_open");
  DJV_DEBUG_PRINT("in = " << in);
  
  _close();

  try
  {  
    _f = new Imf::InputFile(in.c_str());
    
    // Information.
    
    _display_window = imf_to_box(_f->header().displayWindow());
    _data_window = imf_to_box(_f->header().dataWindow());
    
    DJV_DEBUG_PRINT("display window = " << _display_window);
    DJV_DEBUG_PRINT("data window = " << _data_window);
    
    Info _info;
    _info.size = _display_window.size;
    _info.mirror.y = true;
    
    _layer_list = layer(_f->header().channels(), _option.channels);
    info.layer_size(_layer_list.size());
    
    for (list::size_type i = 0; i < _layer_list.size(); ++i)
    {
      const V2i sampling(
        _layer_list[i].channel[0].sampling.x,
        _layer_list[i].channel[0].sampling.y
      );
      
      Info tmp = _info;
      tmp.file_name = in;
      tmp.layer_name = _layer_list[i].name;
      tmp.size /= sampling;
      
      if (
        ! pixel::pixel(
          pixel::format(static_cast<int>(_layer_list[i].channel.size())),
          _layer_list[i].channel[0].type,
          &tmp.pixel
        )
      )
        djv_image::io::throw_error_unsupported(name(), in);
  
      info[i] = tmp;
    }
    
    for (list::size_type i = 0; i < info.layer_size(); ++i)
      DJV_DEBUG_PRINT("info[" << i << "] = " << info[i]);
  
    // Image tags.
    
    tag_load(_f->header(), info);
  }
  catch (const std::exception & error)
  {
    throw Error(name(), error.what());
  }
}

void Load::_close()
{
  delete _f; _f = 0;
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

const Image * Load::get(const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Load::get");
  DJV_DEBUG_PRINT("frame = " << frame);

  try
  {  
    // Open.

    const String file_name =
      _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());
    
    DJV_DEBUG_PRINT("file name = " << file_name);
  
    io::Info info;
    _open(file_name, info);
    
    if (
      frame.layer < 0 ||
      frame.layer >= static_cast<int>(_layer_list.size())
    )
      djv_image::io::throw_error_read(name(), file_name);

    Info _info = info[frame.layer];
    _info.size = _data_window.size;

    Color_Profile color_profile;
    if (_option.color_profile != COLOR_PROFILE_NONE)
    {
      DJV_DEBUG_PRINT("color profile");
      
      switch (_option.color_profile)
      {
        case COLOR_PROFILE_GAMMA:
          color_profile.type = color_profile::GAMMA;
          break;
        
        case COLOR_PROFILE_EXPOSURE:
          color_profile.type = color_profile::EXPOSURE;
          break;
        
        default: break;
      }
      
      color_profile.gamma = _option.gamma;
      color_profile.exposure = _option.exposure;
    }

    _buffer.tag(info.tag);
    _buffer.color_profile(color_profile);

    const Frame_Cache frame_cache(file_name, frame);
    if (frame_cache == _frame_cache)
    {
      DJV_DEBUG_PRINT("cached");
      
      return _buffer();
    }
    _frame_cache = frame_cache;
  
    // Read.

    _buffer()->set(_info);
  
    const bool flip = Imf::DECREASING_Y == _f->header().lineOrder();
    
    DJV_DEBUG_PRINT("flip = " << flip);
    
    const int channels = pixel::channels(_info.pixel);
    const int bytes = pixel::channel_bytes(_info.pixel);
    const V2i & sampling = _layer_list[frame.layer].channel[0].sampling;
    Imf::FrameBuffer frame_buffer;
    
    for (int c = 0; c < channels; ++c)
    {
      const String & channel = _layer_list[frame.layer].channel[c].name;
      
      DJV_DEBUG_PRINT("channel = " << channel);
      
      frame_buffer.insert(
        channel.c_str(),
        Imf::Slice(
          pixel_type_to_imf(pixel::type(_buffer()->pixel())),
          (char *)_buffer()->data() -
            (_data_window.y * _info.size.x * channels * bytes) -
            (_data_window.x * channels * bytes) +
            c * bytes,
          channels * bytes,
          _info.size.x * channels * bytes,
          sampling.x,
          sampling.y,
          0.0
        )
      );
    }
    
    _f->setFrameBuffer(frame_buffer);
  
    if (flip)
    {
      for (
        int y = 0;
        y < _info.size.y * sampling.y;
        y += sampling.y
      )
        _f->readPixels(
          _data_window.y + (_info.size.y * sampling.y - 1 - y)
        );
    }
    else
    {
      _f->readPixels(
        _data_window.y,
        _data_window.y + _info.size.y * sampling.y - 1
      );
    }
    
    if (_display_window != _data_window)
    {
      DJV_DEBUG_PRINT("display window");
      
      // XXX Is there a more efficient way of reading the file's display/data
      // window so we don't need this extra copy?

      _info.size = _display_window.size;
      _buffer.out()->set(_info);
      
      gl_data::Option option;
      option.xform.position = _data_window.position - _display_window.position;
      gl_data::copy(_buffer.in(), _buffer.out(), option);
      _buffer.swap();
    }
  
    if (frame.proxy)
    {
      DJV_DEBUG_PRINT("proxy");
      
      _info.size = data::proxy_scale(_info.size, frame.proxy);
      _info.proxy = frame.proxy;
      _buffer.out()->set(_info);
      
      data::proxy_scale(_buffer.in(), _buffer.out(), frame.proxy);
      _buffer.swap();
    }
  }
  catch (const std::exception & error)
  {
    throw Error(name(), error.what());
  }

  DJV_DEBUG_PRINT("image = " << *_buffer());
  
  return _buffer();
}

//------------------------------------------------------------------------------

}
}

