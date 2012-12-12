/*------------------------------------------------------------------------------
 lib/djv_ui/value_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include <FL/Fl.H>

namespace djv_ui {
namespace value {

//------------------------------------------------------------------------------
// Value_Input_Base
//------------------------------------------------------------------------------

template<class T>
inline Value_Input_Base<T>::Value_Input_Base(T min, T max) :
  _value(T()),
  _default_is_valid(false),
  _default_value(T()),
  _min(min),
  _max(max),
  _clamp(true),
  _inc_small(T()),
  _inc_large(T())
{}

template<class T>
inline Value_Input_Base<T>::~Value_Input_Base()
{}

template<class T>
inline void Value_Input_Base<T>::set(T in)
{
  const T tmp = _clamp ? math::clamp(in, _min, _max) : in;
  if (tmp == _value)
    return;
  
  _value = tmp;
  
  value_callback();
}

template<class T>
inline const T & Value_Input_Base<T>::get() const
{
  return _value;
}
  
template<class T>
inline void Value_Input_Base<T>::default_is_valid(bool in)
{
  _default_is_valid = in;
}

template<class T>
inline bool Value_Input_Base<T>::default_is_valid() const
{
  return _default_is_valid;
}

template<class T>
inline void Value_Input_Base<T>::default_value(T in)
{
  _default_value = in;
  
  default_is_valid(true);
}

template<class T>
inline const T & Value_Input_Base<T>::default_value() const
{
  return _default_value;
}

template<class T>
inline void Value_Input_Base<T>::range(T min, T max)
{
  if (min == _min && max == _max)
    return;
  
  _min = min;
  _max = max;
  
  set(_value);
  
  range_callback();
}

template<class T>
inline void Value_Input_Base<T>::min_action()
{
  set(_min);
}

template<class T>
inline void Value_Input_Base<T>::max_action()
{
  set(_max);
}

template<class T>
inline const T & Value_Input_Base<T>::min() const
{
  return _min;
}

template<class T>
inline const T & Value_Input_Base<T>::max() const
{
  return _max;
}

template<class T>
inline void Value_Input_Base<T>::inc(T inc_small, T inc_large)
{
  if (inc_small == _inc_small && inc_large == _inc_large)
    return;
  
  _inc_small = inc_small;
  _inc_large = inc_large;
  
  range_callback();
}

template<class T>
inline void Value_Input_Base<T>::inc_small_action()
{
  set(_value + _inc_small);
}

template<class T>
inline void Value_Input_Base<T>::inc_large_action()
{
  set(_value + _inc_large);
}

template<class T>
inline void Value_Input_Base<T>::dec_small_action()
{
  set(_value - _inc_small);
}

template<class T>
inline void Value_Input_Base<T>::dec_large_action()
{
  set(_value - _inc_large);
}

template<class T>
inline const T & Value_Input_Base<T>::inc_small() const
{
  return _inc_small;
}

template<class T>
inline const T & Value_Input_Base<T>::inc_large() const
{
  return _inc_large;
}

template<class T>
inline void Value_Input_Base<T>::clamp(bool in)
{
  if (in == _clamp)
    return;
  
  _clamp = in;
  
  set(_value);
}

template<class T>
inline bool Value_Input_Base<T>::clamp() const
{
  return _clamp;
}

template<class T>
inline int Value_Input_Base<T>::handle(int in, bool keyboard_focus)
{
  switch (in)
  {
    case FL_KEYBOARD:
      if (keyboard_focus)
        switch (Fl::event_key())
        {
          case FL_Up: inc_small_action(); return 1;
          case FL_Down: dec_small_action(); return 1;
          case FL_Page_Up: inc_large_action(); return 1;
          case FL_Page_Down: dec_large_action(); return 1;
        }
      break;
  }
  return 0;
}

//------------------------------------------------------------------------------

}
}

