/*------------------------------------------------------------------------------
 include/djv_image/io_lut.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_IO_LUT_H
#define DJV_IMAGE_IO_LUT_H

#include <djv_image/io.h>
#include <djv_base/file_io.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::io_lut
//
//! LUT plugin - Two-dimensional lookup table file formats.
//!
//! Lookup tables provide a way to store basic color transformations.
//!
//! Support:
//!
//! - File types: Inferno, Kodak
//! - Image data:
//!   - L, LA, RGB, RGBA, 8-bit, 16-bit
//!   - RGB, 10-bit
//------------------------------------------------------------------------------

namespace io_lut {

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "LUT";

static const List<String> extension_list = List<String>() <<
  ".lut" <<
  ".1dl";

//@}
//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! File format.

enum FORMAT
{
  FORMAT_AUTO,
  FORMAT_INFERNO,
  FORMAT_KODAK,
  
  _FORMAT_SIZE
};

//! File type.

enum TYPE
{
  TYPE_AUTO,
  TYPE_U8,
  TYPE_U10,
  TYPE_U16,
  
  _TYPE_SIZE
};

//! Load options.
  
enum LOAD_OPTION
{
  LOAD_FORMAT,
  LOAD_TYPE,
  
  _LOAD_OPTION_SIZE
};

struct _DJV_IMAGE Load_Option
{
  Load_Option() :
    format(FORMAT_AUTO),
    type(TYPE_AUTO)
  {}

  FORMAT format;
  TYPE type;
};

//! Save options.

enum SAVE_OPTION
{
  SAVE_FORMAT,
  
  _SAVE_OPTION_SIZE
};

struct _DJV_IMAGE Save_Option
{
  Save_Option() :
    format(FORMAT_AUTO)
  {}

  FORMAT format;
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
  //!\name Load
  //@{
  
  void open(const File &, io::Info &) throw (Error);
  
  const Image * get(const io::Frame_Info &) throw (Error);

  //@}
  
private:

  void _open(const File &, io::Info &) throw (Error);

  Load_Option _option;
  
  File _file;
  File_Io _io;
  
  FORMAT _format;
  Image _image;
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

  void _open(const File &) throw (Error);
  
  Save_Option _option;
  
  File _file;
  File_Io _io;
  
  FORMAT _format;
  Info _info;
  Image _image;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE void inferno_open(File_Io &, Info &, TYPE) throw (Error);
_DJV_IMAGE void inferno_load(File_Io &, Image *) throw (Error);
_DJV_IMAGE void kodak_open(File_Io &, Info &, TYPE) throw (Error);
_DJV_IMAGE void kodak_load(File_Io &, Image *) throw (Error);

_DJV_IMAGE void inferno_open(File_Io &, const Info &) throw (Error);
_DJV_IMAGE void inferno_save(File_Io & io, const Data *) throw (Error);
_DJV_IMAGE void kodak_open(File_Io &, const Info &) throw (Error);
_DJV_IMAGE void kodak_save(File_Io &, const Data *) throw (Error);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String & operator >> (String &, FORMAT &) throw (String);
_DJV_IMAGE String & operator >> (String &, TYPE &) throw (String);

_DJV_IMAGE String & operator << (String &, FORMAT);
_DJV_IMAGE String & operator << (String &, TYPE);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_format();
_DJV_IMAGE const List<String> & label_type();
_DJV_IMAGE const List<String> & label_load_option();
_DJV_IMAGE const List<String> & label_save_option();

//@}
//------------------------------------------------------------------------------

}
}

#endif

