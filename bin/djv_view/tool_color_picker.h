/*------------------------------------------------------------------------------
 bin/djv_view/tool_color_picker.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_TOOL_COLOR_PICKER_H
#define DJV_VIEW_TOOL_COLOR_PICKER_H

#include "base.h"
#include <djv_ui/dialog.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::tool_color_picker
//
//! Color picker tool.
//------------------------------------------------------------------------------

namespace tool_color_picker {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

enum SIZE
{
  SIZE_1,
  SIZE_3,
  SIZE_5,
  SIZE_7,
  SIZE_9,
  SIZE_51,
  SIZE_101,
  
  _SIZE_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Dialog
//
//! Color picker dialog.
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
  
  void update() { Base::update(); }

  //@}
  
private:

  DJV_CALLBACK(Dialog, widget_callback, const djv_image::Color &);
  DJV_CALLBACK(Dialog, size_callback, int);
  DJV_CALLBACK(Dialog, color_profile_callback, bool);
  DJV_CALLBACK(Dialog, display_profile_callback, bool);
  DJV_CALLBACK(Dialog, lock_callback, bool);
  DJV_CALLBACK(Dialog, close_callback, bool);

  void widget_update();
  
  const view::Widget * _view;
  V2i _pick;
  djv_image::Color _value;
  SIZE _size;
  bool _color_profile;
  bool _display_profile;
  bool _lock;
  
  struct _Dialog;
  std::auto_ptr<_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

int size(SIZE);

String & operator >> (String &, SIZE &) throw (String);

String & operator << (String &, SIZE);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_size();

extern const String
  dialog_title;

extern const String
  tooltip_size,
  tooltip_color_profile,
  tooltip_display_profile,
  tooltip_lock;

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

