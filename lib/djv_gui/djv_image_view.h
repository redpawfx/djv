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

//! \file djv_image_view.h

#ifndef DJV_IMAGE_VIEW_H
#define DJV_IMAGE_VIEW_H

#include <djv_gl_widget.h>

#include <djv_gl_image.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Image_View
//!
//! This class provides an image view widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Image_View : public Gl_Widget
{
public:

    //! Constructor.

    Image_View();

    //! Destructor.

    virtual ~Image_View();

    //! Set pixel data.

    virtual void set(const Pixel_Data *);

    //! Get pixel data.

    const Pixel_Data * get() const;

    //! Set image options.

    void options(const Gl_Image_Options &);

    //! Get image options.

    const Gl_Image_Options & options() const;

    //! Set the view position.

    virtual void view(const V2i &);

    //! Get the view position.

    const V2i & view() const;

    //! Set the view zoom.

    virtual void zoom(double);

    //! Set the view zoom.

    virtual void zoom(double, const V2i & focus);

    //! Get the view zoom.

    double zoom() const;

    //! Reset the view position.

    virtual void view_zero();

    //! Center the view position.

    virtual void view_center();

    //! Adjust the zoom to fit the view.

    virtual void view_fit();

    //! This signal is emitted when the view is changed.

    Signal<bool> signal;

    //! Get the bounding box.

    Box2f bbox() const;

    virtual void draw();

private:

    const Pixel_Data * _data;
    Gl_Image_Options   _options;
    V2i                _view;
    double             _zoom;
    Gl_Image_State     _state;
};

} // djv

#endif

