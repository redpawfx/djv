/*------------------------------------------------------------------------------
 plugin/djv_tiff/load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "load.h"

namespace djv_plugin {
namespace tiff {

//------------------------------------------------------------------------------
// djv_image_load()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_load()
{
  tiff_init();

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

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return tiff::name;
}

List<String> Load::extension_list() const
{
  return tiff::extension_list;
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
  
  // Open.
  
  _f = TIFFOpen(in.c_str(), "r");
  if (! _f)
    djv_image::io::throw_error_open(name(), in);
  
  // Header.

  uint32 width, height;
  uint16 photometric, samples, sample_depth, sample_format;
  uint16 * extra_samples, extra_samples_size;
  uint16 orient, compression, channels;
  
  TIFFGetFieldDefaulted(_f, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetFieldDefaulted(_f, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetFieldDefaulted(_f, TIFFTAG_PHOTOMETRIC, &photometric);
  TIFFGetFieldDefaulted(_f, TIFFTAG_SAMPLESPERPIXEL, &samples);
  TIFFGetFieldDefaulted(_f, TIFFTAG_BITSPERSAMPLE, &sample_depth);
  TIFFGetFieldDefaulted(_f, TIFFTAG_SAMPLEFORMAT, &sample_format);
  TIFFGetFieldDefaulted(_f, TIFFTAG_EXTRASAMPLES, &extra_samples_size,
    &extra_samples);
  TIFFGetFieldDefaulted(_f, TIFFTAG_ORIENTATION, &orient);
  TIFFGetFieldDefaulted(_f, TIFFTAG_COMPRESSION, &compression);
  TIFFGetFieldDefaulted(_f, TIFFTAG_PLANARCONFIG, &channels);
  TIFFGetFieldDefaulted(_f, TIFFTAG_COLORMAP,
    &_colormap[0], &_colormap[1], &_colormap[2]);
    
  DJV_DEBUG_PRINT("tiff size = " << width << " " << height);
  DJV_DEBUG_PRINT("tiff samples = " << samples);
  DJV_DEBUG_PRINT("tiff sample depth = " << sample_depth);
  DJV_DEBUG_PRINT("tiff channels = " << channels);
  
  // Information.
  
  info.file_name = in;
  
  info.size = V2i(width, height);
  
  if (samples > 1 && PLANARCONFIG_SEPARATE == channels)
    djv_image::io::throw_error_unsupported(name(), in);

  int pixel = -1;
  switch (photometric)
  {
    case PHOTOMETRIC_PALETTE: pixel = pixel::RGB_U8; break;
    case PHOTOMETRIC_MINISWHITE:
    case PHOTOMETRIC_MINISBLACK:
    case PHOTOMETRIC_RGB:
      if (32 == sample_depth && sample_format != SAMPLEFORMAT_IEEEFP)
        break;
      pixel = pixel::pixel_(samples, sample_depth, true);
      break;
  }

  if (-1 == pixel)
    djv_image::io::throw_error_unsupported(name(), in);

  info.pixel = PIXEL(pixel);

  _compression = compression != COMPRESSION_NONE;
  _palette = PHOTOMETRIC_PALETTE == photometric;
  
  switch (orient)
  {
    case ORIENTATION_TOPLEFT: info.mirror.y = true; break;
    case ORIENTATION_TOPRIGHT: info.mirror.x = info.mirror.y = true; break;
    case ORIENTATION_BOTRIGHT: info.mirror.x = true; break;
    case ORIENTATION_BOTLEFT: break;
  }
  
  // Image tags.
  
  const List<String> & label_tag = tag::label_tag();
  char * tag = 0;
  if (TIFFGetField(_f, TIFFTAG_ARTIST, &tag))
    info.tag[label_tag[tag::CREATOR]] = tag;
  if (TIFFGetField(_f, TIFFTAG_IMAGEDESCRIPTION, &tag))
    info.tag[label_tag[tag::DESCRIPTION]] = tag;
  if (TIFFGetField(_f, TIFFTAG_COPYRIGHT, &tag))
    info.tag[label_tag[tag::COPYRIGHT]] = tag;
  if (TIFFGetField(_f, TIFFTAG_DATETIME, &tag))
    info.tag[label_tag[tag::TIME]] = tag;
}

void Load::_close()
{
  if (_f)
  {
    TIFFClose(_f);
    _f = 0;
  }
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

const Image * Load::get(const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Load::get");
  DJV_DEBUG_PRINT("frame = " << frame);
  
  // Open.
  
  const String file_name =
    _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());
  
  DJV_DEBUG_PRINT("file name = " << file_name);
  
  io::Info info;
  _open(file_name, info);
  _image.tag = info.tag;
  
  // Read.
  
  Data * data = frame.proxy ? &_tmp : &_image;
  data->set(info);
  
  for (int y = 0; y < info.size.y; ++y)
  {
    if (TIFFReadScanline(_f, (tdata_t *)data->data(0, y), y) == -1)
      djv_image::io::throw_error_read(name(), file_name);

    if (_palette)
      palette_load(
        data->data(0, y),
        info.size.x,
        pixel::channel_bytes(info.pixel),
        _colormap[0], _colormap[1], _colormap[2]
      );
  }

  if (frame.proxy)
  {
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

