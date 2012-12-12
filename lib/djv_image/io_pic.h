/*------------------------------------------------------------------------------
 lib/djv_image/io_pic.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_IO_PIC_H
#define DJV_IMAGE_IO_PIC_H

#include <djv_image/io.h>
#include <djv_base/file_io.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::io_pic
//
//! PIC plugin - Softimage image file format.
//!
//! Support:
//!
//! - Read-only
//! - Image data:
//!   - RGB, RGBA, 8-bit
//!   - RGB plus alpha, 8-bit
//! - File compression
//!
//! References:
//!
//! - Softimage, "INFO: PIC file format"
//!   http://xsi.wiki.avid.com/index.php/INFO:_PIC_file_format
//------------------------------------------------------------------------------

namespace io_pic {

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "PIC";

static const List<String> extension_list = List<String>() <<
  ".pic";

//@}
//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Type.

enum TYPE
{
  TYPE_RGB,
  TYPE_RGBA,
  TYPE_RGB_A,
  
  _TYPE_SIZE
};

//! Compression.

enum COMPRESSION
{
  COMPRESSION_NONE,
  COMPRESSION_RLE,
  
  _COMPRESSION_SIZE
};

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
  
  TYPE _type;
  bool _compression [2];
  Image _image;
  Data _tmp;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const uint8_t * rle_load(
  const uint8_t * in,
  const uint8_t * end,
  uint8_t * out,
  int size,
  int channels,
  int stride,
  bool endian
);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_compression();

//@}
//------------------------------------------------------------------------------

}
}

#endif

