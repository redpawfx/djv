/*------------------------------------------------------------------------------
 lib/djv_ui/int_value.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_INT_VALUE_H
#define DJV_UI_INT_VALUE_H

#include <djv_ui/int_value_def.h>

#include <djv_ui/value.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::int_value
//
//! Integer value widgets.
//------------------------------------------------------------------------------

namespace int_value {

//------------------------------------------------------------------------------
//!\class Int_Edit
//
//! An integer value edit widget.
//------------------------------------------------------------------------------

class _DJV_UI Int_Edit :
  public widget::Widget,
  public value::Int_Input_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Int_Edit();
  
  virtual ~Int_Edit();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  callback::Signal<int> signal;

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
  DJV_CALLBACK(Int_Edit, widget_callback, const String &);
  DJV_CALLBACK(Int_Edit, inc_callback, bool);
  DJV_CALLBACK(Int_Edit, dec_callback, bool);
  
  void value_update();
  
  struct _Int_Edit;
  std::auto_ptr<_Int_Edit> _p;
};

//------------------------------------------------------------------------------
//!\class Int_Edit_Slider
//
//! An integer value edit widget with slider.
//------------------------------------------------------------------------------

class _DJV_UI Int_Edit_Slider :
  public widget::Widget,
  public value::Int_Input_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Int_Edit_Slider(
    int min = 0,
    int max = 100,
    const String & label = String()
  );
  
  virtual ~Int_Edit_Slider();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  virtual void default_is_valid(bool);
  
  bool default_is_valid() const;

  callback::Signal<int> signal;

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
  DJV_CALLBACK(Int_Edit_Slider, widget_callback, const String &);
  DJV_CALLBACK(Int_Edit_Slider, inc_callback, bool);
  DJV_CALLBACK(Int_Edit_Slider, dec_callback, bool);
  DJV_CALLBACK(Int_Edit_Slider, slider_callback, int);
  DJV_CALLBACK(Int_Edit_Slider, default_callback, bool);
  
  void value_update();
  void range_update();
  
  struct _Int_Edit_Slider;
  std::auto_ptr<_Int_Edit_Slider> _p;
};

//------------------------------------------------------------------------------
//!\class Int_Display
//
//! A widget for displaying integer values.
//------------------------------------------------------------------------------

class _DJV_UI Int_Display : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Int_Display();
  
  virtual ~Int_Display();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(int);

  int get() const;
  
  //@}
  
private:

  void widget_update();

  int _value;

  struct _Int_Display;
  std::auto_ptr<_Int_Display> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String format_int;

//@}
//------------------------------------------------------------------------------

}
}

#endif

