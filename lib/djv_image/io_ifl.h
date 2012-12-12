/*------------------------------------------------------------------------------
 include/djv_image/io_ifl.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_IO_IFL_H
#define DJV_IMAGE_IO_IFL_H

#include <djv_image/io.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::io_ifl
//
//! IFL plugin - Autodesk Image File List format.
//!
//! IFL is a file format for creating sequences or playlists of other image
//! files. An IFL file simply consists of a list of image file names, one per
//! line.
//! 
//! Support:
//!
//! - Read only
//------------------------------------------------------------------------------

namespace io_ifl {

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "IFL";

static const List<String> extension_list = List<String>() <<
  ".ifl";

//@}
//------------------------------------------------------------------------------
//!\class Load
//
//! Load plugin.
//------------------------------------------------------------------------------

class _DJV_IMAGE Load : public io::Load
{
public:
  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{

  Plugin * copy() const;
  
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

  List<String> _list;
  
  std::auto_ptr<io::Load> _load;
};

//------------------------------------------------------------------------------

}
}

#endif

