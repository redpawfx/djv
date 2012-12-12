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

//! \file djv_view_tool_group.h

#ifndef DJV_VIEW_TOOL_GROUP_H
#define DJV_VIEW_TOOL_GROUP_H

#include <djv_callback.h>
#include <djv_row_layout.h>
#include <djv_menu.h>

namespace djv
{

class Menu;
class Tool_Button;

} // djv

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Tool_Group
//!
//! This class provides the tool group.
//------------------------------------------------------------------------------

class Tool_Group : public Callback
{
public:

    //! Constructor.

    Tool_Group(
        Menu *                menu,
        Abstract_Row_Layout * toolbar,
        const Tool_Group *    copy    = 0);

    //! Destructor.

    virtual ~Tool_Group();

    //! Show the magnify tool.

    Signal<bool> magnify_signal;

    //! Show the color picker tool.

    Signal<bool> color_picker_signal;

    //! Show the histogram tool.

    Signal<bool> histogram_signal;

    //! Show the information tool.

    Signal<bool> info_signal;

    //! Update the menu items.

    void menu_update(Menu_Item_Group *);

    //! Update the toolbar.

    void toolbar_update();

private:

    DJV_CALLBACK(Tool_Group, magnify_callback, bool);
    DJV_CALLBACK(Tool_Group, color_picker_callback, bool);
    DJV_CALLBACK(Tool_Group, histogram_callback, bool);
    DJV_CALLBACK(Tool_Group, info_callback, bool);

    DJV_FL_WIDGET_CALLBACK(Tool_Group, _magnify_callback);
    DJV_FL_WIDGET_CALLBACK(Tool_Group, _color_picker_callback);
    DJV_FL_WIDGET_CALLBACK(Tool_Group, _histogram_callback);
    DJV_FL_WIDGET_CALLBACK(Tool_Group, _info_callback);

    Menu *        _menu;
    Tool_Button * _magnify_widget;
    Tool_Button * _color_picker_widget;
    Tool_Button * _histogram_widget;
    Tool_Button * _info_widget;
};

} // djv_view

#endif // DJV_VIEW_TOOL_GROUP_H

