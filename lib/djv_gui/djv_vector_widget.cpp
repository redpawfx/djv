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

//! \file djv_vector_widget.cpp

#include <djv_vector_widget.h>

#include <djv_row_layout.h>
#include <djv_float_edit.h>
#include <djv_int_edit.h>

namespace djv
{

//------------------------------------------------------------------------------
// V2i_Edit
//------------------------------------------------------------------------------

V2i_Edit::V2i_Edit() :
    signal  (this),
    _widget (0),
    _widget2(0)
{
    _widget = new Int_Edit;
    _widget2 = new Int_Edit;

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(0);
    layout->add(_widget);
    layout->add(_widget2);

    widget_update();

    _widget->signal.set(this, widget_callback);
    _widget2->signal.set(this, widget2_callback);
}

V2i_Edit::~V2i_Edit()
{}

void V2i_Edit::set(const V2i & in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    widget_update();

    signal.emit(_value);
}

const V2i & V2i_Edit::get() const
{
    return _value;
}

void V2i_Edit::widget_callback(int in)
{
    set(V2i(in, _value.y));
}

void V2i_Edit::widget2_callback(int in)
{
    set(V2i(_value.x, in));
}

void V2i_Edit::widget_update()
{
    callbacks(false);

    _widget->set(_value.x);
    _widget2->set(_value.y);

    callbacks(true);
}

//------------------------------------------------------------------------------
// V2f_Edit
//------------------------------------------------------------------------------

V2f_Edit::V2f_Edit() :
    signal  (this),
    _widget (0),
    _widget2(0)
{
    _widget = new Float_Edit;
    _widget2 = new Float_Edit;

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(0);
    layout->add(_widget);
    layout->add(_widget2);

    widget_update();

    _widget->signal.set(this, widget_callback);
    _widget2->signal.set(this, widget2_callback);
}

V2f_Edit::~V2f_Edit()
{}

void V2f_Edit::set(const V2f & in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    widget_update();

    signal.emit(_value);
}

const V2f & V2f_Edit::get() const
{
    return _value;
}

void V2f_Edit::widget_callback(double in)
{
    set(V2f(in, _value.y));
}

void V2f_Edit::widget2_callback(double in)
{
    set(V2f(_value.x, in));
}

void V2f_Edit::widget_update()
{
    callbacks(false);

    _widget->set(_value.x);
    _widget2->set(_value.y);

    callbacks(true);
}

} // djv
