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

//! \file djv_view_view_group.h

#ifndef DJV_VIEW_VIEW_GROUP_H
#define DJV_VIEW_VIEW_GROUP_H

#include <djv_view_image.h>
#include <djv_view_view.h>

#include <djv_callback.h>
#include <djv_row_layout.h>
#include <djv_menu.h>

namespace djv
{

class Label;
class Menu;
class Tool_Button;

} // djv

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class View_Group
//!
//! This class provides the view group.
//------------------------------------------------------------------------------

class View_Group : public Callback
{
public:

    //! Constructor.

    View_Group(
        Menu *                menu,
        Abstract_Row_Layout * toolbar,
        const View_Group *    copy    = 0);

    //! Destructor.

    virtual ~View_Group();

    //! Set the zoom.

    void zoom(double);

    //! Set the grid.

    void grid(View::GRID);

    //! Get the grid.

    View::GRID grid() const;

    //! Set whether the HUD is enabled.

    void hud(bool);

    //! Get the HUD.

    bool hud() const;

    //! Update the menu items.

    void menu_update(Menu_Item_Group *);

    //! Update the toolbar.

    void toolbar_update();

    //! This signal is emitted to move the view.

    Signal<const V2i &> move_signal;

    //! This signal is emited to center the view.

    Signal<bool> center_signal;

    //! This signal is emitted to zoom the view.

    Signal<double> zoom_signal;

    //! This signal is emitted to reset the view zoom.

    Signal<bool> zoom_reset_signal;

    //! This signal is emitted to reset the view.

    Signal<bool> reset_signal;

    //! This signal is emitted to fit the view.

    Signal<bool> fit_signal;

    //! This signal is emitted to update the overlay.

    Signal<bool> overlay_signal;

    //! This signal is emitted to update the view.

    Signal<bool> update_signal;

private:

    DJV_CALLBACK(View_Group, zoom_in_callback, bool);
    DJV_CALLBACK(View_Group, zoom_out_callback, bool);
    DJV_CALLBACK(View_Group, zoom_inc_callback, bool);
    DJV_CALLBACK(View_Group, zoom_reset_callback, bool);
    DJV_CALLBACK(View_Group, zoom_dec_callback, bool);
    DJV_CALLBACK(View_Group, fit_callback, bool);
    DJV_CALLBACK(View_Group, grid_callback, View::GRID);
    DJV_CALLBACK(View_Group, hud_callback, bool);

    DJV_FL_WIDGET_CALLBACK(View_Group, _left_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _right_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _up_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _down_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _center_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _zoom_in_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _zoom_out_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _zoom_reset_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _reset_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _fit_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _grid_callback);
    DJV_FL_WIDGET_CALLBACK(View_Group, _hud_callback);

    View::GRID    _grid;
    bool          _hud;
    Menu *        _menu;
    List<int>     _menu_grid;
    Tool_Button * _zoom_in_widget;
    Tool_Button * _zoom_out_widget;
    Tool_Button * _zoom_reset_widget;
    Tool_Button * _fit_widget;
    Label *       _zoom_label;
};

} // djv_view

#endif // DJV_VIEW_VIEW_GROUP_H

