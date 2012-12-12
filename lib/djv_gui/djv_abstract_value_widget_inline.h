//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_abstract_value_widget_inline.h

#include <FL/Fl.H>

namespace djv
{

//------------------------------------------------------------------------------
// Abstract_Value_Input
//------------------------------------------------------------------------------

template<class T>
inline Abstract_Value_Input<T>::Abstract_Value_Input(T min, T max) :
    _value(T()),
    _is_default_valid(false),
    _default_value(T()),
    _min(min),
    _max(max),
    _clamp(true),
    _inc_small(T()),
    _inc_large(T())
{}

template<class T>
inline Abstract_Value_Input<T>::~Abstract_Value_Input()
{}

template<class T>
inline void Abstract_Value_Input<T>::set(T in)
{
    const T tmp = _clamp ? Math::clamp(in, _min, _max) : in;

    if (tmp == _value)
    {
        return;
    }

    _value = tmp;

    value_callback();
}

template<class T>
inline const T & Abstract_Value_Input<T>::get() const
{
    return _value;
}

template<class T>
inline void Abstract_Value_Input<T>::is_default_valid(bool in)
{
    _is_default_valid = in;
}

template<class T>
inline bool Abstract_Value_Input<T>::is_default_valid() const
{
    return _is_default_valid;
}

template<class T>
inline void Abstract_Value_Input<T>::default_value(T in)
{
    _default_value = in;

    is_default_valid(true);
}

template<class T>
inline const T & Abstract_Value_Input<T>::default_value() const
{
    return _default_value;
}

template<class T>
inline void Abstract_Value_Input<T>::range(T min, T max)
{
    if (min == _min && max == _max)
    {
        return;
    }

    _min = min;
    _max = max;

    set(_value);

    range_callback();
}

template<class T>
inline void Abstract_Value_Input<T>::min_action()
{
    set(_min);
}

template<class T>
inline void Abstract_Value_Input<T>::max_action()
{
    set(_max);
}

template<class T>
inline const T & Abstract_Value_Input<T>::min() const
{
    return _min;
}

template<class T>
inline const T & Abstract_Value_Input<T>::max() const
{
    return _max;
}

template<class T>
inline void Abstract_Value_Input<T>::inc(T inc_small, T inc_large)
{
    if (inc_small == _inc_small && inc_large == _inc_large)
    {
        return;
    }

    _inc_small = inc_small;
    _inc_large = inc_large;

    range_callback();
}

template<class T>
inline void Abstract_Value_Input<T>::inc_small_action()
{
    set(_value + _inc_small);
}

template<class T>
inline void Abstract_Value_Input<T>::inc_large_action()
{
    set(_value + _inc_large);
}

template<class T>
inline void Abstract_Value_Input<T>::dec_small_action()
{
    set(_value - _inc_small);
}

template<class T>
inline void Abstract_Value_Input<T>::dec_large_action()
{
    set(_value - _inc_large);
}

template<class T>
inline const T & Abstract_Value_Input<T>::inc_small() const
{
    return _inc_small;
}

template<class T>
inline const T & Abstract_Value_Input<T>::inc_large() const
{
    return _inc_large;
}

template<class T>
inline void Abstract_Value_Input<T>::clamp(bool in)
{
    if (in == _clamp)
    {
        return;
    }

    _clamp = in;

    set(_value);
}

template<class T>
inline bool Abstract_Value_Input<T>::clamp() const
{
    return _clamp;
}

template<class T>
inline int Abstract_Value_Input<T>::handle(int in, bool keyboard_focus)
{
    switch (in)
    {
        case FL_KEYBOARD:

            if (keyboard_focus)
            {
                switch (Fl::event_key())
                {
                    case FL_Up:
                        inc_small_action();
                        return 1;

                    case FL_Down:
                        dec_small_action();
                        return 1;

                    case FL_Page_Up:
                        inc_large_action();
                        return 1;

                    case FL_Page_Down:
                        dec_large_action();
                        return 1;
                }
            }

            break;
    }

    return 0;
}

} // djv

