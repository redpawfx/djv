/*------------------------------------------------------------------------------
 plugin/djv_tiff/load.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_TIFF_IO_H
#define DJV_TIFF_IO_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace tiff {
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
  TIFF * _f;
  
  bool _planar;
  bool _compression;
  bool _palette;
  uint16 * _colormap [3];
  Image _image;
  Data _tmp;
};

//------------------------------------------------------------------------------

}
}

#endif

