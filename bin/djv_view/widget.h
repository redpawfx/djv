/*------------------------------------------------------------------------------
 bin/djv_view/widget.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_WIDGET_H
#define DJV_WIDGET_H

#include "base.h"
#include <djv_ui/float_value_def.h>
#include <djv_ui/menu_def.h>
#include <djv_ui/text_def.h>
#include <djv_ui/slider.h>
#include <djv_image/time.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::widget
//
//! Utility widgets.
//------------------------------------------------------------------------------

namespace widget {

//------------------------------------------------------------------------------
//!\class Frame_Widget
//
//! A widget for editing frame numbers.
//------------------------------------------------------------------------------

class Frame_Widget : public djv_ui::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Frame_Widget();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(int64_t);
  
  int64_t get() const;

  void list(const List<int64_t> &);
  
  const List<int64_t> & list() const;

  void speed(const djv_image::time::Speed &);
  
  const djv_image::time::Speed & speed() const;

  djv_ui::callback::Signal<int64_t> signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void text_font(Fl_Font);
  void text_color(Fl_Color);
  
  Fl_Font text_font() const;
  Fl_Color text_color() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  void dirty();
  
  djv_ui::Layout_Item * group_align();

  //@}
  
private:

  void value_callback();
  
  DJV_CALLBACK(Frame_Widget, widget_callback, const String &);
  DJV_CALLBACK(Frame_Widget, inc_callback, bool);
  DJV_CALLBACK(Frame_Widget, dec_callback, bool);
  DJV_CALLBACK(Frame_Widget, push_callback, bool);

  void widget_update();

  int64_t _value;
  List<int64_t> _list;
  djv_image::time::Speed _speed;
  djv_ui::Text_Edit * _widget;
};

//------------------------------------------------------------------------------
//!\class Frame_Slider
//
//! A slider widget for frame numbers.
//------------------------------------------------------------------------------

class Frame_Slider : public djv_ui::Int_Slider
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Frame_Slider();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void in_out(bool);
  void in_out(int64_t, int64_t);
  
  bool in_out() const;
  int64_t in() const;
  int64_t out() const;
  
  void min_action();
  void max_action();
  
  void cached_frames(const List<int64_t> &);

  djv_ui::callback::Signal<bool> push_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  int handle(int);
  void draw();
  
  //@}
  
private:

  bool _in_out;
  int64_t _in, _out;
  List<int64_t> _cached_frames;
};

//------------------------------------------------------------------------------
//!\class Frame_Display_Widget
//
//! A widget for displaying frame numbers.
//------------------------------------------------------------------------------

class Frame_Display_Widget : public djv_ui::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Frame_Display_Widget();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(int64_t);
  
  int64_t get() const;

  void speed(const djv_image::time::Speed &);
  
  const djv_image::time::Speed & speed() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void text_font(Fl_Font);
  void text_color(Fl_Color);
  
  Fl_Font text_font() const;
  Fl_Color text_color() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  void dirty();
  
  //@}
  
private:

  void widget_update();
  
  int64_t _value;
  djv_image::time::Speed _speed;
  djv_ui::Text_Display * _widget;
};

//------------------------------------------------------------------------------
//!\class Speed_Widget
//
//! A widget for editing speed values.
//------------------------------------------------------------------------------

class Speed_Widget : public djv_ui::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Speed_Widget();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const djv_image::time::Speed &);
  void set_default(const djv_image::time::Speed &);
  
  const djv_image::time::Speed & get() const;
  const djv_image::time::Speed & get_default() const;
  
  djv_ui::callback::Signal<const djv_image::time::Speed &> signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  void dirty();
  
  //@}

private:

  DJV_CALLBACK(Speed_Widget, widget_callback, double);
  DJV_CALLBACK(Speed_Widget, menu_callback, int);
  
  void widget_update();
  void menu_update();

  djv_image::time::Speed _value, _value_default;
  //djv_ui::Text_Display * _widget;
  djv_ui::Float_Edit * _widget;
  djv_ui::menu::Popup * _menu;
};

//------------------------------------------------------------------------------
//!\class Speed_Display_Widget
//
//! A widget for displaying speed values.
//------------------------------------------------------------------------------

class Speed_Display_Widget : public djv_ui::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Speed_Display_Widget();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(double);
  
  void dropped_frames(bool);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  void dirty();
  
  //@}
  
private:

  void widget_update();

  double _value;
  bool _dropped_frames;
  
  djv_ui::Text_Display * _widget;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  label_timecode_format,
  label_frame_format,
  label_speed_format;

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

