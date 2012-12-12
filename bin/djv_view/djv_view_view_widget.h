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

//! \file djv_view_view_widget.h

#ifndef DJV_VIEW_VIEW_WIDGET_H
#define DJV_VIEW_VIEW_WIDGET_H

#include <djv_view_hud_info.h>
#include <djv_view_input.h>
#include <djv_view_view.h>

#include <djv_image_view.h>

namespace djv
{

class File;

} // djv

namespace djv_view
{

//------------------------------------------------------------------------------
//! \class View_Widget
//!
//! This class provides a view widget.
//------------------------------------------------------------------------------

class View_Widget : public Image_View
{
public:

    //! Constructor.

    View_Widget();

    virtual void del();

    //! Set the zoom.

    void zoom(double);

    //! Set the zoom.

    void zoom(double, const V2i & focus);

    //! Set the zoom.

    void zoom_focus(double);

    //! Get the zoom.

    double zoom() const;

    //! Set the grid.

    void grid(View::GRID);

    //! Set the grid color.

    void grid_color(const Color &);

    //! Set the HUD.

    void hud(bool);

    //! Set the HUD information.

    void hud_info(const Hud_Info &);

    //! Set the HUD visibility.

    void hud_show(const List<bool> &);

    //! Set the HUD color.

    void hud_color(const Color &);

    //! Set the HUD background.

    void hud_background(View::HUD_BACKGROUND);

    //! Set the HUD background color.

    void hud_background_color(const Color &);

    //! This signal is emitted when the view is picked.

    Signal<const V2i &> pick_signal;

    //! This signal is emitted when the mouse wheel is changed.

    Signal<Input::MOUSE_WHEEL> mouse_wheel_signal;

    //! This signal is emitted when the mouse wheel is changed.

    Signal<int> mouse_wheel_value_signal;

    //! This signal is emitted when a file is dropped on the view.

    Signal<const File &> file_signal;

    virtual int handle(int);

    virtual void draw();

    //! Get the mouse position.

    const V2i & mouse() const;

    virtual const V2i & size_hint() const;

    virtual const V2i & size_min() const;

private:

    void draw_grid();

    void draw_hud();

    static void draw_hud(
        const String &,
        const V2i & position,
        const Color & text_color,
        View::HUD_BACKGROUND,
        const Color & background_color);

    DJV_FL_CALLBACK(View_Widget, mouse_wheel_timeout_callback);

    V2i                  _view_tmp;
    double               _zoom_tmp;
    bool                 _mouse_wheel;
    int                  _mouse_wheel_tmp;
    View::GRID           _grid;
    Color                _grid_color;
    bool                 _hud;
    Hud_Info             _hud_info;
    List<bool>           _hud_show;
    Color                _hud_color;
    View::HUD_BACKGROUND _hud_background;
    Color                _hud_background_color;
    bool                 _inside;
    V2i                  _mouse;
    V2i                  _mouse_td;
    V2i                  _size_min;
};

} // djv_view

#endif // DJV_VIEW_VIEW_WIDGET_H

