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

//! \file djv_style.h

#ifndef DJV_STYLE_H
#define DJV_STYLE_H

#include <djv_gui_export.h>

#include <djv_color.h>

#include <FL/Enumerations.H>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Style
//!
//! This class provides the user interface style.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Style
{
public:

    //! Constructor.

    Style();

    //! Colors.

    enum COLOR
    {
        COLOR_DARK,
        COLOR_NEUTRAL,
        COLOR_LIGHT,
        COLOR_CUSTOM,

        _COLOR_SIZE
    };

    //! Get the color labels.

    static const List<String> & label_color();

    //! This struct provides color values.

    struct DJV_GUI_EXPORT Color
    {
        //! Constructor.

        Color();

        //! Constructor.

        Color(
            const djv::Color & foreground,
            const djv::Color & background,
            const djv::Color & background2,
            const djv::Color & button,
            const djv::Color & select,
            const djv::Color & highlight);

        djv::Color foreground;
        djv::Color background;
        djv::Color background2;
        djv::Color button;
        djv::Color select;
        djv::Color highlight;

        bool operator == (const Color &) const;

        bool operator != (const Color &) const;
    };

    //! Set the color.

    void color(COLOR);

    //! Set the color values.

    void color_value(COLOR, const Color &);

    //! Set the current color values.

    void color_value(const Color &);

    //! Get the color.

    COLOR color() const;

    //! Get the color values.

    const Color & color_value(COLOR) const;

    //! Get thec current color values.

    const Color & color_value() const;

    //! \todo Is this still necessary?

    void color_update();

    //! Size.

    enum SIZE
    {
        SIZE_SMALL,
        SIZE_MEDIUM,
        SIZE_LARGE,
        SIZE_CUSTOM,

        _SIZE_SIZE
    };

    //! Get the size labels.

    static const List<String> & label_size();

    //! Set the size.

    void size(SIZE);

    //! Set the size value.

    void size_value_(SIZE, int);

    //! Set the current size value.

    void size_value(int);

    //! Get the size.

    SIZE size() const;

    //! Get the size value.

    int size_value_(SIZE) const;

    //! Get the current size value.

    int size_value() const;

    //! Get the pick size.

    int size_pick() const;

    //! Get the handle size.

    int size_handle() const;

    //! Get the font.

    Fl_Font font() const;

    //! Get the bold font.

    Fl_Font font_bold() const;

    //! Get the fixed width font.

    Fl_Font font_fixed() const;

    //! Get the font size.

    int font_size() const;

    //! Get the spacing.

    int spacing() const;

    //! Get the large spacing.

    int spacing_large() const;

    //! Get the margin.

    int margin() const;

    //! Get the text margin.

    int margin_text() const;

    //! Get the widget margin.

    int margin_widget() const;

    //! Get the large margin.

    int margin_large() const;

    //! Get the icon size.

    int size_icon() const;

    //! Get the tool icon size.

    int size_tool_icon() const;

    //! Get the button size.

    int size_button() const;

    //! Get the text size.

    int size_text() const;

    //! Get the swatch size.

    int size_swatch() const;

    //! Get the thumbnail size.

    int size_thumbnail() const;

    //! Convert a FLTK color.

    static djv::Color fltk_to_color(Fl_Color);

    //! Set the current FLTK color.

    static void color_dark(Fl_Color);

    //! Set the current FLTK color.

    static void color_light(Fl_Color);

    //! Get the global style.

    static Style * global();

private:

    void color_update(bool redraw);
    void size_update();
    void box_update();

    COLOR _color;
    Color _color_value [_COLOR_SIZE];
    SIZE  _size;
    int   _size_value [_SIZE_SIZE];
};

//------------------------------------------------------------------------------
//! \class Style_Prefs
//!
//! This class provides the style preferences.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Style_Prefs
{
public:

    //! Constructor.

    Style_Prefs();

    //! Destructor.

    ~Style_Prefs();

    //! Set the color.

    void color(Style::COLOR);

    //! Set the color values.

    void color_value(Style::COLOR, const Style::Color &);

    //! Set the current color values.

    void color_value(const Style::Color &);

    //! Get the color.

    Style::COLOR color() const;

    //! Get the color values.

    const Style::Color & color_value(Style::COLOR) const;

    //! Get the current color values.

    const Style::Color & color_value() const;

    //! Set whether the focus is shown.

    void focus(bool);

    //! Get whether the focus is shown.

    bool focus() const;

    //! Set whether the highlight is shown.

    void highlight(bool);

    //! Get whether the highlight is shown.

    bool highlight() const;

    //! Set the size.

    void size(Style::SIZE);

    //! Set the size value.

    void size_value_(Style::SIZE, int);

    //! Set the current size value.

    void size_value(int);

    //! Get the size.

    Style::SIZE size() const;

    //! Get the size value.

    int size_value_(Style::SIZE) const;

    //! Get the current size value.

    int size_value() const;

    //! Get the global preferences.

    static Style_Prefs * global();

private:

    Style::COLOR _color;
    Style::Color _color_value [Style::_COLOR_SIZE];
    bool         _focus;
    bool         _highlight;
    Style::SIZE  _size;
    int          _size_value [Style::_SIZE_SIZE];
};

//------------------------------------------------------------------------------

DJV_GUI_EXPORT String & operator >> (String &, Style::COLOR &) throw (String);
DJV_GUI_EXPORT String & operator >> (String &, Style::Color &) throw (String);
DJV_GUI_EXPORT String & operator >> (String &, Style::SIZE &) throw (String);

DJV_GUI_EXPORT String & operator << (String &, Style::COLOR);
DJV_GUI_EXPORT String & operator << (String &, const Style::Color &);
DJV_GUI_EXPORT String & operator << (String &, Style::SIZE);

DJV_GUI_EXPORT extern const Fl_Color FL_COLOR_BUTTON;
DJV_GUI_EXPORT extern const Fl_Color FL_COLOR_HIGHLIGHT;
DJV_GUI_EXPORT extern const Fl_Color FL_COLOR_BORDER;

DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_UP;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_UP_H1;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_UP_H2;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_UP_H3;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_UP_V1;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_UP_V2;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_UP_V3;

DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_DOWN;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_DOWN_H1;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_DOWN_H2;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_DOWN_H3;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_DOWN_V1;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_DOWN_V2;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_DOWN_V3;

DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_THIN_UP;
DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_THIN_DOWN;

DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_BORDER;

DJV_GUI_EXPORT extern const Fl_Boxtype FL_BOX_MENU;

} // djv

#endif // DJV_STYLE_H
