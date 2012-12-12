/*------------------------------------------------------------------------------
 plugin/djv_jpeg/load.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_JPEG_LOAD_H
#define DJV_JPEG_LOAD_H

#include "base.h"
#include <djv_image/io.h>

namespace djv_plugin {
namespace jpeg {

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
  
  libjpeg::jpeg_decompress_struct _jpeg;
  bool _jpeg_init;
  Jpeg_Error _jpeg_error;
  
  Image _image;
  Data _tmp;
};

//------------------------------------------------------------------------------

}
}

#endif

