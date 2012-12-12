/*------------------------------------------------------------------------------
 lib/djv_ui/color.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_COLOR_H
#define DJV_UI_COLOR_H

#include <djv_ui/color_def.h>

#include <djv_image/color.h>
#include <djv_ui/frame.h>
#include <djv_ui/layout_row_def.h>
#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::color
//
//! Color widgets.
//------------------------------------------------------------------------------

namespace color {

//------------------------------------------------------------------------------
//!\class Swatch
//
//! A basic color swatch widget.
//------------------------------------------------------------------------------

class _DJV_UI Swatch : public Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Swatch();
  
  virtual ~Swatch();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const djv_image::Color &);
  
  const djv_image::Color & get() const;
  
  callback::Signal<const djv_image::Color &> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{

  virtual int handle(int);
  virtual void draw();
  
  callback::Signal<bool> action_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();

  //@}
  
private:

  djv_image::Color _value;
};

//------------------------------------------------------------------------------
//!\class Swatch_Widget
//
//! A color swatch widget.
//------------------------------------------------------------------------------

class _DJV_UI Swatch_Widget : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Swatch_Widget(const String & label = String());
  
  virtual ~Swatch_Widget();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const djv_image::Color &);
  
  const djv_image::Color & get() const;
  
  callback::Signal<const djv_image::Color &> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Label
  //@{

  void label(const String &);
  
  const String & label() const;
  
  //@}
  
private:

  DJV_CALLBACK(Swatch_Widget, widget_callback, const djv_image::Color &);
  DJV_CALLBACK(Swatch_Widget, dialog_callback, bool);
  
  struct _Swatch_Widget;
  std::auto_ptr<_Swatch_Widget> _p;
};

//------------------------------------------------------------------------------
//!\class Color_Widget
//
//! A color picker widget.
//------------------------------------------------------------------------------

class _DJV_UI Color_Widget : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Color_Widget();
  
  virtual ~Color_Widget();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const djv_image::Color &);
  
  const djv_image::Color & get() const;
  
  callback::Signal<const djv_image::Color &> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Float Range
  //@{
  
  void float_range(double min, double max);
  
  double float_min() const;
  double float_max() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  Layout_H * bottom_layout();

  //@}
  
private:

  DJV_CALLBACK(Color_Widget, int_callback, int);
  DJV_CALLBACK(Color_Widget, float_callback, double);
  DJV_CALLBACK(Color_Widget, format_callback, int);
  DJV_CALLBACK(Color_Widget, type_callback, int);
  
  void widget_update();
  void value_update();

  djv_image::Color _value;
  double _float_min, _float_max;
  
  struct _Color_Widget;
  std::auto_ptr<_Color_Widget> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  tooltip_red,
  tooltip_green,
  tooltip_blue,
  tooltip_alpha,
  tooltip_luminance,
  tooltip_pixel_format,
  tooltip_pixel_type;

//@}
//------------------------------------------------------------------------------

}
}

#endif

