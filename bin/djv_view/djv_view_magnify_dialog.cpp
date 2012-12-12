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

//! \file djv_view_magnify_dialog.cpp

#include <djv_view_magnify_dialog.h>

#include <djv_view_view_widget.h>

#include <djv_application.h>
#include <djv_choice_widget.h>
#include <djv_image_view.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_prefs.h>
#include <djv_push_button.h>
#include <djv_style.h>

#include <djv_gl_image.h>
#include <djv_gl_offscreen_buffer.h>

#include <FL/fl_draw.H>

namespace djv_view
{

//------------------------------------------------------------------------------
// Magnify_Widget
//------------------------------------------------------------------------------

class Magnify_Widget : public Frame
{
public:

    Magnify_Widget();

    virtual void del();

    void set(const View_Widget *, const V2i & pick, double zoom);

    virtual void draw();

    virtual void dirty();

    virtual void geom(const Box2i &);

private:

    Pixel_Data                         _data;
    std::auto_ptr<Gl_Offscreen_Buffer> _buffer;
    Gl_Image_State                     _state;
};

Magnify_Widget::Magnify_Widget()
{
    style(STYLE_BORDER);
    color(FL_BLACK);
}

void Magnify_Widget::del()
{
    Frame::del();

    _buffer.reset();
}

void Magnify_Widget::set(const View_Widget * in, const V2i & pick, double zoom)
{
    //DJV_DEBUG("Magnify_Widget::set");
    //DJV_DEBUG_PRINT("pick = " << pick);
    //DJV_DEBUG_PRINT("zoom = " << zoom);
    //DJV_DEBUG_PRINT("size = " << Frame::geom());

    if (const Pixel_Data * data = in->get())
    {
        //DJV_DEBUG_PRINT("data = " << *data);

        try
        {
            if (! _buffer.get())
            {
                _buffer = std::auto_ptr<Gl_Offscreen_Buffer>(
                    new Gl_Offscreen_Buffer(_data.info()));
            }
            
            Gl_Image_Options options = in->options();
            options.xform.position -= pick - ((_data.size() - 1) / 2);
            options.xform.scale *= zoom;

            Gl_Image::copy(
                *data,
                _data,
                options,
                &_state,
                _buffer.get());

            //! \todo Flip the image vertically. Normally we would use
            //! fl_draw_image() to do this but it appears to be broken on OS X.

            options = Gl_Image_Options();
            options.xform.mirror.y = true;

            Gl_Image::copy(_data, _data, options);
        }
        catch (Error in)
        {
            DJV_APP->error(in);
        }
    }
    else
    {
        _data.zero();
    }

    redraw();
}

void Magnify_Widget::draw()
{
    const Box2i & geom = frame_geom();

    const int w = _data.w();
    const int h = _data.h();
    const int c = _data.channels();

    fl_draw_image(
        _data.data(),
        geom.x,
        geom.y,
        w,
        h,
        c,
        static_cast<int>(_data.bytes_scanline()));

    Frame::draw();
}

void Magnify_Widget::dirty()
{
    Frame::dirty();

    size_hint(_data.size());
}

void Magnify_Widget::geom(const Box2i & in)
{
    Frame::geom(in);

    _data.set(Pixel_Data_Info(frame_geom().size, Pixel::RGB_U8));
    _data.zero();
    
    _buffer.reset();
}

//------------------------------------------------------------------------------
// Magnify_Dialog
//------------------------------------------------------------------------------

const List<String> & Magnify_Dialog::label_zoom()
{
    static const List<String> data = List<String>() <<
        "100%" <<
        "200%" <<
        "400%" <<
        "800%" <<
        "1600%";

    DJV_ASSERT(data.size() == _ZOOM_SIZE);

    return data;
}

namespace
{

const String
    label_close = "Clos&e";

} // namespace

Magnify_Dialog::Magnify_Dialog() :
    Dialog("Magnify"),
    _view        (0),
    _zoom        (ZOOM_200),
    _widget      (0),
    _zoom_widget (0),
    _close_widget(0)
{
    // Create widgets.

    _widget = new Magnify_Widget;

    _zoom_widget = new Choice_Widget(label_zoom());

    _close_widget = new Push_Button(label_close);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);

    layout->add(_widget);
    layout->stretch(_widget);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add(_zoom_widget);
    layout_h->add_spacer(-1, true);
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    // Preferences.

    Prefs prefs(Prefs::prefs(), "magnify_dialog");
    Prefs::get_(&prefs, "zoom", &_zoom);

    // Initialize.

    widget_update();

    size(Vector_Util::max(size_hint(), V2i(300, 300)));

    // Callbacks.

    _zoom_widget->signal.set(this, zoom_callback);

    _close_widget->signal.set(this, close_callback);
}

Magnify_Dialog::~Magnify_Dialog()
{}

void Magnify_Dialog::del()
{
    Dialog::del();

    Prefs prefs(Prefs::prefs(), "magnify_dialog");
    Prefs::set_(&prefs, "zoom", _zoom);
}

void Magnify_Dialog::set(const View_Widget * in, const V2i & pick)
{
    _view = in;
    _pick = pick;

    widget_update();
}

void Magnify_Dialog::pick(const View_Widget * in, const V2i & pick)
{
    if (! shown() || ! visible())
    {
        return;
    }

    set(in, pick);
}

void Magnify_Dialog::update(const View_Widget * in)
{
    if (in != _view || ! shown() || ! visible())
    {
        return;
    }

    widget_update();
}

void Magnify_Dialog::del(const View_Widget * in)
{
    if (in != _view)
    {
        return;
    }

    _view = 0;
}

void Magnify_Dialog::show()
{
    _close_widget->take_focus();

    Dialog::show();
}

Magnify_Dialog * Magnify_Dialog::global()
{
    static Magnify_Dialog * global = 0;

    if (! global)
    {
        global = new Magnify_Dialog;
    }

    return global;
}

void Magnify_Dialog::zoom_callback(int in)
{
    _zoom = static_cast<ZOOM>(in);

    widget_update();
}

void Magnify_Dialog::close_callback(bool)
{
    hide();
}

void Magnify_Dialog::widget_update()
{
    if (! _view)
    {
        return;
    }

    //DJV_DEBUG("Magnify_Dialog::widget_update");

    callbacks(false);

    const double zoom = Math::pow(2, _zoom);

    //DJV_DEBUG_PRINT("zoom = " << zoom);

    _widget->set(
        _view,
        Vector_Util::floor<double, int>(V2f(_pick) * zoom),
        _view->zoom() * zoom);

    _zoom_widget->set(_zoom);

    callbacks(true);
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(Magnify_Dialog::ZOOM, Magnify_Dialog::label_zoom())

} // djv_view

