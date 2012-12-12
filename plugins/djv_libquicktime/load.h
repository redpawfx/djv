/*------------------------------------------------------------------------------
 plugin/djv_libquicktime/load.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_LIBQUICKTIME_LOAD_H
#define DJV_LIBQUICKTIME_LOAD_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace libquicktime {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Load options.

enum LOAD_OPTION
{
  LOAD_START_FRAME,
  
  _LOAD_OPTION_SIZE
};

struct Load_Option
{
  Load_Option() :
    start_frame(0)
  {}

  int start_frame;
};

//@}
//------------------------------------------------------------------------------
//!\class Load
//
//! Load plugin.
//------------------------------------------------------------------------------

class Load : public djv_image::io::Load
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Load();
  
  ~Load();

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
  //!\name Load
  //@{
  
  void open(const File &, io::Info &) throw (Error);
  
  const Image * get(const io::Frame_Info &) throw (Error);

  //@}
  
private:

  void _close();
  
  Load_Option _option;
  
  quicktime_t * _f;

  io::Info _info;
  int _start_frame;
  int _frame;
  Image _image;
  Data _tmp;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_load_option();

//@}
//------------------------------------------------------------------------------

}
}

#endif

