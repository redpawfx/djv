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

//! \file djv_vector_widget.h

#ifndef DJV_VECTOR_WIDGET_H
#define DJV_VECTOR_WIDGET_H

#include <djv_widget.h>

namespace djv
{

class Float_Edit;
class Int_Edit;

//------------------------------------------------------------------------------
//! \class V2i_Edit
//!
//! This class provides a two-dimensional integer vector widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT V2i_Edit : public Widget
{
public:

    //! Constructor.

    V2i_Edit();

    //! Destructor.

    virtual ~V2i_Edit();

    //! Set the value.

    void set(const V2i &);

    //! Get the value.

    const V2i & get() const;

    //! This signal is emitted when the value is changed.

    Signal<const V2i &> signal;

private:

    DJV_CALLBACK(V2i_Edit, widget_callback, int);
    DJV_CALLBACK(V2i_Edit, widget2_callback, int);

    void widget_update();

    V2i        _value;
    V2i        _tmp;
    Int_Edit * _widget;
    Int_Edit * _widget2;
};

//------------------------------------------------------------------------------
//! \class V2f_Edit
//!
//! This class provides a two-dimensional floating-point vector widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT V2f_Edit : public Widget
{
public:

    //! Constructor.

    V2f_Edit();

    //! Destructor.

    virtual ~V2f_Edit();

    //! Set the value.

    void set(const V2f &);

    //! Get the value.

    const V2f & get() const;

    //! This signal is emitted when the value is changed.

    Signal<const V2f &> signal;

private:

    DJV_CALLBACK(V2f_Edit, widget_callback, double);
    DJV_CALLBACK(V2f_Edit, widget2_callback, double);

    void widget_update();

    V2f          _value;
    V2f          _tmp;
    Float_Edit * _widget;
    Float_Edit * _widget2;
};

} // djv

#endif // DJV_VECTOR_WIDGET_H

