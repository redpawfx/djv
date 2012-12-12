/*------------------------------------------------------------------------------
 lib/djv_image/data.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "data.h"

#include <djv_image/color.h>

namespace djv_image {
namespace data {

//------------------------------------------------------------------------------
// Info
//------------------------------------------------------------------------------

void Info::init()
{
  layer_name = "Default";
  proxy = PROXY_NONE;
  pixel = PIXEL(0);
  bgr = false;
  align = 1;
  endian = memory::endian();
}

Info::Info()
{
  init();
}

Info::Info(
  const V2i & size,
  PIXEL pixel
) {
  init();
  
  this->size = size;
  this->pixel = pixel;
}

Info::Info(
  const String & file_name,
  const V2i & size,
  PIXEL pixel
) {
  init();
  
  this->file_name = file_name;
  this->size = size;
  this->pixel = pixel;
}

//------------------------------------------------------------------------------
// Data
//------------------------------------------------------------------------------

void Data::init()
{
  _channels = 0;
  _bytes_pixel = 0;
  _bytes_scanline = 0;
  _bytes_data = 0;
  _p = 0;
}

Data::Data()
{
  DJV_DEBUG("Data::Data");
  
  init();
}

Data::Data(const Data & in)
{
  DJV_DEBUG("Data::Data");

  init();
  
  set(in);
}

Data::Data(const Info & in, const uint8_t * data)
{
  DJV_DEBUG("Data::Data");

  init();
  
  set(in, data);
}

Data::~Data()
{}

Data & Data::operator = (const Data & in)
{
  if (&in != this)
    set(in);
  return *this;
}

void Data::set(const Data & in)
{
  set(in._info);
  memory::copy(in.data(), data(), _bytes_data);
}

void Data::set(const Info & in, const uint8_t * data)
{
  DJV_DEBUG("Data::Data");
  DJV_DEBUG_PRINT("in = " << in);

  _info = in;
  
  _channels = pixel::channels(_info.pixel);
  _bytes_pixel = pixel::bytes(_info.pixel);
  _bytes_scanline = data::bytes_scanline(_info);
  _bytes_data = data::bytes_data(_info);
  
  DJV_DEBUG_PRINT("channels = " << _channels);
  DJV_DEBUG_PRINT("bytes pixel = " << _bytes_pixel);
  DJV_DEBUG_PRINT("bytes scanline = " << _bytes_scanline);
  DJV_DEBUG_PRINT("bytes data = " << _bytes_data);

  if (data)
  {
    _data.size(0);
    _p = data;
  }
  else
  {
    _data.size(_bytes_data);
    _p = _data();
  }
}

void Data::zero()
{ 
  DJV_DEBUG("Data::zero");

  _data.zero();
}

//------------------------------------------------------------------------------

}
}

