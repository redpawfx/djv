/*------------------------------------------------------------------------------
 plugin/djv_openexr/save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "save.h"

#include <djv_image/gl_data.h>
#include <ImfChannelList.h>
#include <ImfCompressionAttribute.h>
#include <ImfHeader.h>
#include <ImfOutputFile.h>

namespace djv_plugin {
namespace openexr {

//------------------------------------------------------------------------------
// djv_image_save()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_save()
{
  return new Save;
}

}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save() :
  _f(0)
{}

Save::~Save()
{
  _close();
}

void Save::init() throw (Error)
{
  openexr_init();
}

void Save::del()
{
  openexr_del();
}

djv_base::plugin::Plugin * Save::copy() const
{
  Save * out = new Save;
  out->_option = _option;
  return out;
}

String Save::name() const
{
  return openexr::name;
}

List<String> Save::extension_list() const
{
  return openexr::extension_list;
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
    if (string::compare_no_case(in, list[SAVE_COMPRESSION]))
      *data >> _option.compression;
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
  
  if (string::compare_no_case(in, list[SAVE_COMPRESSION]))
    out << _option.compression;
  
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
    switch (static_cast<SAVE_OPTION>(index))
    {
      case SAVE_COMPRESSION: out = label_compression(); break;
    }
  
  return out;
}

String Save::option_default(const String & in) const
{
  String out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_save_option(), &index))
    switch (static_cast<SAVE_OPTION>(index))
    {
      case SAVE_COMPRESSION: out << _option.compression; break;
    }
  
  return out;
}

//------------------------------------------------------------------------------
// Save::open(), Save::_open(), Save::_close()
//------------------------------------------------------------------------------

void Save::open(const File & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::open");
  DJV_DEBUG_PRINT("in = " << in);
  
  // File information.
  
  _file = in;
  
  if (info.time.list.size() > 1)
    _file.type(file::SEQ);
  
  // Image information.
  
  _info = Info();
  _info.size = info.size;
  _info.mirror.y = true;
  
  pixel::FORMAT format = pixel::format(info.pixel);
  pixel::TYPE type = pixel::TYPE(0);
  switch (pixel::type(info.pixel))
  {
    case pixel::F32:
      type = pixel::F32;
      break;

    default:
      type = pixel::F16;
      break;
  }
  
  _info.pixel = pixel::pixel(format, type);
  
  DJV_DEBUG_PRINT("info = " << _info);
  
  _speed = info.time.speed;
  
  // Initialize temporary image buffer.
  
  _tmp.set(_info);
}

void Save::_open(const String & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);

  try
  {
    _close();
    
    // Header.
    
    Imf::Header header(info.size.x, info.size.y);
    
    switch (pixel::channels(info.pixel))
    {
      case 1: _channel_list = List<String>() << "Y"; break;
      case 2: _channel_list = List<String>() << "Y" << "A"; break;
      case 3: _channel_list = List<String>() << "R" << "G" << "B"; break;
      case 4: _channel_list = List<String>() << "R" << "G" << "B" << "A"; break;
    }
    
    for (list::size_type i = 0; i < _channel_list.size(); ++i)
      header.channels().insert(
        _channel_list[i].c_str(),
        pixel_type_to_imf(pixel::type(info.pixel))
      );
    
    Imf::CompressionAttribute compression;
    switch (_option.compression)
    {
      case COMPRESSION_NONE: compression = Imf::NO_COMPRESSION; break;
      case COMPRESSION_RLE: compression = Imf::RLE_COMPRESSION; break;
      case COMPRESSION_ZIPS: compression = Imf::ZIPS_COMPRESSION; break;
      case COMPRESSION_ZIP: compression = Imf::ZIP_COMPRESSION; break;
      case COMPRESSION_PIZ: compression = Imf::PIZ_COMPRESSION; break;
      case COMPRESSION_PXR24: compression = Imf::PXR24_COMPRESSION; break;
      case COMPRESSION_B44: compression = Imf::B44_COMPRESSION; break;
      case COMPRESSION_B44A: compression = Imf::B44A_COMPRESSION; break;
    }
    
    header.insert(Imf::CompressionAttribute::staticTypeName(), compression);
    
    // Image tags.
    
    tag_save(info, header);
  
    // Open.
  
    _f = new Imf::OutputFile(in.c_str(), header);
  
  }
  catch (const std::exception & error)
  {
    throw Error(name(), error.what());
  }
}

void Save::_close()
{
  delete _f; _f = 0;
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  try
  {
    // Open.
    
    io::Info info(_info);
    info.tag = in.tag;
    info.time.speed = _speed;
    
    _open(_file.get(frame.frame), info);
    
    // Convert.
    
    const Data * p = &in;
    
    if (p->info() != _info)
    {
      DJV_DEBUG_PRINT("convert = " << _tmp);
      
      _tmp.zero();
      
      gl_data::copy(in, &_tmp);
      
      p = &_tmp;
    }
    
    // Write.
  
    const int w = p->w(), h = p->h();
    const int channels = p->channels();
    const int bytes = pixel::channel_bytes(p->pixel());
  
    Imf::FrameBuffer frame_buffer;
    
    for (int c = 0; c < channels; ++c)
    {
      const String & channel = _channel_list[c];
      
      DJV_DEBUG_PRINT("channel = " << channel);
      
      frame_buffer.insert(
        channel.c_str(),
        Imf::Slice(
          pixel_type_to_imf(pixel::type(p->pixel())),
          (char *)p->data() + c * bytes,
          channels * bytes,
          w * channels * bytes,
          1,
          1,
          0.0
        )
      );
    }
    
    _f->setFrameBuffer(frame_buffer);
  
    _f->writePixels(h);
  
  }
  catch (const std::exception & err)
  {
    throw Error(name(), err.what());
  }
  
  _close();
}

//------------------------------------------------------------------------------

}
}

