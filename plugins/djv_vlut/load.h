/*------------------------------------------------------------------------------
 plugin/djv_vlut/load.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VLUT_LOAD_H
#define DJV_VLUT_LOAD_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace vlut {
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
  //!\name Load
  //@{
  
  void open(const File &, io::Info &) throw (Error);

  const Image * get(const io::Frame_Info &) throw (Error);

  //@}
  
private:

  int _size;
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

