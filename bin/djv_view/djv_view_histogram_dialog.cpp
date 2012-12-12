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

//! \file djv_view_histogram_dialog.cpp

#include <djv_view_histogram_dialog.h>

#include <djv_view_view_widget.h>

#include <djv_application.h>
#include <djv_choice_widget.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_prefs.h>
#include <djv_push_button.h>
#include <djv_style.h>
#include <djv_text_display.h>

#include <FL/fl_draw.H>

namespace djv_view
{

//------------------------------------------------------------------------------
// Histogram_Widget
//------------------------------------------------------------------------------

class Histogram_Widget : public Frame
{
public:

    Histogram_Widget();

    void set(
        const Pixel_Data *,
        const Color & min,
        const Color & max);

    virtual void draw();

    virtual void dirty();

private:

    const Pixel_Data * _value;
    int                _value_max;
    Color              _min;
    Color              _max;
};

Histogram_Widget::Histogram_Widget() :
    _value(0),
    _value_max(0)
{
    style(STYLE_BORDER);
    color(FL_BLACK);
}

void Histogram_Widget::set(
    const Pixel_Data * in,
    const Color &      min,
    const Color &      max)
{
    _value = in;

    if (_value)
    {
        _value_max = 0;

        //! \todo Is this still necessary?

        const Pixel::U16_T * p =
            reinterpret_cast<const Pixel::U16_T *>(_value->data());

        const int w = in->w();
        const int channels = in->channels();

        for (int i = 0; i < w; ++i, p += channels)
            for (int c = 0; c < channels; ++c)
            {
                _value_max = Math::max(static_cast<int>(p[c]), _value_max);
            }
    }

    _min = min;
    _max = max;

    redraw();
}

void Histogram_Widget::draw()
{
    const Box2i geom = frame_geom();

    fl_color(FL_BACKGROUND2_COLOR);
    fl_rectf(geom.x, geom.y, geom.w, geom.h);

    if (_value)
    {
        fl_push_clip(geom.x, geom.y, geom.w, geom.h);

        //fl_color(fl_darker(FL_BACKGROUND2_COLOR));
        //for (int x = geom.x; x < geom.x + geom.w -1; x += 10)
        //  fl_line(geom.x + x, geom.y, geom.x + x, geom.y + geom.h - 1);

        //! \todo Is this still necessary?

        const Pixel::U16_T * p =
            reinterpret_cast<const Pixel::U16_T *>(_value->data());

        const uchar colors [][3] =
        {
            { 255, 0,   0 },
            { 0,   255, 0 },
            { 0,   0,   255 },
            { 255, 255, 255 }
        };

        const int w = _value->w();
        const int channels = Pixel::channels(_value->pixel());

        for (int i = 0; i < w; ++i, p += channels)
            for (int c = channels - 1; c >= 0; --c)
            {
                fl_color(colors[c][0], colors[c][1], colors[c][2]);
                fl_line(
                    geom.x + i,
                    geom.y + geom.h - 1,
                    geom.x + i,
                    geom.y + geom.h - 1 - static_cast<int>(
                        p[c] / static_cast<double>(_value_max) * (geom.h - 1)));
            }

        fl_pop_clip();
    }

    Frame::draw();
}

void Histogram_Widget::dirty()
{
    Frame::dirty();

    size_hint(V2i(_value ? _value->w() : 256, 200) + frame_size() * 2);
}

//------------------------------------------------------------------------------
// Histogram_Dialog
//------------------------------------------------------------------------------

namespace
{

const String
    label_size = "Size:",
    label_min = "Min:",
    label_max = "Max:",
    label_close = "Clos&e";

} // namespace

Histogram_Dialog::Histogram_Dialog() :
    Dialog("Histogram"),
    _view        (0),
    _size        (Gl_Image::HISTOGRAM_256),
    _widget      (0),
    _size_widget (0),
    _min_widget  (0),
    _max_widget  (0),
    _close_widget(0)
{
    // Create widgets.

    _widget = new Histogram_Widget;

    _size_widget = new Choice_Widget(Gl_Image::label_histogram());

    Label * size_label = new Label(label_size);

    _min_widget = new Text_Display;

    Label * min_label = new Label(label_min);

    _max_widget = new Text_Display;

    Label * max_label = new Label(label_max);

    _close_widget = new Push_Button(label_close);

    // Layout.

    Layout_Item::create_group(List<Layout_Item *>() << min_label << max_label);

    Vertical_Layout * layout = new Vertical_Layout(this);

    layout->add(_widget);
    layout->stretch(_widget);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add(min_label);
    layout_h->add(_min_widget);
    layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add(max_label);
    layout_h->add(_max_widget);

    layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add(size_label);
    layout_h->add(_size_widget);
    layout_h->add_spacer(-1, true);
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    // Preferences.

    Prefs prefs(Prefs::prefs(), "histogram_dialog");
    Prefs::get_(&prefs, "size", &_size);

    // Initialize.

    widget_update();

    size(size_hint());

    // Callbacks.

    _size_widget->signal.set(this, size_callback);
    _close_widget->signal.set(this, close_callback);
}

Histogram_Dialog::~Histogram_Dialog()
{}

void Histogram_Dialog::del()
{
    Dialog::del();

    Prefs prefs(Prefs::prefs(), "histogram_dialog");
    Prefs::set_(&prefs, "size", _size);
}

void Histogram_Dialog::set(const View_Widget * in)
{
    _view = in;

    widget_update();
}

void Histogram_Dialog::pick(const View_Widget * in)
{
    if (in == _view || ! shown() || ! visible())
    {
        return;
    }

    set(in);
}

void Histogram_Dialog::update(const View_Widget * in)
{
    if (in != _view || ! shown() || ! visible())
    {
        return;
    }

    widget_update();
}

void Histogram_Dialog::del(const View_Widget * in)
{
    if (in != _view)
    {
        return;
    }

    _view = 0;
}

void Histogram_Dialog::show()
{
    _close_widget->take_focus();

    Dialog::show();
}

Histogram_Dialog * Histogram_Dialog::global()
{
    static Histogram_Dialog * global = 0;

    if (! global)
    {
        global = new Histogram_Dialog;
    }

    return global;
}

void Histogram_Dialog::size_callback(int in)
{
    _size = static_cast<Gl_Image::HISTOGRAM>(in);

    widget_update();
}

void Histogram_Dialog::close_callback(bool)
{
    hide();
}

void Histogram_Dialog::widget_update()
{
    //DJV_DEBUG("Histogram_Dialog::widget_update");

    callbacks(false);

    if (const Pixel_Data * data = _view ? _view->get() : 0)
    {
        try
        {
            Pixel_Data tmp(Pixel_Data_Info(data->size(), data->pixel()));
            Gl_Image::copy(*data, tmp, _view->options());
            Gl_Image::histogram(tmp, &_histogram, _size, &_min, &_max);
        }
        catch (Error in)
        {
            DJV_APP->error(in);
        }
    }
    else
    {
        _histogram.zero();
        _min.zero();
        _max.zero();
    }

    _widget->set(&_histogram, _min, _max);
    _size_widget->set(_size);
    _min_widget->set(String_Util::label(_min));
    _max_widget->set(String_Util::label(_max));

    callbacks(true);


    Dialog::update();
}

} // djv_view


