/*------------------------------------------------------------------------------
 include/djv_image/io_ppm.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_IO_PPM_H
#define DJV_IMAGE_IO_PPM_H

#include <djv_image/io.h>
#include <djv_base/file_io.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::io_ppm
//
//! PPM plugin - NetPBM image file formats.
//!
//! Support:
//!
//! - Image data: L, RGB, 1-bit, 8-bit, 16-bit
//!
//! References:
//!
//! - Netpbm, "PPM Format Specification"
//!   http://netpbm.sourceforge.net/doc/ppm.html
//------------------------------------------------------------------------------

namespace io_ppm {

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "PPM";

static const List<String> extension_list = List<String>() <<
  ".ppm" <<
  ".pnm" <<
  ".pgm" <<
  ".pbm";

//@}
//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Type.

enum TYPE
{
  TYPE_AUTO,
  TYPE_U1,
  
  _TYPE_SIZE
};

//! Data.

enum DATA
{
  DATA_ASCII,
  DATA_BINARY,
  
  _DATA_SIZE
};

//! Save options.

enum SAVE_OPTION
{
  SAVE_TYPE,
  SAVE_DATA,
  
  _SAVE_OPTION_SIZE
};

struct _DJV_IMAGE Save_Option
{
  Save_Option() :
    type(TYPE_AUTO),
    data(DATA_BINARY)
  {}

  TYPE type;
  DATA data;
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
  
  int _bit_depth;
  DATA _data;
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
  
  int _bit_depth;
  Info _info;
  Image _image;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE size_t bytes_scanline(
  int width,
  int channels,
  int bit_depth,
  DATA
);

_DJV_IMAGE void ascii_load(
  File_Io &,
  void *,
  int size,
  int bit_depth
) throw (Error);

_DJV_IMAGE size_t ascii_save(
  const void *,
  void *,
  int size,
  int bit_depth
);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String & operator >> (String &, TYPE &) throw (String);
_DJV_IMAGE String & operator >> (String &, DATA &) throw (String);

_DJV_IMAGE String & operator << (String &, TYPE);
_DJV_IMAGE String & operator << (String &, DATA);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_type();
_DJV_IMAGE const List<String> & label_data();
_DJV_IMAGE const List<String> & label_save_option();

//@}
//------------------------------------------------------------------------------

}
}

#endif

