/*------------------------------------------------------------------------------
 plugin/djv_vlut/save.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VLUT_SAVE_H
#define DJV_VLUT_SAVE_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace vlut {
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
  //!\name Plugin
  //@{
  
  plugin::Plugin * copy() const;
  
  String name() const;
  
  List<String> extension_list() const;
  
  bool is_seq() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Save
  //@{
  
  void open(const File &, const io::Info &) throw (Error);
  
  void set(const Image &, const io::Frame_Info &) throw (Error);

  //@}
  
private:

  Info _info;
  Image _image;
  Data _tmp;
#if defined(DJV_WINDOWS)
  Handle _handle;
#endif
};

//------------------------------------------------------------------------------

}
}

#endif

