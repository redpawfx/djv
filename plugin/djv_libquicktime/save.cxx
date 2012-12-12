/*------------------------------------------------------------------------------
 plugin/djv_libquicktime/save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "save.h"

#include <djv_image/gl_data.h>

namespace djv_plugin {
namespace libquicktime {

//------------------------------------------------------------------------------
// djv_image_save()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_save()
{
  quicktime_init();
  
  return new Save;
}

}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save() :
  _f(0),
  _frame(0),
  _frame_duration(0)
{}

plugin::Plugin * Save::copy() const
{
  Save * out = new Save;
  out->_option = _option;
  return out;
}

String Save::name() const
{
  return libquicktime::name;
}

List<String> Save::extension_list() const
{
  return libquicktime::extension_list;
}

bool Save::is_seq() const
{
  return false;
}

//------------------------------------------------------------------------------
// Save::option(), Save::option_list(), Save::option_value(),
// Save::option_default()
//------------------------------------------------------------------------------

bool Save::option(const String & in, String * data)
{
  const List<String> & list = option_list();
  
  try
  {
    if (string::compare_no_case(in, list[SAVE_CODEC]))
      *data >> _option.codec;
  }
  catch (String)
  {
    return false;
  }
  
  return true;
}

String Save::option(const String & in) const
{
  const List<String> & list = option_list();
  
  String out;
  
  if (string::compare_no_case(in, list[SAVE_CODEC]))
    out << _option.codec;
  
  return out;
}

List<String> Save::option_list() const
{
  return label_save_option();
}

List<String> Save::option_value(const String & in) const
{
  List<String> out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_save_option(), &index))
    switch (index)
    {
      case SAVE_CODEC: out = label_codec(); break;
    }
  
  return out;
}

String Save::option_default(const String & in) const
{
  String out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_save_option(), &index))
    switch (index)
    {
      case SAVE_CODEC: out << _option.codec; break;
    }
  
  return out;
}

//------------------------------------------------------------------------------
// Save::open(), Save::close()
//------------------------------------------------------------------------------

namespace {

struct Codec_Info
{
  Codec_Info(const String & name) :
    p(lqt_find_video_codec_by_name(name.c_str()))
  {}

  ~Codec_Info()
  {
    if (p)
      lqt_destroy_codec_info(p);
  }
    
  lqt_codec_info_t ** p;
};

}

void Save::open(const File & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::open");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("info = " << info);
  
  // Open.
  
  _f = lqt_open_write(in.get().c_str(), LQT_FILE_QT);
  if (! _f)
    djv_image::io::throw_error_open(name(), in);
  
  // Information.
  
  Codec_Info codec_info(_option.codec);
  if (! codec_info.p)
    io::throw_error(name(), string::Format(error_codec).arg(in));
  
  _frame = 0;
  _frame_duration = info.time.speed.duration;
  
  // XXX Return value?

  lqt_set_video(
    _f,
    1,
    info.size.x,
    info.size.y,
    _frame_duration,
    info.time.speed.scale,
    codec_info.p[0]
  );
  
  _info = Info();
  _info.file_name = in;
  _info.size = info.size;
  _info.pixel = info.pixel;
  
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
    case BC_BGR888:
      _info.bgr = true;
    case BC_RGB888:
      _info.pixel = pixel::RGB_U8;
      break;
    
    case BC_RGB161616:
      _info.pixel = pixel::RGB_U16;
      break;
    
    case BC_RGBA8888:
      _info.pixel = pixel::RGBA_U8;
      break;
    
    case BC_RGBA16161616:
      _info.pixel = pixel::RGBA_U16;
      break;
    
    default:
      io::throw_error(name(), string::Format(error_colormodel).arg(in));
  }
  
  lqt_set_cmodel(_f, 0, cmodel);
  
  DJV_DEBUG_PRINT("info = " << _info);
  
  const List<String> & label_tag = tag::label_tag();
  String tag;
  
  tag = info.tag[label_tag[tag::CREATOR]];
  if (tag.size())
    lqt_set_artist(_f, (char *)tag.c_str());
  
  tag = info.tag[label_tag[tag::DESCRIPTION]];
  if (tag.size())
    lqt_set_comment(_f, (char *)tag.c_str());
  
  tag = info.tag[label_tag[tag::COPYRIGHT]];
  if (tag.size())
    quicktime_set_copyright(_f, (char *)tag.c_str());

  _image.set(_info);
}

void Save::close() throw (Error)
{
  DJV_DEBUG("Save::close");

  if (_f)
  {
    quicktime_close(_f);
    _f = 0;
  }
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

namespace {

// Convert our image data to an array of row pointers for libquicktime.

struct Rows
{
  Rows(const Data * in) :
    p(new const unsigned char * [in->h() + 1])
  {
    for (int y = 0; y < in->h(); ++y)
      p[y] = in->data(0, in->h() - 1 - y);
  }
    
  ~Rows()
  {
    delete [] p;
  }
  
  const unsigned char ** p;
};

}

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);

  // Convert the image if necessary.
  
  const Data * p = &in;
  
  if (in.info() != _info)
  {
    DJV_DEBUG_PRINT("convert = " << _image);
    
    _image.zero();
    
    gl_data::copy(in, &_image);
    
    p = &_image;
  }

  // Write the image.
  
  lqt_encode_video(
    _f,
    const_cast<unsigned char **>(Rows(p).p),
    0,
    _frame * _frame_duration
  );
  
  ++_frame;
}

//------------------------------------------------------------------------------

}
}

