/*------------------------------------------------------------------------------
 include/djv_image/io_targa.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_IO_TARGA_H
#define DJV_IMAGE_IO_TARGA_H

#include <djv_image/io.h>
#include <djv_base/file_io.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::io_targa
//
//! Targa plugin - Targa image file format.
//!
//! Support:
//!
//! - Image data: L, LA, RGB, RGBA, 8-bit
//! - File compression
//!
//! References:
//!
//! - James D. Murray, William vanRyper, "Encyclopedia of Graphics File Formats,
//!   Second Edition"
//------------------------------------------------------------------------------

namespace io_targa {

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "Targa";

static const List<String> extension_list = List<String>() <<
  ".tga";

//@}
//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Compression.

enum COMPRESSION
{
  COMPRESSION_NONE,
  COMPRESSION_RLE,
  
  _COMPRESSION_SIZE
};

//! Save options.

enum SAVE_OPTION
{
  SAVE_COMPRESSION,
  
  _SAVE_OPTION_SIZE
};

struct _DJV_IMAGE Save_Option
{
  Save_Option() :
    compression(COMPRESSION_NONE)
  {}

  COMPRESSION compression;
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
  
  bool _compression;
  Image _image;
  Data _tmp;
};

//------------------------------------------------------------------------------
//!\class Save
//
//! Save plugin.
//------------------------------------------------------------------------------

class _DJV_IMAGE Save : public io::Save
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Save();

  //@}
  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{
  
  Plugin * copy() const;
  
  String name() const;
  
  List<String> extension_list() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Options
  //@{
  
  bool option(const String &, String *);
  
  String option(const String &) const;
  List<String> option_list() const;
  List<String> option_value(const String &) const;
  String option_default(const String &) const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Save
  //@{

  void open(const File &, const io::Info &) throw (Error);
  
  void set(const Image &, const io::Frame_Info &) throw (Error);

  //@}
  
private:
  
  void _open(const String &) throw (Error);

  Save_Option _option;
  
  File _file;
  File_Io _io;
  
  Info _info;
  Image _image;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE void info_load(
  File_Io &,
  io::Info &,
  bool * compression
) throw (Error);

_DJV_IMAGE void info_save(
  File_Io &,
  const io::Info &,
  bool compression
) throw (Error);

_DJV_IMAGE const uint8_t * rle_load(
  const uint8_t * in,
  const uint8_t * end,
  uint8_t * out,
  int size,
  int channels
);

_DJV_IMAGE size_t rle_save(
  const uint8_t * in,
  uint8_t * out,
  int size,
  int channels
);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String & operator >> (String &, COMPRESSION &) throw (String);

_DJV_IMAGE String & operator << (String &, COMPRESSION);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_compression();
_DJV_IMAGE const List<String> & label_compression_text();
_DJV_IMAGE const List<String> & label_save_option();

//@}
//------------------------------------------------------------------------------

}
}

#endif

