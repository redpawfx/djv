/*------------------------------------------------------------------------------
 bin/djv_view/tool_magnify.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_TOOL_MAGNIFY_H
#define DJV_VIEW_TOOL_MAGNIFY_H

#include "base.h"
#include <djv_ui/dialog.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::tool_magnify
//
//! Magnify tool.
//------------------------------------------------------------------------------

namespace tool_magnify {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

enum ZOOM
{
  ZOOM_100,
  ZOOM_200,
  ZOOM_400,
  ZOOM_800,
  ZOOM_1600,
  
  _ZOOM_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Dialog
//
//! Magnify dialog.
//------------------------------------------------------------------------------

class Dialog : public djv_ui::dialog::Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Dialog();
  
  ~Dialog();
  
  void del();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const view::Widget *, const V2i &);
  void pick(const view::Widget *, const V2i &);
  void update(const view::Widget *);
  void del(const view::Widget *);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void show();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  void dirty();
  void update() { Base::update(); }

  //@}
  
private:
  
  DJV_CALLBACK(Dialog, zoom_callback, int);
  DJV_CALLBACK(Dialog, close_callback, bool);
  
  void value_update();
  void widget_update();
  
  const view::Widget * _view;
  V2i _pick;
  ZOOM _zoom;
  
  struct _Dialog;
  std::auto_ptr<_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

String & operator >> (String &, ZOOM &) throw (String);

String & operator << (String &, ZOOM);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_zoom();

extern const String
  dialog_title;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to global dialog.

Dialog * dialog();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

