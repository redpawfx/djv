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

//! \file djv_view_view_prefs.h

#ifndef DJV_VIEW_VIEW_PREFS_H
#define DJV_VIEW_VIEW_PREFS_H

#include <djv_view_view.h>

#include <djv_widget.h>

#include <djv_color.h>

namespace djv
{

class Check_Button_Group;
class Color_Swatch_Widget;

} // djv

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class View_Prefs
//!
//! This class provides view preferences.
//------------------------------------------------------------------------------

class View_Prefs : public Callback
{
public:

    //! Constructor.

    View_Prefs();

    //! Destructor.

    virtual ~View_Prefs();

    //! Set the background color.

    void background(const Color &);

    //! Get the background color.

    const Color & background() const;

    //! Set the view resize behavior.

    void resize(View::RESIZE);

    //! Get the view resize behavior.

    View::RESIZE resize() const;

    //! This signal is emitted when the view size behavior is changed.

    Signal<View::RESIZE> resize_signal;

    //! Set the view grid.

    void grid(View::GRID);

    //! Set the view grid color.

    void grid_color(const Color &);

    //! Get the view grid.

    View::GRID grid() const;

    //! Get the view grid color.

    const Color & grid_color() const;

    //! This signal is emitted when the view grid is changed.

    Signal<View::GRID> grid_signal;

    //! Set whether the HUD is enabled.

    void hud(bool);

    //! Set the HUD visibility.

    void hud_show(View::HUD_SHOW, bool);

    //! Set the HUD color.

    void hud_color(const Color &);

    //! Set the HUD background.

    void hud_background(View::HUD_BACKGROUND);

    //! Set the HUD background color.

    void hud_background_color(const Color &);

    //! Get whether the HUD is enabled.

    bool hud() const;

    //! Get the HUD visibility.

    List<bool> hud_show() const;

    //! Get the HUD visibility.

    bool hud_show(View::HUD_SHOW in) const;

    //! Get the HUD color.

    const Color & hud_color() const;

    //! Get the HUD background.

    View::HUD_BACKGROUND hud_background() const;

    //! Get the HUD background color.

    const Color & hud_background_color() const;

    //! This signal is emitted when the HUD is changed.

    Signal<bool> hud_signal;

    //! Get the global preferences.

    static View_Prefs * global();

private:

    Color                _background;
    View::RESIZE         _resize;
    View::GRID           _grid;
    Color                _grid_color;
    bool                 _hud;
    List<bool>           _hud_show;
    Color                _hud_color;
    View::HUD_BACKGROUND _hud_background;
    Color                _hud_background_color;
};

//------------------------------------------------------------------------------
//! \class View_Prefs_Widget
//!
//! This class provides a view preferences widget.
//------------------------------------------------------------------------------

class View_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    View_Prefs_Widget();

    //! Destructor.

    virtual ~View_Prefs_Widget();

private:

    DJV_CALLBACK(View_Prefs_Widget, background_callback, const Color &);
    DJV_CALLBACK(View_Prefs_Widget, resize_callback, int);
    DJV_CALLBACK(View_Prefs_Widget, grid_callback, int);
    DJV_CALLBACK(View_Prefs_Widget, grid_callback, const Color &);
    DJV_CALLBACK(View_Prefs_Widget, hud_callback, bool);
    DJV_CALLBACK(View_Prefs_Widget, hud_show_callback, const List<bool> &);
    DJV_CALLBACK(View_Prefs_Widget, hud_color_callback, const Color &);
    DJV_CALLBACK(View_Prefs_Widget, hud_background_callback, int);
    DJV_CALLBACK(View_Prefs_Widget,
        hud_background_color_callback, const Color &);

    Color_Swatch_Widget * _background_color_widget;
    Color_Swatch_Widget * _grid_color_widget;
    Check_Button_Group *  _hud_show_widget;
    Color_Swatch_Widget * _hud_color_widget;
    Color_Swatch_Widget * _hud_background_color_widget;
};

} // djv_view

#endif // DJV_VIEW_VIEW_PREFS_H

