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

//! \file djv_float_edit.h

#ifndef DJV_FLOAT_EDIT_H
#define DJV_FLOAT_EDIT_H

#include <djv_abstract_value_widget.h>

namespace djv
{

class Text_Edit;

//------------------------------------------------------------------------------
//! \class Float_Edit
//!
//! This class provides a floating-point edit widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Float_Edit :
    public Widget,
    public Abstract_Float_Input
{
public:

    //! Constructor.

    Float_Edit();

    //! Destructor.

    virtual ~Float_Edit();

    //! This signal is emitted when the value is changed.

    Signal<double> signal;

    virtual int handle(int);

    virtual Layout_Item * group_align();

private:

    void value_callback();
    DJV_CALLBACK(Float_Edit, widget_callback, const String &);
    DJV_CALLBACK(Float_Edit, inc_callback, bool);
    DJV_CALLBACK(Float_Edit, dec_callback, bool);

    void value_update();

    Text_Edit * _widget;
};

} // djv

#endif // DJV_FLOAT_EDIT_H

