/*------------------------------------------------------------------------------
 include/djv_image/data.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_DATA_H
#define DJV_IMAGE_DATA_H

#include <djv_image/data_def.h>

#include <djv_image/pixel.h>
#include <djv_base/box_def.h>
#include <djv_base/vector.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::data
//
//! Image data.
//------------------------------------------------------------------------------

namespace data {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Proxy scale.

enum PROXY
{
  PROXY_NONE,
  PROXY_1_2,
  PROXY_1_4,
  PROXY_1_8,
  
  _PROXY_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Info
//
//! Image data information.
//------------------------------------------------------------------------------

class _DJV_IMAGE Info
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Info();
  
  Info(
    const V2i & size,
    PIXEL
  );
  
  Info(
    const String & file_name,
    const V2i & size,
    PIXEL
  );

  //@}
  
  String file_name;
  String layer_name;
  V2i size;
  PROXY proxy;
  pixel::PIXEL pixel;
  bool bgr;
  V2b mirror;
  int align;
  memory::ENDIAN endian;

private:

  void init();
};

//------------------------------------------------------------------------------
//!\class Data
//
//! Image data.
//------------------------------------------------------------------------------

class _DJV_IMAGE Data
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Data();
  Data(const Data &);
  Data(const Info &, const uint8_t * = 0);
  
  virtual ~Data();
  
  Data & operator = (const Data &);

  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  void set(const Data &);
  void set(const Info &, const uint8_t * = 0);
  
  void zero();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Information
  //@{
  
  inline const Info & info() const;
  inline const V2i & size() const;
  inline PROXY proxy() const;
  inline int w() const;
  inline int h() const;
  inline PIXEL pixel() const;
  inline int channels() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Data
  //@{
  
  inline uint8_t * data();
  inline const uint8_t * data() const;
  inline uint8_t * data(int x, int y);
  inline const uint8_t * data(int x, int y) const;
  
  inline size_t bytes_pixel() const;
  inline size_t bytes_scanline() const;
  inline size_t bytes_data() const;

  //@}
  
private:

  void init();
  void alloc();
  void del();
  
  Info _info;
  int _channels;
  memory::Buffer<uint8_t> _data;
  size_t _bytes_pixel, _bytes_scanline, _bytes_data;
  const uint8_t * _p;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Proxy scale image data.

_DJV_IMAGE void proxy_scale(const Data &, Data *, PROXY);

//! Calculate proxy scale.

_DJV_IMAGE int proxy_scale(PROXY);
_DJV_IMAGE V2i proxy_scale(const V2i &, PROXY);
_DJV_IMAGE Box2i proxy_scale(const Box2i &, PROXY);

//! Get the number of bytes in a scanline.

_DJV_IMAGE size_t bytes_scanline(const Info &);

//! Get the number of bytes in image data.

_DJV_IMAGE size_t bytes_data(const Info &);

//! Interleave image data channels.

_DJV_IMAGE void planar_interleave(const Data &, Data *, PROXY = PROXY_NONE);

//! De-interleave image data channels.

_DJV_IMAGE void planar_deinterleave(const Data &, Data *);

//! Create a linear gradient.

_DJV_IMAGE void gradient(Data *);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool operator == (const Info &, const Info &);
_DJV_IMAGE bool operator == (const Data &, const Data &);

_DJV_IMAGE bool operator != (const Info &, const Info &);
_DJV_IMAGE bool operator != (const Data &, const Data &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String & operator >> (String &, PROXY &) throw (String);

_DJV_IMAGE String & operator << (String &, PROXY);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_proxy();

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, PROXY);
_DJV_IMAGE Debug & operator << (Debug &, const Info &);
_DJV_IMAGE Debug & operator << (Debug &, const Data &);

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_image/data_inline.h>
#endif

