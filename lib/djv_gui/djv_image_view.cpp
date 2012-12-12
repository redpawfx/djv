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

//! \file djv_image_view.cpp

#include <djv_image_view.h>

#include <djv_application.h>

#include <djv_image.h>
#include <djv_matrix.h>

namespace djv
{

//------------------------------------------------------------------------------
// Image_View
//------------------------------------------------------------------------------

Image_View::Image_View() :
    signal(this),
    _data(0),
    _zoom(1.0)
{}

Image_View::~Image_View()
{}

void Image_View::set(const Pixel_Data * in)
{
    _data = in;
}

const Pixel_Data * Image_View::get() const
{
    return _data;
}

void Image_View::view(const V2i & in)
{
    if (in == _view)
    {
        return;
    }

    _view = in;

    signal.emit(true);
}

const V2i & Image_View::view() const
{
    return _view;
}

void Image_View::zoom(double in)
{
    if (in == _zoom)
    {
        return;
    }

    //DJV_DEBUG("Image_View::zoom");
    //DJV_DEBUG_PRINT("in = " << in);

    _zoom = in;

    signal.emit(true);
}

void Image_View::zoom(double in, const V2i & focus)
{
    if (in == _zoom)
    {
        return;
    }

    //DJV_DEBUG("Image_View::zoom");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("focus = " << focus);

    _view = focus + V2i(V2f(_view - focus) * (in / _zoom));
    _zoom = in;

    signal.emit(true);
}

double Image_View::zoom() const
{
    return _zoom;
}

void Image_View::view_zero()
{
    _view = V2i();
    _zoom = 1.0;

    signal.emit(true);
}

void Image_View::view_center()
{
    view(V2f(geom().size) / 2.0 - bbox().size / 2.0);
}

void Image_View::view_fit()
{
    if (! _data)
    {
        return;
    }

    //DJV_DEBUG("Image_View::view_fit");

    const Box2i & geom = this->geom();

    //DJV_DEBUG_PRINT("geom = " << geom.size);

    _view.zero();
    _zoom = 1.0;
    const Box2f bbox = this->bbox();

    //DJV_DEBUG_PRINT("bbox = " << bbox);

    _zoom = Vector_Util::is_size_valid(bbox.size) ?
            Math::min(geom.w / bbox.size.x, geom.h / bbox.size.y) : 1.0;

    //DJV_DEBUG_PRINT("zoom = " << _zoom);

    _view =
        V2f(geom.size) / 2.0 -
        bbox.size * _zoom / 2.0 -
        this->bbox().position;

    //DJV_DEBUG_PRINT("view = " << _view);

    signal.emit(true);
}

void Image_View::options(const Gl_Image_Options & in)
{
    _options = in;
}

const Gl_Image_Options & Image_View::options() const
{
    return _options;
}

Box2f Image_View::bbox() const
{
    if (! _data)
    {
        return Box2f();
    }

    const Pixel_Data_Info & info = _data->info();

    Gl_Image_Xform xform = _options.xform;
    xform.position += _view;
    xform.scale *= _zoom;

    return
        Gl_Image_Xform::xform_matrix(xform) *
        Box2f(_data->info().size * Pixel_Data::proxy_scale(info.proxy));
}

void Image_View::draw()
{
    Gl_Widget::draw();

    if (! visible())
    {
        return;
    }

    //DJV_DEBUG("Image_View::draw");
    //DJV_DEBUG_PRINT("valid = " << valid());
    //DJV_DEBUG_PRINT("view = " << _view);
    //DJV_DEBUG_PRINT("zoom = " << _zoom);

    const Box2i & geom = this->geom();

    //DJV_DEBUG_PRINT("geom = " << geom.size);

    //if (! valid())
    //{
    Gl_Util::ortho(V2i(geom.w, geom.h));
    glViewport(0, 0, geom.w, geom.h);
    //}

    Color background(Pixel::RGB_F32);
    Color::convert(_options.background, background);
    glClearColor(
        background.get_f32(0),
        background.get_f32(1),
        background.get_f32(2),
        0.0);
    //glClearColor(0, 1, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (! _data)
    {
        return;
    }

    //DJV_DEBUG_PRINT("data = " << *_data);

    Gl_Image_Options options = _options;
    options.xform.position += _view;
    options.xform.scale *= _zoom;

    try
    {
        Gl_Image::draw(*_data, options, &_state);
    }
    catch (Error in)
    {
        DJV_APP->error(in);
    }
}

} // djv
