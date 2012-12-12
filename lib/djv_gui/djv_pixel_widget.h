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

//! \file djv_pixel_widget.h

#ifndef DJV_PIXEL_WIDGET_H
#define DJV_PIXEL_WIDGET_H

#include <djv_widget.h>

#include <djv_pixel.h>

namespace djv
{

class Choice_Widget;

//------------------------------------------------------------------------------
//! \class Pixel_Widget
//!
//! This class provides a pixel widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Pixel_Widget : public Widget
{
public:

    //! Constructor.

    Pixel_Widget();

    //! Destructor.

    virtual ~Pixel_Widget();

    virtual void del();

    //! Set the pixel.

    void set(Pixel::PIXEL);

    //! Get the pixel.

    Pixel::PIXEL get() const;

    //! This signal is emitted when the pixel is changed.

    Signal<Pixel::PIXEL> signal;

    virtual void draw();

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

private:

    DJV_CALLBACK(Pixel_Widget, widget_callback, int);

    void widget_update();

    Pixel::PIXEL    _value;
    Choice_Widget * _widget;
};

} // djv

#endif // DJV_PIXEL_WIDGET_H
