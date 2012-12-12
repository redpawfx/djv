/*------------------------------------------------------------------------------
 include/djv_base/directory.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_DIRECTORY_H
#define DJV_DIRECTORY_H

#include <djv_base/directory_def.h>

#include <djv_base/file.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::directory
//
//! File-system directories.
//------------------------------------------------------------------------------

namespace directory {

//------------------------------------------------------------------------------
//!\class Directory
//
//! A file-system directory.
//------------------------------------------------------------------------------

class _DJV_BASE Directory
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Directory();
  Directory(const String &);

  //@}
  //----------------------------------------------------------------------------
  //!\name Path
  //@{
  
  void set(const String &);
  
  const String & get() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  //! Get a list of items in the directory.
  
  List<File> list() const;

  //! Change directory.
  
  void cd(const String &);
  
  //! Go up a directory.
  
  void up();

  //! Is the directory valid?
  
  bool is_valid() const;

  //@}
  
private:

  void update();
  
  String _path;
  
  bool _is_valid;
};

//------------------------------------------------------------------------------

}
}

#include <djv_base/directory_fnc.h>
#endif

