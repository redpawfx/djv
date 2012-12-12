/*------------------------------------------------------------------------------
 include/djv_image/pixel.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_PIXEL_H
#define DJV_IMAGE_PIXEL_H

#include <djv_image/base.h>
#if defined(DJV_OPENEXR)
#include <half.h>
#endif

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::pixel
//
//! Image pixels.
//------------------------------------------------------------------------------

namespace pixel {

//------------------------------------------------------------------------------
//!\name Pixel Components
//------------------------------------------------------------------------------
//@{

//! Pixel format.

enum FORMAT
{
  L,     //!< Luminance
  LA,    //!< Luminance, alpha
  RGB,   //!< Red, green, blue
  RGBA,  //!< Red, green, blue, alpha
  
  _FORMAT_SIZE
};

//! Pixel type.

enum TYPE
{
  U8,   //!< 8-bit unsigned
  U10,  //!< 10-bit unsigned
  U16,  //!< 16-bit unsigned
#if defined(DJV_OPENEXR)
  F16,  //!< 16-bit floating-point
#endif
  F32,  //!< 32-bit floating-point
  
  _TYPE_SIZE
};

typedef uint8_t  U8_T;
typedef uint16_t U10_T;
typedef uint16_t U16_T;
#if defined(DJV_OPENEXR)
typedef half     F16_T;
#endif
typedef float    F32_T;

//! 10-bit MSB data.

struct U10_S_MSB { uint r:10, g:10, b:10, pad:2; };

//! 10-bit LSB data.

struct U10_S_LSB { uint pad:2, b:10, g:10, r:10; };

#if defined(DJV_MSB)
typedef U10_S_MSB U10_S;
#else
typedef U10_S_LSB U10_S;
#endif

static const int channels_max = 4;
static const int bytes_max = 4;

static const int u8_max  = uint8_t(~0);
static const int u10_max = uint16_t(~0) >> 6;
static const int u16_max = uint16_t(~0);

//@}
//------------------------------------------------------------------------------
//!\name Pixels
//------------------------------------------------------------------------------
//@{

//! Pixel.

enum PIXEL
{
  L_U8,
  L_U16,
#if defined(DJV_OPENEXR)
  L_F16,
#endif
  L_F32,
  LA_U8,
  LA_U16,
#if defined(DJV_OPENEXR)
  LA_F16,
#endif
  LA_F32,
  RGB_U8,
  RGB_U10,
  RGB_U16,
#if defined(DJV_OPENEXR)
  RGB_F16,
#endif
  RGB_F32,
  RGBA_U8,
  RGBA_U16,
#if defined(DJV_OPENEXR)
  RGBA_F16,
#endif
  RGBA_F32,
  
  _PIXEL_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

inline FORMAT format(PIXEL);
inline FORMAT format(int channels);
inline TYPE type(PIXEL);

inline int channels(FORMAT);
inline int channels(PIXEL);
inline int channel_bytes(PIXEL);
inline int bytes(PIXEL);
inline int bit_depth(TYPE);
inline int bit_depth(PIXEL);
inline int max(PIXEL);

// XXX Pointer/bool casting confusion?

inline PIXEL pixel(FORMAT, TYPE);
inline PIXEL pixel(PIXEL, TYPE);
inline bool  pixel(FORMAT, TYPE, PIXEL *);
inline bool  pixel_(int channels, int bit_depth, bool floating_point, PIXEL *);
inline int   pixel_(int channels, int bit_depth, bool floating_point);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

inline U10_T u8_to_u10(U8_T);
inline U16_T u8_to_u16(U8_T);
#if defined(DJV_OPENEXR)
inline F16_T u8_to_f16(U8_T);
#endif
inline F32_T u8_to_f32(U8_T);

inline U8_T  u10_to_u8 (U10_T);
inline U16_T u10_to_u16(U10_T);
#if defined(DJV_OPENEXR)
inline F16_T u10_to_f16(U10_T);
#endif
inline F32_T u10_to_f32(U10_T);

inline U8_T  u16_to_u8 (U16_T);
inline U10_T u16_to_u10(U16_T);
#if defined(DJV_OPENEXR)
inline F16_T u16_to_f16(U16_T);
#endif
inline F32_T u16_to_f32(U16_T);

#if defined(DJV_OPENEXR)
inline U8_T  f16_to_u8 (F16_T);
inline U10_T f16_to_u10(F16_T);
inline U16_T f16_to_u16(F16_T);
inline F32_T f16_to_f32(F16_T);
#endif

inline U8_T  f32_to_u8 (F32_T);
inline U10_T f32_to_u10(F32_T);
inline U16_T f32_to_u16(F32_T);
#if defined(DJV_OPENEXR)
inline F16_T f32_to_f16(F32_T);
#endif

#define PIXEL_U8_ZERO  0
#define PIXEL_U10_ZERO 0
#define PIXEL_U16_ZERO 0
#if defined(DJV_OPENEXR)
#define PIXEL_F16_ZERO 0.0
#endif
#define PIXEL_F32_ZERO 0.0

#define PIXEL_U8_ONE   pixel::u8_max
#define PIXEL_U10_ONE  pixel::u10_max
#define PIXEL_U16_ONE  pixel::u16_max
#if defined(DJV_OPENEXR)
#define PIXEL_F16_ONE  1.0
#endif
#define PIXEL_F32_ONE  1.0

#define PIXEL_U8_TO_U8(IN)   (IN)
#define PIXEL_U8_TO_U10(IN)  pixel::u8_to_u10(IN)
#define PIXEL_U8_TO_U16(IN)  pixel::u8_to_u16(IN)
#if defined(DJV_OPENEXR)
#define PIXEL_U8_TO_F16(IN)  pixel::u8_to_f16(IN)
#endif
#define PIXEL_U8_TO_F32(IN)  pixel::u8_to_f32(IN)

#define PIXEL_U10_TO_U8(IN)  pixel::u10_to_u8(IN)
#define PIXEL_U10_TO_U10(IN) (IN)
#define PIXEL_U10_TO_U16(IN) pixel::u10_to_u16(IN)
#if defined(DJV_OPENEXR)
#define PIXEL_U10_TO_F16(IN) pixel::u10_to_f16(IN)
#endif
#define PIXEL_U10_TO_F32(IN) pixel::u10_to_f32(IN)

#define PIXEL_U16_TO_U8(IN)  pixel::u16_to_u8(IN)
#define PIXEL_U16_TO_U10(IN) pixel::u16_to_u10(IN)
#define PIXEL_U16_TO_U16(IN) (IN)
#if defined(DJV_OPENEXR)
#define PIXEL_U16_TO_F16(IN) pixel::u16_to_f16(IN)
#endif
#define PIXEL_U16_TO_F32(IN) pixel::u16_to_f32(IN)

#if defined(DJV_OPENEXR)
#define PIXEL_F16_TO_U8(IN)  pixel::f16_to_u8(IN)
#define PIXEL_F16_TO_U10(IN) pixel::f16_to_u10(IN)
#define PIXEL_F16_TO_U16(IN) pixel::f16_to_u16(IN)
#define PIXEL_F16_TO_F16(IN) (IN)
#define PIXEL_F16_TO_F32(IN) pixel::f16_to_f32(IN)
#endif

#define PIXEL_F32_TO_U8(IN)  pixel::f32_to_u8(IN)
#define PIXEL_F32_TO_U10(IN) pixel::f32_to_u10(IN)
#define PIXEL_F32_TO_U16(IN) pixel::f32_to_u16(IN)
#if defined(DJV_OPENEXR)
#define PIXEL_F32_TO_F16(IN) pixel::f32_to_f16(IN)
#endif
#define PIXEL_F32_TO_F32(IN) (IN)

_DJV_IMAGE void convert(
  const void * in, PIXEL,
  void * out, PIXEL,
  int size = 1,
  int stride = 1,
  bool bgr = false
);

_DJV_IMAGE String & operator >> (String &, FORMAT &) throw (String);
_DJV_IMAGE String & operator >> (String &, TYPE &) throw (String);
_DJV_IMAGE String & operator >> (String &, PIXEL &) throw (String);

_DJV_IMAGE String & operator << (String &, FORMAT);
_DJV_IMAGE String & operator << (String &, TYPE);
_DJV_IMAGE String & operator << (String &, PIXEL);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_format();
_DJV_IMAGE const List<String> & label_type();
_DJV_IMAGE const List<String> & label_pixel();

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, FORMAT);
_DJV_IMAGE Debug & operator << (Debug &, TYPE);
_DJV_IMAGE Debug & operator << (Debug &, PIXEL);

//@}
//------------------------------------------------------------------------------

}

typedef pixel::PIXEL PIXEL;

}

#include "djv_image/pixel_inline.h"
#endif

