/*------------------------------------------------------------------------------
 plugin/djv_png/save.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_PNG_SAVE_H
#define DJV_PNG_SAVE_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace png {
using namespace djv_base;
using namespace djv_image;

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
  //!\name Save
  //@{
  
  void open(const File &, const io::Info &) throw (Error);
  
  void set(const Image &, const io::Frame_Info &) throw (Error);

  //@}
  
private:

  void _open(const String &, const io::Info &) throw (Error);
  void _close();
  
  File _file;
  FILE * _f;
  
  png_structp _png;
  png_infop _png_info;
  Png_Error _png_error;
  
  Info _info;
  Image _image;
};

//------------------------------------------------------------------------------

}
}

#endif

