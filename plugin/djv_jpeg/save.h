/*------------------------------------------------------------------------------
 plugin/djv_jpeg/save.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_JPEG_SAVE_H
#define DJV_JPEG_SAVE_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace jpeg {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Save options.

enum SAVE_OPTION
{
  SAVE_QUALITY,
  
  _SAVE_OPTION_SIZE
};

struct Save_Option
{
  Save_Option() :
    quality(100)
  {}

  int quality;
};

//@}
//------------------------------------------------------------------------------
//!\class Save
//
//! Save plugin.
//------------------------------------------------------------------------------

class Save : public djv_image::io::Save
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Save();
  
  ~Save();

  //@}
  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{
  
  plugin::Plugin * copy() const;
  
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
  
  void _open(const String &, const io::Info &) throw (Error);
  void _close();

  Save_Option _option;
  
  File _file;
  FILE * _f;
  
  libjpeg::jpeg_compress_struct _jpeg;
  bool _jpeg_init;
  Jpeg_Error _jpeg_error;
  
  Info _info;
  Image _image;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_save_option();

extern const String
  label_quality;

//@}
//------------------------------------------------------------------------------

}
}

#endif

