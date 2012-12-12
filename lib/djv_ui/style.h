/*------------------------------------------------------------------------------
 lib/djv_ui/style.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_STYLE_H
#define DJV_UI_STYLE_H

#include <djv_ui/style_def.h>

#include <djv_image/color.h>
#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::style
//
//! Style options.
//------------------------------------------------------------------------------

namespace style {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Colors.

enum COLOR
{
  COLOR_DARK,
  COLOR_NEUTRAL,
  COLOR_LIGHT,
  COLOR_CUSTOM,
  
  _COLOR_SIZE
};

_DJV_UI extern const Fl_Color FL_COLOR_BUTTON;
_DJV_UI extern const Fl_Color FL_COLOR_HIGHLIGHT;
_DJV_UI extern const Fl_Color FL_COLOR_BORDER;

//! Color values.

struct _DJV_UI Color
{
  Color();
  Color(
    const djv_image::Color & foreground,
    const djv_image::Color & background,
    const djv_image::Color & background2,
    const djv_image::Color & button,
    const djv_image::Color & select,
    const djv_image::Color & highlight
  );

  djv_image::Color foreground;
  djv_image::Color background;
  djv_image::Color background2;
  djv_image::Color button;
  djv_image::Color select;
  djv_image::Color highlight;
};

//! Size.

enum SIZE
{
  SIZE_SMALL,
  SIZE_MEDIUM,
  SIZE_LARGE,
  SIZE_CUSTOM,
  
  _SIZE_SIZE
};

//! Boxes.

_DJV_UI extern const Fl_Boxtype FL_BOX_UP;
_DJV_UI extern const Fl_Boxtype FL_BOX_UP_H1;
_DJV_UI extern const Fl_Boxtype FL_BOX_UP_H2;
_DJV_UI extern const Fl_Boxtype FL_BOX_UP_H3;
_DJV_UI extern const Fl_Boxtype FL_BOX_UP_V1;
_DJV_UI extern const Fl_Boxtype FL_BOX_UP_V2;
_DJV_UI extern const Fl_Boxtype FL_BOX_UP_V3;

_DJV_UI extern const Fl_Boxtype FL_BOX_DOWN;
_DJV_UI extern const Fl_Boxtype FL_BOX_DOWN_H1;
_DJV_UI extern const Fl_Boxtype FL_BOX_DOWN_H2;
_DJV_UI extern const Fl_Boxtype FL_BOX_DOWN_H3;
_DJV_UI extern const Fl_Boxtype FL_BOX_DOWN_V1;
_DJV_UI extern const Fl_Boxtype FL_BOX_DOWN_V2;
_DJV_UI extern const Fl_Boxtype FL_BOX_DOWN_V3;

_DJV_UI extern const Fl_Boxtype FL_BOX_THIN_UP;
_DJV_UI extern const Fl_Boxtype FL_BOX_THIN_DOWN;

_DJV_UI extern const Fl_Boxtype FL_BOX_BORDER;

_DJV_UI extern const Fl_Boxtype FL_BOX_MENU;

//@}
//------------------------------------------------------------------------------
//!\class Style
//
//! Style options.
//------------------------------------------------------------------------------

class _DJV_UI Style
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Style();

  //@}
  //----------------------------------------------------------------------------
  //!\name Color Options
  //@{

  void color(COLOR);
  void color_value(COLOR, const Color &);
  void color_value(const Color &);
  
  COLOR color() const;
  const Color & color_value(COLOR) const;
  const Color & color_value() const;
  
  // XXX
  
  void color_update();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Size Options
  //@{

  void size(SIZE);
  void size_value_(SIZE, int);
  void size_value(int);
  
  SIZE size() const;
  int size_value_(SIZE) const;
  int size_value() const;
  int size_pick() const;
  int size_handle() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Font Options
  //@{

  Fl_Font font() const;
  Fl_Font font_bold() const;
  int font_size() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout Options
  //@{

  int spacing() const;
  int spacing_large() const;
  
  int margin() const;
  int margin_text() const;
  int margin_widget() const;
  int margin_large() const;

  int size_icon() const;
  int size_tool_icon() const;
  int size_button() const;
  int size_text() const;
  int size_swatch() const;
  int size_thumbnail() const;
  
  //@}
  
private:
  
  void color_update(bool redraw);
  void size_update();
  void box_update();
  
  COLOR _color;
  Color _color_value [_COLOR_SIZE];
  
  SIZE _size;
  int _size_value [_SIZE_SIZE];
};

//------------------------------------------------------------------------------
//!\class Prefs
//
//! Style preferences.
//------------------------------------------------------------------------------

class _DJV_UI Prefs
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs();
  
  ~Prefs();

  //@}
  //----------------------------------------------------------------------------
  //!\name Color Options
  //@{
  
  void color(COLOR);
  void color_value(COLOR, const Color &);
  void color_value(const Color &);
  void focus(bool);
  void highlight(bool);
  
  COLOR color() const;
  const Color & color_value(COLOR) const;
  const Color & color_value() const;
  bool focus() const;
  bool highlight() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Size Options
  //@{
  
  void size(SIZE);
  void size_value_(SIZE, int);
  void size_value(int);
  
  SIZE size() const;
  int size_value_(SIZE) const;
  int size_value() const;
  
  //@}
  
private:

  COLOR _color;
  Color _color_value [_COLOR_SIZE];
  bool _focus;
  bool _highlight;
  
  SIZE _size;
  int _size_value [_SIZE_SIZE];
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

djv_image::Color fltk_to_color(Fl_Color);

void color_dark(Fl_Color);
void color_light(Fl_Color);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_UI bool operator == (const Color &, const Color &);

_DJV_UI bool operator != (const Color &, const Color &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_UI String & operator >> (String &, COLOR &) throw (String);
_DJV_UI String & operator >> (String &, Color &) throw (String);
_DJV_UI String & operator >> (String &, SIZE &) throw (String);

_DJV_UI String & operator << (String &, COLOR);
_DJV_UI String & operator << (String &, const Color &);
_DJV_UI String & operator << (String &, SIZE);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI const List<String> & label_color();
_DJV_UI const List<String> & label_size();

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to global options.

_DJV_UI Style * global();

//! Access to preferences.

_DJV_UI Prefs * prefs();

//@}
//------------------------------------------------------------------------------

}
}

#endif

