/*------------------------------------------------------------------------------
 include/djv_image/data_inline.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_image {
namespace data {

//------------------------------------------------------------------------------
// Data
//------------------------------------------------------------------------------

inline const Info & Data::info() const
{
  return _info;
}

inline const V2i & Data::size() const
{
  return _info.size;
}

inline PROXY Data::proxy() const
{
  return _info.proxy;
}

inline int Data::w() const
{
  return _info.size.x;
}

inline int Data::h() const
{
  return _info.size.y;
}

inline PIXEL Data::pixel() const
{
  return _info.pixel;
}

inline int Data::channels() const
{
  return _channels;
}
  
inline uint8_t * Data::data()
{
  return _data();
}

inline const uint8_t * Data::data() const
{
  return _p;
}

inline uint8_t * Data::data(int x, int y)
{
  return _data() + (y * _info.size.x + x) * _bytes_pixel;
}

inline const uint8_t * Data::data(int x, int y) const
{
  return _p + (y * _info.size.x + x) * _bytes_pixel;
}

inline size_t Data::bytes_pixel() const
{
  return _bytes_pixel;
}

inline size_t Data::bytes_scanline() const
{
  return _bytes_scanline;
}

inline size_t Data::bytes_data() const
{
  return _bytes_data;
}

//------------------------------------------------------------------------------

}
}

