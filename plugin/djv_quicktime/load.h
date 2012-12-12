/*------------------------------------------------------------------------------
 plugin/djv_quicktime/load.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_QUICKTIME_LOAD_H
#define DJV_QUICKTIME_LOAD_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace quicktime {
using namespace djv_base;
using namespace djv_image;

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

  void init() throw (Error);
  void del();
  
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
  
  io::Info _info;
  int _start_frame;
  qt::Movie _movie;
  qt::GWorldPtr _gworld;
  Image _image;
  Image _tmp;
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

