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

//! \file djv_abstract_value_widget.h

#ifndef DJV_ABSTRACT_VALUE_WIDGET_H
#define DJV_ABSTRACT_VALUE_WIDGET_H

#include <djv_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Abstract_Value_Input
//!
//! This class provides the base functionality for numeric widgets.
//------------------------------------------------------------------------------

template<class T>
class Abstract_Value_Input
{
public:

    //! Constructor.

    inline Abstract_Value_Input(T min = T(), T max = T());

    //! Destructor.

    inline virtual ~Abstract_Value_Input() = 0;

    //! Set the value.

    inline virtual void set(T);

    //! Get the value.

    inline const T & get() const;

    //! Set the default value.

    inline virtual void default_value(T);

    //! Get the default value.

    inline const T & default_value() const;

    //! Set whether the default value is valid.

    inline virtual void is_default_valid(bool);

    //! Get whether the default value is valid.

    inline bool is_default_valid() const;

    //! Set the value range.

    inline virtual void range(T min, T max);

    //! Set whether clamping is enabled.

    inline virtual void clamp(bool);

    //! Set the value to the minimum.

    inline virtual void min_action();

    //! Set the value to the maximum.

    inline virtual void max_action();

    //! Get the minimum value.

    inline const T & min() const;

    //! Get the maximum value.

    inline const T & max() const;

    //! Get whether clamping is enabled.

    inline bool clamp() const;

    //! Set the value increment.

    inline virtual void inc(T inc_small, T inc_large);

    //! Increment the value.

    inline virtual void inc_small_action();

    //! Increment the value.

    inline virtual void inc_large_action();

    //! Decrement the value.

    inline virtual void dec_small_action();

    //! Decrement the value.

    inline virtual void dec_large_action();

    //! Get the small increment.

    inline const T & inc_small() const;

    //! Get the large increment.

    inline const T & inc_large() const;

    //! Change the value based on the FLTK event.

    inline int handle(int, bool keyboard_focus);

protected:

    virtual void value_callback() {}
    virtual void range_callback() {}

private:

    T    _value;
    bool _is_default_valid;
    T    _default_value;
    T    _min;
    T    _max;
    bool _clamp;
    T    _inc_small;
    T    _inc_large;
};

//------------------------------------------------------------------------------
//! \class Abstract_Int_Input
//!
//! This class provides the base functionality for integer widgets.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Abstract_Int_Input : public Abstract_Value_Input<int>
{
public:

    static const int int_min;
    static const int int_max;

    //! Constructor.

    Abstract_Int_Input(int min = int_min, int max = int_max);
};

//------------------------------------------------------------------------------
//! \class Abstract_Float_Input
//!
//! This class provides the base functioanlity for floating-point widgets.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Abstract_Float_Input : public Abstract_Value_Input<double>
{
public:

    static const double float_min;
    static const double float_max;

    //! Constructor.

    Abstract_Float_Input(double min = float_min, double max = float_max);

    //! Destructor.

    virtual ~Abstract_Float_Input() {}

    //! Set the precision.

    virtual void precision(int);

    //! Get the precision.

    virtual int precision() const;

private:

    int _precision;
};

} // djv

#include <djv_abstract_value_widget_inline.h>

#endif // DJV_ABSTRACT_VALUE_WIDGET_H

