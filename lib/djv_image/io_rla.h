/*------------------------------------------------------------------------------
 include/djv_image/io_rla.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_IO_RLA_H
#define DJV_IMAGE_IO_RLA_H

#include <djv_image/io.h>
#include <djv_base/file_io.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::io_rla
//
//! RLA plugin - Wavefront RLA image file format.
//!
//! Support:
//!
//! - Read-only
//! - Image data: L, LA, RGB, RGBA, 8-bit, 16-bit, 32-bit float
//! - File compression
//!
//! References:
//!
//! - James D. Murray, William vanRyper, "Encyclopedia of Graphics File Formats,
//!   Second Edition"
//------------------------------------------------------------------------------

namespace io_rla {

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "RLA";

static const List<String> extension_list = List<String>() <<
  ".rla" <<
  ".rpf";

//@}
//------------------------------------------------------------------------------
//!\class Load
//
//! Load plugin.
//------------------------------------------------------------------------------

class _DJV_IMAGE Load : public io::Load
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Load();

  //@}
  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{

  Plugin * copy() const;
  
  String name() const;
  
  List<String> extension_list() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Load
  //@{
  
  void open(const File &, io::Info &) throw (Error);
 
  const Image * get(const io::Frame_Info &) throw (Error);

  //@}
  
private:

  void _open(const String &, io::Info &) throw (Error);

  File _file;
  File_Io _io;
  
  memory::Buffer<int32_t> _rle_offset;
  Image _image;
  Data _tmp;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE void rle_load(
  File_Io &,
  uint8_t *,
  int size,
  int channels,
  int bytes
) throw (Error);

_DJV_IMAGE void float_load(
  File_Io &,
  uint8_t *,
  int size,
  int channels
) throw (Error);

_DJV_IMAGE void skip(File_Io &) throw (Error);

//@}
//------------------------------------------------------------------------------

}
}

#endif

