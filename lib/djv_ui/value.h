/*------------------------------------------------------------------------------
 lib/djv_ui/value.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_VALUE_H
#define DJV_UI_VALUE_H

#include <djv_ui/value_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::value
//
//! Numeric value widgets.
//------------------------------------------------------------------------------

namespace value {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

static const int int_min = -1000000;
static const int int_max = 1000000;

static const double float_min = -1000000.0;
static const double float_max = 1000000.0;

//@}
//------------------------------------------------------------------------------
//!\class Value_Input_Base
//
//! A base class for numeric value widgets.
//------------------------------------------------------------------------------

template<class T>
class Value_Input_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  inline Value_Input_Base(T min = T(), T max = T());
  
  inline virtual ~Value_Input_Base() = 0;

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  inline virtual void set(T);
  
  inline const T & get() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Default Value
  //@{

  inline virtual void default_value(T);

  inline const T & default_value() const;

  inline virtual void default_is_valid(bool);
  
  inline bool default_is_valid() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Range
  //@{

  inline virtual void range(T min, T max);
  inline virtual void clamp(bool);
  inline virtual void min_action();
  inline virtual void max_action();
  
  inline const T & min() const;
  inline const T & max() const;
  inline bool clamp() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Increment
  //@{

  inline virtual void inc(T inc_small, T inc_large);
  inline virtual void inc_small_action();
  inline virtual void inc_large_action();
  inline virtual void dec_small_action();
  inline virtual void dec_large_action();
  
  inline const T & inc_small() const;
  inline const T & inc_large() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  inline int handle(int, bool keyboard_focus);

  //@}
  
protected:
  
  virtual void value_callback() {}
  virtual void range_callback() {}

private:

  T _value;
  bool _default_is_valid;
  T _default_value;
  T _min, _max;
  bool _clamp;
  T _inc_small, _inc_large;
};

//------------------------------------------------------------------------------
//!\class Int_Input_Base
//
//! A base class for integer value widgets.
//------------------------------------------------------------------------------

class _DJV_UI Int_Input_Base : public Value_Input_Base<int>
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Int_Input_Base(int min = int_min, int max = int_max);
  
  //@}
};

//------------------------------------------------------------------------------
//!\class Float_Input_Base
//
//! A base class for floating-point value widgets.
//------------------------------------------------------------------------------

class _DJV_UI Float_Input_Base : public Value_Input_Base<double>
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Float_Input_Base(double min = float_min, double max = float_max);
  
  virtual ~Float_Input_Base() {}

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  virtual void precision(int);
  
  virtual int precision() const;
  
  //@}
  
private:

  int _precision;
};

//------------------------------------------------------------------------------
//!\class Inc_Tool
//
//! A tool widget that provides increment/decrement buttons for numeric value
//! widgets.
//------------------------------------------------------------------------------

class _DJV_UI Inc_Tool : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Inc_Tool();
  
  virtual ~Inc_Tool();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  callback::Signal<bool> inc_signal;
  callback::Signal<bool> dec_signal;
  callback::Signal<bool> push_signal;
  
  //@}

private:

  DJV_CALLBACK(Inc_Tool, inc_callback, bool);
  DJV_CALLBACK(Inc_Tool, dec_callback, bool);
  DJV_CALLBACK(Inc_Tool, push_callback, bool);
  
  struct _Inc_Tool;
  std::auto_ptr<_Inc_Tool> _p;
};

//------------------------------------------------------------------------------

}
}

#include <djv_ui/value_inline.h>
#endif

