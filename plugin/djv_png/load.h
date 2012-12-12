/*------------------------------------------------------------------------------
 plugin/djv_png/load.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_PNG_LOAD_H
#define DJV_PNG_LOAD_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace png {
using namespace djv_base;
using namespace djv_image;

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

  File _file;
  FILE * _f;
  
  png_structp _png;
  png_infop _png_info, _png_info_end;
  Png_Error _png_error;
  
  Image _image;
  Data _tmp;
};

//------------------------------------------------------------------------------

}
}

#endif

