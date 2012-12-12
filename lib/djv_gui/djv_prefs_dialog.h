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

//! \file djv_prefs_dialog.h

#ifndef DJV_PREFS_DIALOG_H
#define DJV_PREFS_DIALOG_H

#include <djv_dialog.h>
#include <djv_widget.h>

namespace djv
{

class Check_Button;
class Color_Swatch_Widget;
class Int_Edit;
class Push_Button;
class Radio_Button_Group;
class Tabs;

//------------------------------------------------------------------------------
//! \class Style_Prefs_Widget
//!
//! This class provides a style preferences widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Style_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    Style_Prefs_Widget();

private:

    DJV_CALLBACK(Style_Prefs_Widget, color_callback, int);
    DJV_CALLBACK(Style_Prefs_Widget, color_foreground_callback, const Color &);
    DJV_CALLBACK(Style_Prefs_Widget, color_background_callback, const Color &);
    DJV_CALLBACK(Style_Prefs_Widget, color_background2_callback, const Color &);
    DJV_CALLBACK(Style_Prefs_Widget, color_button_callback, const Color &);
    DJV_CALLBACK(Style_Prefs_Widget, color_select_callback, const Color &);
    DJV_CALLBACK(Style_Prefs_Widget, color_highlight_callback, const Color &);
    DJV_CALLBACK(Style_Prefs_Widget, focus_callback, bool);
    DJV_CALLBACK(Style_Prefs_Widget, highlight_callback, bool);
    DJV_CALLBACK(Style_Prefs_Widget, size_callback, int);
    DJV_CALLBACK(Style_Prefs_Widget, size_value_callback, int);

    void color_update();
    void size_update();

    Radio_Button_Group *  _color_widget;
    Color_Swatch_Widget * _color_foreground_widget;
    Color_Swatch_Widget * _color_background_widget;
    Color_Swatch_Widget * _color_background2_widget;
    Color_Swatch_Widget * _color_button_widget;
    Color_Swatch_Widget * _color_select_widget;
    Color_Swatch_Widget * _color_highlight_widget;
    Check_Button *        _focus_widget;
    Check_Button *        _highlight_widget;
    Radio_Button_Group *  _size_widget;
    Int_Edit *            _size_value_widget;
};

//------------------------------------------------------------------------------
//! \class General_Prefs_Widget
//!
//! This class provides a general preferences widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT General_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    General_Prefs_Widget();

private:

    DJV_CALLBACK(General_Prefs_Widget, time_units_callback, int);
    DJV_CALLBACK(General_Prefs_Widget, speed_callback, int);
    DJV_CALLBACK(General_Prefs_Widget, filter_min_callback, int);
    DJV_CALLBACK(General_Prefs_Widget, filter_mag_callback, int);
    DJV_CALLBACK(General_Prefs_Widget, help_tooltips_callback, bool);
};

//------------------------------------------------------------------------------
//! \class Prefs_Dialog
//!
//! This class provides a preferences dialog.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Prefs_Dialog : public Dialog
{
public:

    //! Constructor.

    Prefs_Dialog();

    //! Destructor.

    virtual ~Prefs_Dialog();

    //! Add a preferences widget.

    void add(const String & name, Abstract_Widget *);

    //! Add a shortcut preferences widet.

    void add_shortcut(const String & name, Abstract_Widget *);

    virtual void show();

    virtual void dirty();

    //! Get the global preferences dialog.

    static Prefs_Dialog * global();

private:

    DJV_CALLBACK(Prefs_Dialog, close_callback, bool);

    Tabs *        _tabs;
    Tabs *        _shortcut_tabs;
    Push_Button * _close_widget;
};

} // djv

#endif // DJV_PREFS_DIALOG_H
