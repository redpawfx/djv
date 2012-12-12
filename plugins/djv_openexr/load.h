/*------------------------------------------------------------------------------
 plugin/djv_openexr/load.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_OPENEXR_LOAD_H
#define DJV_OPENEXR_LOAD_H

#include "base.h"
#include <djv_image/io.h>
#include <djv_image/image_util.h>

namespace Imf {
class InputFile;
}

namespace djv_plugin {
namespace openexr {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Load options.

enum LOAD_OPTION
{
  LOAD_COLOR_PROFILE,
  LOAD_GAMMA,
  LOAD_EXPOSURE,
  LOAD_CHANNELS,
  
  _LOAD_OPTION_SIZE
};

struct Load_Option
{
  Load_Option()  :
    color_profile(COLOR_PROFILE_GAMMA),
    gamma(2.2),
    channels(CHANNELS_GROUP_KNOWN)
  {}

  COLOR_PROFILE color_profile;
  
  double gamma;
  
  color_profile::Exposure exposure;
  
  CHANNELS channels;
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
  
  void _open(const String &, io::Info &) throw (Error);
  
  void _close();
  
  Load_Option _option;
  
  File _file;
  Imf::InputFile * _f;
  
  Box2i _display_window;
  Box2i _data_window;
  
  List<Layer> _layer_list;
  
  util::Double_Buffer _buffer;
  
  Frame_Cache _frame_cache;
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

