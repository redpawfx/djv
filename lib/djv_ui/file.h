/*------------------------------------------------------------------------------
 lib/djv_ui/file.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_FILE_H
#define DJV_UI_FILE_H

#include <djv_ui/file_def.h>

#include <djv_ui/widget.h>
#include <djv_base/file.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::file
//
//! File widgets.
//------------------------------------------------------------------------------

namespace file {

//------------------------------------------------------------------------------
//!\class File_Edit
//
//! A file name edit widget.
//------------------------------------------------------------------------------

class _DJV_UI File_Edit : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  File_Edit();
  
  ~File_Edit();

  void del();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const File &);
  
  const File & get() const;
  
  callback::Signal<const File &> signal;
  
  //@}
  
private:

  DJV_CALLBACK(File_Edit, widget_callback, const String &);
  DJV_CALLBACK(File_Edit, dialog_callback, bool);
  DJV_CALLBACK(File_Edit, dialog_callback, const File &);
  
  void widget_update();
  
  File _value;
  
  struct _File_Edit;
  std::auto_ptr<_File_Edit> _p;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_UI void recent(const String &, List<String> &, int max = 10);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  label_browse;

//@}
//------------------------------------------------------------------------------

}
}

#endif

