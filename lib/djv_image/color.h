/*------------------------------------------------------------------------------
 include/djv_image/color.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_COLOR_H
#define DJV_IMAGE_COLOR_H

#include <djv_image/color_def.h>

#include <djv_image/pixel.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::color
//
//! Colors.
//------------------------------------------------------------------------------

namespace color {

//------------------------------------------------------------------------------
//!\class Color
//
//! A color.
//------------------------------------------------------------------------------

class _DJV_IMAGE Color
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Color();
  Color(const Color &);
  Color(PIXEL);
  Color(pixel::F32_T l);
  Color(pixel::F32_T l, pixel::F32_T a);
  Color(pixel::F32_T r, pixel::F32_T g, pixel::F32_T b);
  Color(pixel::F32_T r, pixel::F32_T g, pixel::F32_T b, pixel::F32_T a);
  
  Color & operator = (const Color &);

  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  void pixel(PIXEL);
  
  inline PIXEL pixel() const;
  
  inline int channels() const;
  
  inline void set_u8(pixel::U8_T, int);
  inline void set_u10(pixel::U10_T, int);
  inline void set_u16(pixel::U16_T, int);
#if defined(DJV_OPENEXR)
  inline void set_f16(pixel::F16_T, int);
#endif
  inline void set_f32(pixel::F32_T, int);
  
  inline pixel::U8_T get_u8(int) const;
  inline pixel::U10_T get_u10(int) const;
  inline pixel::U16_T get_u16(int) const;
#if defined(DJV_OPENEXR)
  inline pixel::F16_T get_f16(int) const;
#endif
  inline pixel::F32_T get_f32(int) const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  void zero();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Data
  //@{

  inline uint8_t * data();
  
  inline const uint8_t * data() const;

  //@}
  
private:

  void init();

  pixel::PIXEL _pixel;
  
  int _channels;
  
  uint8_t _data [pixel::channels_max * pixel::bytes_max];
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE void  scale(double, const Color &, Color &);
_DJV_IMAGE Color scale(double, const Color &);

_DJV_IMAGE void  lerp(double, const Color &, const Color &, Color &);
_DJV_IMAGE Color lerp(double, const Color &, const Color &);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool operator == (const Color &, const Color &);

_DJV_IMAGE bool operator != (const Color &, const Color &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE void convert(const Color &, Color &);

inline void rgb_to_hsv(const double [3], double [3]);

inline void hsv_to_rgb(const double [3], double [3]);

_DJV_IMAGE String & operator >> (String &, Color &) throw (String);

_DJV_IMAGE String & operator << (String &, const Color &);

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, const Color &);

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_image/color_inline.h>
#endif

