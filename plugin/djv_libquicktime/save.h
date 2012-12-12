/*------------------------------------------------------------------------------
 plugin/djv_libquicktime/save.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_LIBQUICKTIME_SAVE_H
#define DJV_LIBQUICKTIME_SAVE_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace libquicktime {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------

//! Save options.

enum SAVE_OPTION
{
  SAVE_CODEC,
  
  _SAVE_OPTION_SIZE
};

struct Save_Option
{
  Save_Option() :
    codec("jpeg")
  {}

  String codec;
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

  //@}
  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{
  
  plugin::Plugin * copy() const;
  
  String name() const;
  
  List<String> extension_list() const;
  
  bool is_seq() const;

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
  
  void close() throw (Error);
  
  void set(const Image &, const io::Frame_Info &) throw (Error);

  //@}
  
private:
  
  Save_Option _option;
  
  quicktime_t * _f;
  
  int _frame;
  int _frame_duration;
  Info _info;
  Image _image;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_save_option();

//@}
//------------------------------------------------------------------------------

}
}

#endif

