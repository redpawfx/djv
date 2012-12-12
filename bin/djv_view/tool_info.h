/*------------------------------------------------------------------------------
 bin/djv_view/tool_info.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_TOOL_INFO_H
#define DJV_VIEW_TOOL_INFO_H

#include "base.h"
#include <djv_ui/dialog.h>
#include <djv_image/io.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::tool_info
//
//! Information tool.
//------------------------------------------------------------------------------

namespace tool_info {

//------------------------------------------------------------------------------
//!\class Dialog
//
//! Information dialog.
//------------------------------------------------------------------------------

class Dialog : public djv_ui::dialog::Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Dialog();
  
  ~Dialog();
  
  void del() { Base::del(); }
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const void *, const djv_image::io::Info &);
  void pick(const void *, const djv_image::io::Info &);
  void update(const void *, const djv_image::io::Info &);
  void del(const void *);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void show();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  void update() { Base::update(); }

  //@}
  
private:
  
  DJV_CALLBACK(Dialog, close_callback, bool);
  
  void widget_update();

  const void * _obj;
  djv_image::io::Info _info;
  
  struct _Dialog;
  std::auto_ptr<_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  dialog_title,
  dialog_file_name,
  dialog_layer_name,
  dialog_size,
  dialog_pixel,
  dialog_time,
  dialog_tag;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to the global dialog.

Dialog * dialog();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

