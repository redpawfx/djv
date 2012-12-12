/*------------------------------------------------------------------------------
 plugin/djv_tiff/base.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_TIFF_BASE_H
#define DJV_TIFF_BASE_H

#include <djv_image/base.h>
#include <tiffio.h>

namespace djv_plugin {

//------------------------------------------------------------------------------
//!\namespace djv_plugin::tiff
//
//! TIFF plugin - Tagged image file format.
//!
//! Requires:
//!
//! - libtiff - http://www.libtiff.org
//!
//! Support:
//!
//! - Image data: L, LA, RGB, RGBA, 8-bit, 16-bit, 32-bit float
//! - Interleaved channels only
//! - File compression
//------------------------------------------------------------------------------

namespace tiff {
using namespace djv_base;
using namespace djv_image;

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "TIFF";

static const List<String> extension_list = List<String>() <<
  ".tiff" <<
  ".tif";

//@}
//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Compression.

enum COMPRESSION
{
  // XXX libtiff defines
  
  _COMPRESSION_NONE,
  _COMPRESSION_RLE,
  _COMPRESSION_LZW,
  
  _COMPRESSION_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

void tiff_init();

void palette_load(
  uint8_t *,
  int size,
  int bytes,
  uint16_t * red,
  uint16_t * green,
  uint16_t * blue
);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

String & operator >> (String &, COMPRESSION &) throw (String);

String & operator << (String &, COMPRESSION);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_compression();
const List<String> & label_compression_text();

//@}
//------------------------------------------------------------------------------

}
}

#endif

