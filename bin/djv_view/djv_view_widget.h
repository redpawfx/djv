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

//! \file djv_view_widget.h

#ifndef DJV_VIEW_WIDGET_H
#define DJV_VIEW_WIDGET_H

#include <djv_int_slider.h>

#include <djv_time.h>

namespace djv
{

class Float_Edit;
class Popup_Menu;
class Text_Edit;
class Text_Display;

} // djv

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Frame_Widget
//!
//! This class provides a widget for editing frame numbers.
//------------------------------------------------------------------------------

class Frame_Widget : public Widget
{
public:

    //! Constructor.

    Frame_Widget();

    //! Set the frame.

    void set(int64_t);

    //! Get the frame.

    int64_t get() const;

    //! Set the frame list.

    void list(const List<int64_t> &);

    //! Get the frame list.

    const List<int64_t> & list() const;

    //! Set the speed.

    void speed(const Speed &);

    //! Get the speed.

    const Speed & speed() const;

    //! This signal is emitted when the frame is changed.

    Signal<int64_t> signal;

    virtual void text_font(Fl_Font);

    virtual void text_color(Fl_Color);

    virtual Fl_Font text_font() const;

    virtual Fl_Color text_color() const;

    virtual void dirty();

    virtual Layout_Item * group_align();

private:

    void value_callback();

    DJV_CALLBACK(Frame_Widget, widget_callback, const String &);
    DJV_CALLBACK(Frame_Widget, inc_callback, bool);
    DJV_CALLBACK(Frame_Widget, dec_callback, bool);
    DJV_CALLBACK(Frame_Widget, push_callback, bool);

    void widget_update();

    int64_t       _value;
    List<int64_t> _list;
    Speed   _speed;
    Text_Edit *   _widget;
};

//------------------------------------------------------------------------------
//! \class Frame_Slider
//!
//! This class provides a slider widget for frame numbers.
//------------------------------------------------------------------------------

class Frame_Slider : public Int_Slider
{
public:

    //! Constructor.

    Frame_Slider();

    //! Set whether in/out points are enabled.

    void in_out(bool);

    //! Set in/out points.

    void in_out(int64_t, int64_t);

    //! Get whether in/out points are enabled.

    bool in_out() const;

    //! Get the in point.

    int64_t in() const;

    //! Get the out point.

    int64_t out() const;

    //! Go to the minimum frame.

    void min_action();

    //! Go to the maximum frame.

    void max_action();

    //! Set cached frames.

    void cached_frames(const List<int64_t> &);

    //! This signal is emitted when the slider is clicked.

    Signal<bool> push_signal;

    virtual int handle(int);

    virtual void draw();

private:

    bool          _in_out;
    int64_t       _in;
    int64_t       _out;
    List<int64_t> _cached_frames;
};

//------------------------------------------------------------------------------
//! \class Frame_Display_Widget
//!
//! This class provides a widget for displaying frame numbers.
//------------------------------------------------------------------------------

class Frame_Display_Widget : public Widget
{
public:

    //! Constructor.

    Frame_Display_Widget();

    //! Set the frame.

    void set(int64_t);

    //! Get the frame.

    int64_t get() const;

    //! Set the speed.

    void speed(const Speed &);

    //! Get the speed.

    const Speed & speed() const;

    virtual void text_font(Fl_Font);

    virtual void text_color(Fl_Color);

    virtual Fl_Font text_font() const;

    virtual Fl_Color text_color() const;

    virtual void dirty();

private:

    void widget_update();

    int64_t        _value;
    Speed    _speed;
    Text_Display * _widget;
};

//------------------------------------------------------------------------------
//! \class Speed_Widget
//!
//! This class provides a widget for editing speed values.
//------------------------------------------------------------------------------

class Speed_Widget : public Widget
{
public:

    //! Constructor.

    Speed_Widget();

    //! Set the speed.

    void set(const Speed &);

    //! Set the default speed.

    void set_default(const Speed &);

    //! Get the speed.

    const Speed & get() const;

    //! Get the default speed.

    const Speed & get_default() const;

    //! This signal is emitted when the speed is changed.

    Signal<const Speed &> signal;

    virtual void dirty();

private:

    DJV_CALLBACK(Speed_Widget, widget_callback, double);
    DJV_CALLBACK(Speed_Widget, menu_callback, int);

    void widget_update();
    void menu_update();

    Speed    _value;
    Speed    _value_default;
    //Text_Display * _widget;
    Float_Edit *   _widget;
    Popup_Menu *   _menu;
};

//------------------------------------------------------------------------------
//! \class Speed_Display_Widget
//!
//! This class provides a widget for displaying speed values.
//------------------------------------------------------------------------------

class Speed_Display_Widget : public Widget
{
public:

    //! Constructor.

    Speed_Display_Widget();

    //! Set the speed.

    void set(double);

    //! Set whether frames were dropped.

    void dropped_frames(bool);

    virtual void dirty();

private:

    void widget_update();

    double         _value;
    bool           _dropped_frames;
    Text_Display * _widget;
};

} // djv_view

#endif // DJV_VIEW_WIDGET_H

