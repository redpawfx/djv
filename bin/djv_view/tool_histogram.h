/*------------------------------------------------------------------------------
 bin/djv_view/tool_histogram.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_TOOL_HISTOGRAM_H
#define DJV_VIEW_TOOL_HISTOGRAM_H

#include "base.h"
#include <djv_ui/dialog.h>
#include <djv_ui/frame.h>
#include <djv_image/gl_data.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::tool_histogram
//
//! Histogram tool.
//------------------------------------------------------------------------------

namespace tool_histogram {

//------------------------------------------------------------------------------
//!\class Widget
//
//! Histogram widget.
//------------------------------------------------------------------------------

class Widget : public djv_ui::Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Widget();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(
    const djv_image::Data *,
    const djv_image::Color & min,
    const djv_image::Color & max
  );
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void draw();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  void dirty();
  
  //@}
  
private:

  const djv_image::Data * _value;
  int _value_max;
  djv_image::Color _min, _max;
};

//------------------------------------------------------------------------------
//!\class Dialog
//
//! Histogram dialog.
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
  
  void set(const view::Widget *);
  void pick(const view::Widget *);
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

  DJV_CALLBACK(Dialog, size_callback, int);
  DJV_CALLBACK(Dialog, close_callback, bool);
  
  void widget_update();
  
  const view::Widget * _view;
  djv_image::gl_data::HISTOGRAM _size;
  djv_image::Data _histogram;
  djv_image::Color _min, _max;
  
  struct _Dialog;
  std::auto_ptr<_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  dialog_title,
  dialog_size,
  dialog_min,
  dialog_max;

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

