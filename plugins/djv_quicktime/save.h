/*------------------------------------------------------------------------------
 plugin/djv_quicktime/save.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_QUICKTIME_SAVE_H
#define DJV_QUICKTIME_SAVE_H

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

//! Save options.

enum SAVE_OPTION
{
  SAVE_CODEC,
  SAVE_QUALITY,
  
  _SAVE_OPTION_SIZE
};

struct Save_Option
{
  Save_Option() :
    codec(CODEC_JPEG),
    quality(QUALITY_NORMAL)
  {}

  CODEC codec;
  QUALITY quality;
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
  //!\name Save
  //@{

  void open(const File &, const io::Info &) throw (Error);
  
  void close() throw (Error);
  
  void set(const Image &, const io::Frame_Info &) throw (Error);

  //@}
  
private:

  void _open(const String &, const io::Info &) throw (Error);
  
  Save_Option _option;
  
  //short _f;
  qt::DataHandler _f;
  int _frame_duration;
  qt::Movie _movie;
  qt::Track _track;
  qt::Media _media;
  File _file;
  qt::GWorldPtr _gworld;
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

