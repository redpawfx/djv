/*------------------------------------------------------------------------------
 lib/djv_ui/float_value.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_FLOAT_VALUE_H
#define DJV_UI_FLOAT_VALUE_H

#include <djv_ui/float_value_def.h>

#include <djv_ui/value.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::float_value
//
//! Floating-point value widgets.
//------------------------------------------------------------------------------

namespace float_value {

//------------------------------------------------------------------------------
//!\class Float_Edit
//
//! A floating-point value edit widget.
//------------------------------------------------------------------------------

class _DJV_UI Float_Edit :
  public widget::Widget,
  public value::Float_Input_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Float_Edit();
  
  virtual ~Float_Edit();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  callback::Signal<double> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual int handle(int);

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  virtual Layout_Item * group_align();

  //@}
  
private:
  
  void value_callback();
  DJV_CALLBACK(Float_Edit, widget_callback, const String &);
  DJV_CALLBACK(Float_Edit, inc_callback, bool);
  DJV_CALLBACK(Float_Edit, dec_callback, bool);
  
  void value_update();
  
  struct _Float_Edit;
  std::auto_ptr<_Float_Edit> _p;
};

//------------------------------------------------------------------------------
//!\class Float_Edit_Slider
//
//! A floating-point value edit widget with slider.
//------------------------------------------------------------------------------

class _DJV_UI Float_Edit_Slider :
  public widget::Widget,
  public value::Float_Input_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Float_Edit_Slider(
    double min = 0.0,
    double max = 1.0,
    const String & label = String()
  );
  
  virtual ~Float_Edit_Slider();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  virtual void default_is_valid(bool);
  
  bool default_is_valid() const;
  
  virtual void range(double min, double max);
  virtual void slider_range(double min, double max);
  
  callback::Signal<double> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Label
  //@{
  
  void label(Widget *);
  
  Widget * label() const;
  virtual Layout_Item * label_align();

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{

  virtual void text_font(Fl_Font);
  virtual void text_color(Fl_Color);
  
  virtual Fl_Font text_font() const;
  virtual Fl_Color text_color() const;
  
  virtual int handle(int);

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  virtual Layout_Item * group_align();
  
  //@}
  
private:
  
  void value_callback();
  void range_callback();
  DJV_CALLBACK(Float_Edit_Slider, widget_callback, const String &);
  DJV_CALLBACK(Float_Edit_Slider, inc_callback, bool);
  DJV_CALLBACK(Float_Edit_Slider, dec_callback, bool);
  DJV_CALLBACK(Float_Edit_Slider, slider_callback, double);
  DJV_CALLBACK(Float_Edit_Slider, default_callback, bool);
  
  void value_update();
  void range_update();
  
  struct _Float_Edit_Slider;
  std::auto_ptr<_Float_Edit_Slider> _p;
};

//------------------------------------------------------------------------------
//!\class Float_Display
//
//! A widget for displaying floating-point values.
//------------------------------------------------------------------------------

class _DJV_UI Float_Display : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Float_Display();
  
  virtual ~Float_Display();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(double);
  
  double get() const;

  void precision(int);
  
  int precision() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void text_color(Fl_Color);
  
  virtual Fl_Color text_color() const;
  
  virtual void draw();

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  virtual void dirty();
  
  //@}
  
private:

  void widget_update();

  double _value;
  int _precision;
  
  struct _Float_Display;
  std::auto_ptr<_Float_Display> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String format_float;

//@}
//------------------------------------------------------------------------------

}
}

#endif

