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

//! \file djv_progress_dialog.cpp

#include <djv_progress_dialog.h>

#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_push_button.h>
#include <djv_style.h>

#include <djv_time.h>

#include <FL/Fl.H>
#include <FL/Fl_Progress.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Progress_Dialog::Widget
//------------------------------------------------------------------------------

class Progress_Dialog::Widget : public Fl_Progress, public Abstract_Widget
{
public:

    Widget() : Fl_Progress(0, 0, 0, 0)
    {
        selection_color(FL_SELECTION_COLOR);

        Abstract_Widget::size(STRETCH, FIXED);
    }

    int handle(int in)
    {
        switch (Abstract_Widget::handle(in))
        {
            case -1:
                return 0;

            case 1:
                return 1;
        }

        return Fl_Progress::handle(in);
    }

    void show()
    {
        Fl_Progress::show();
        Abstract_Widget::show();
    }

    void draw()
    {
        Fl_Progress::draw();
        Abstract_Widget::draw();
    }

    const Fl_Widget * fltk_widget() const
    {
        return this;
    }

    void dirty()
    {
        Abstract_Widget::dirty();

        fl_font(text_font(), Style::global()->font_size());

        size_hint(
            V2i(100, fl_height()) + Style::global()->margin_widget() * 2);
    }

    void geom(const Box2i & in)
    {
        Fl_Progress::resize(in.x, in.y, in.w, in.h);
        Layout_Item::geom(in);
    }

    const Box2i & geom() const
    {
        return Abstract_Widget::geom();
    }

    void resize(int x, int y, int w, int h)
    {
        geom(Box2i(x, y, w, h));
    }

    int visible() const
    {
        return Fl_Progress::visible();
    }
};

//------------------------------------------------------------------------------
// Progress_Dialog
//------------------------------------------------------------------------------

namespace
{

const String
    label_progress = "Progress Dialog",
    label_progress_estimate = "Estimated: %% (%% Frames/Second)",
    label_progress_elapsed = "Elapsed: %%",
    label_cancel = "&Cancel";

} // namespace

Progress_Dialog::Progress_Dialog() :
    Abstract_Message_Dialog(label_progress),
    signal         (this),
    del_signal     (this),
    _value         (0),
    _current       (0),
    _widget        (0),
    _label         (0),
    _estimate_label(0),
    _elapsed_label (0)
{
    // Create widgets.

    _widget = new Widget;

    _label = new Label;

    _estimate_label = new Label;
    _elapsed_label = new Label;

    Push_Button * cancel_button = new Push_Button(label_cancel);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout->stretch(layout_h);
    layout_h->margin(Style::global()->margin_large());

    Vertical_Layout * layout_v = new Vertical_Layout(layout_h);
    layout_v->add(_label);
    layout_v->add(_widget);
    layout_v->add(_estimate_label);
    layout_v->add(_elapsed_label);

    layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add_stretch();
    layout_h->add(cancel_button);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    //set_modal();

    size(size_hint(), false);

    // Callbacks.

    cancel_button->signal.set(this, cancel_callback);
    close_signal.set(this, cancel_callback);
}

Progress_Dialog::~Progress_Dialog()
{}

void Progress_Dialog::del()
{
    del_signal.emit(false);

    Abstract_Message_Dialog::del();

    Fl::remove_idle(idle_callback, this);
}

void Progress_Dialog::set(int in)
{
    _value = in;
    _current = 0;
    _time.start();
    _time_accum = 0.0;
    _elapsed.start();

    _widget->maximum(static_cast<float>(_value));
    _widget->value(static_cast<float>(_current));

    Fl::add_idle(idle_callback, this);
}

void Progress_Dialog::label(const String & in)
{
    _label->set(in);
}

const String & Progress_Dialog::label() const
{
    return _label->get();
}

namespace
{

Progress_Dialog * _dialog = 0;

} // namespace

void Progress_Dialog::dialog(
    const String &               label,
    int                          value,
    Callback *                   in,
    Signal<int>::Callback_Fnc *  callback,
    Signal<bool>::Callback_Fnc * del_callback)
{
    if (! _dialog)
    {
        _dialog = new Progress_Dialog;
    }

    _dialog->signal.del();
    _dialog->del_signal.del();

    _dialog->set(value);
    _dialog->label(label);

    _dialog->signal.set(in, callback);
    _dialog->del_signal.set(in, del_callback);

    _dialog->show();
}

void Progress_Dialog::dialog_del(Callback * in)
{
    if (! _dialog)
    {
        _dialog = new Progress_Dialog;
    }

    _dialog->signal.del(in);
    _dialog->hide();
}

void Progress_Dialog::cancel_callback(bool)
{
    Fl::remove_idle(idle_callback, this);

    del_signal.emit(false);

    hide();
}

void Progress_Dialog::idle_callback()
{
    if (_current >= _value)
    {
        hide();

        Fl::remove_idle(idle_callback, this);

        del_signal.emit(true);
    }
    else
    {
        signal.emit(_current);
    }

    _widget->value(static_cast<float>(_current));

    _time.check();
    _elapsed.check();

    _time_accum += _time.seconds();
    _time.start();
    
    const double estimate =
        _time_accum /
        static_cast<double>(_current + 1) *
        (_value - (_current + 1));

    _estimate_label->set(
        String_Format(label_progress_estimate).
        arg(Time::label_time(estimate)).
        arg(_current / _elapsed.seconds(), 0, 2));
    _elapsed_label->set(
        String_Format(label_progress_elapsed).
        arg(Time::label_time(_elapsed.seconds())));

    ++_current;
}

} // djv
