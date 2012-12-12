/*------------------------------------------------------------------------------
 plugin/djv_tiff/save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "save.h"

#include <djv_image/gl_data.h>

namespace djv_plugin {
namespace tiff {

//------------------------------------------------------------------------------
// djv_image_save()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_save()
{
  tiff_init();

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

plugin::Plugin * Save::copy() const
{
  Save * out = new Save;
  out->_option = _option;
  return out;
}

String Save::name() const
{
  return tiff::name;
}

List<String> Save::extension_list() const
{
  return tiff::extension_list;
}

//------------------------------------------------------------------------------
// Save::option(), Save::option_list(), Save::option_value(),
// Save::option_default()
//------------------------------------------------------------------------------

bool Save::option(const String & in, String * data)
{
  try
  {
    if (string::compare_no_case(in, option_list()[SAVE_COMPRESSION]))
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
  String out;
  
  if (string::compare_no_case(in, option_list()[SAVE_COMPRESSION]))
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
    switch (index)
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
    switch (index)
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
  
  _file = in;
  if (info.time.list.size() > 1)
    _file.type(file::SEQ);
  
  _info = Info();
  _info.size = info.size;
  _info.mirror.y = true;
  
  pixel::TYPE type = pixel::type(info.pixel);
  switch (type)
  {
    case pixel::U10: type = pixel::U16; break;
    case pixel::F16: type = pixel::F32; break;
    default: break;
  }
  _info.pixel = pixel::pixel(pixel::format(info.pixel), type);
  
  DJV_DEBUG_PRINT("info = " << _info);
  
  _image.set(_info);
}

void Save::_open(const String & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);

  _close();
  
  // Open.
  
  _f = TIFFOpen(in.c_str(), "w");
  if (! _f)
    djv_image::io::throw_error_open(name(), in);
  
  // Header.
  
  uint16 photometric = 0, samples = 0, sample_depth = 0, sample_format = 0;
  uint16 extra_samples [] = { EXTRASAMPLE_ASSOCALPHA };
  uint16 extra_samples_size = 0;
  uint16 compression = 0;
  
  switch (pixel::format(_image.pixel()))
  {
    case pixel::L:
      photometric = PHOTOMETRIC_MINISBLACK;
      samples = 1;
      break;
    
    case pixel::LA:
      photometric = PHOTOMETRIC_MINISBLACK;
      samples = 2;
      extra_samples_size = 1;
      break;
    
    case pixel::RGB:
      photometric = PHOTOMETRIC_RGB;
      samples = 3;
      break;
    
    case pixel::RGBA:
      photometric = PHOTOMETRIC_RGB;
      samples = 4;
      extra_samples_size = 1;
      break;
    
    default: break;
  }
  
  switch (pixel::type(_image.pixel()))
  {
    case pixel::U8:
      sample_depth = 8;
      sample_format = SAMPLEFORMAT_UINT;
      break;
    
    case pixel::U16:
      sample_depth = 16;
      sample_format = SAMPLEFORMAT_UINT;
      break;
    
    case pixel::F16:
    case pixel::F32:
      sample_depth = 32;
      sample_format = SAMPLEFORMAT_IEEEFP;
      break;
    
    default: break;
  }
  
  switch (_option.compression)
  {
    case _COMPRESSION_NONE: compression = COMPRESSION_NONE; break;
    case _COMPRESSION_RLE: compression = COMPRESSION_PACKBITS; break;
    case _COMPRESSION_LZW: compression = COMPRESSION_LZW; break;
  }
  
  TIFFSetField(_f, TIFFTAG_IMAGEWIDTH, _image.w());
  TIFFSetField(_f, TIFFTAG_IMAGELENGTH, _image.h());
  TIFFSetField(_f, TIFFTAG_PHOTOMETRIC, photometric);
  TIFFSetField(_f, TIFFTAG_SAMPLESPERPIXEL, samples);
  TIFFSetField(_f, TIFFTAG_BITSPERSAMPLE, sample_depth);
  TIFFSetField(_f, TIFFTAG_SAMPLEFORMAT, sample_format);
  TIFFSetField(_f, TIFFTAG_EXTRASAMPLES, extra_samples_size, extra_samples);
  TIFFSetField(_f, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
  TIFFSetField(_f, TIFFTAG_COMPRESSION, compression);
  TIFFSetField(_f, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  
  // Image tags.
  
  const List<String> & label_tag = tag::label_tag();
  String tag = info.tag[label_tag[tag::CREATOR]];
  if (tag.size())
    TIFFSetField(_f, TIFFTAG_ARTIST, tag.c_str());
  tag = info.tag[label_tag[tag::COPYRIGHT]];
  if (tag.size())
    TIFFSetField(_f, TIFFTAG_COPYRIGHT, tag.c_str());
  tag = info.tag[label_tag[tag::TIME]];
  if (tag.size())
    TIFFSetField(_f, TIFFTAG_DATETIME, tag.c_str());
  tag = info.tag[label_tag[tag::DESCRIPTION]];
  if (tag.size())
    TIFFSetField(_f, TIFFTAG_IMAGEDESCRIPTION, tag.c_str());
}

void Save::_close()
{
  if (_f)
  {
    TIFFClose(_f);
    _f = 0;
  }
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);

  // Open.
  
  const String file_name = _file.get(frame.frame);
  
  DJV_DEBUG_PRINT("file name = " << file_name);
  
  io::Info info(_info);
  info.tag = in.tag;
  
  _open(file_name, info);
  
  // Write.
  
  const Data * p = &in;
  
  if (in.info() != _info)
  {
    DJV_DEBUG_PRINT("convert = " << _image);
    
    _image.zero();
    
    gl_data::copy(in, &_image);
    
    p = &_image;
  }
  
  const int h = p->h();
  for (int y = 0; y < h; ++y)
    if (TIFFWriteScanline(_f, (tdata_t *)p->data(0, y), y) == -1)
      djv_image::io::throw_error_write(name(), file_name);

  _close();
}

//------------------------------------------------------------------------------

}
}

