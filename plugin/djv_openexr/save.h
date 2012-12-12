/*------------------------------------------------------------------------------
 plugin/djv_openexr/save.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_OPENEXR_SAVE_H
#define DJV_OPENEXR_SAVE_H

#include "base.h"
#include <djv_image/io.h>

namespace Imf {
class OutputFile;
}

namespace djv_plugin {
namespace openexr {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Save options.

enum SAVE_OPTION
{
  SAVE_COMPRESSION,
  
  _SAVE_OPTION_SIZE
};

struct Save_Option
{
  Save_Option() :
    compression(COMPRESSION_NONE)
  {}

  COMPRESSION compression;
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

  void init() throw (Error);
  void del();

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
  Imf::OutputFile * _f;
  
  Info _info;
  List<String> _channel_list;
  djv_image::time::Speed _speed;
  
  Data _tmp;
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

