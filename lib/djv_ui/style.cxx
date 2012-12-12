/*------------------------------------------------------------------------------
 lib/djv_ui/style.cxx

 Copyright (c) 2004-2009 Darby Johnston
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "style.h"

#include <djv_ui/global.h>
#include <FL/Fl.H>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace style {

//------------------------------------------------------------------------------
// Data
//------------------------------------------------------------------------------

namespace {

const int _spacing [] = { 4, 4, 4, 4 };
const int _spacing_large [] = { 16, 16, 16, 16 };

const int _margin [] = { 4, 4, 4, 4 };
const int _margin_text [] = { 2, 2, 2, 2 };
const int _margin_widget [] = { 4, 4, 4, 4 };
const int _margin_large [] = { 16, 16, 16, 16 };

const int _size_icon = 22;
const int _size_tool_icon = 11;

}

//------------------------------------------------------------------------------
// Color
//------------------------------------------------------------------------------

Color::Color()
{}

Color::Color(
  const djv_image::Color & foreground,
  const djv_image::Color & background,
  const djv_image::Color & background2,
  const djv_image::Color & button,
  const djv_image::Color & select,
  const djv_image::Color & highlight
) :
  foreground(foreground),
  background(background),
  background2(background2),
  button(button),
  select(select),
  highlight(highlight)
{}

//------------------------------------------------------------------------------
// Style
//------------------------------------------------------------------------------

namespace {

void box_draw(
  int x, int y, int w, int h,
  bool west, bool north, bool east, bool south,
  bool down,
  Fl_Color fltk_color
) {
  bool select = false;
  bool highlight = false;
  
  if (FL_SELECTION_COLOR == fltk_color)
  {
    select = true;
    fltk_color = FL_COLOR_BUTTON;
  }
  else if (FL_COLOR_HIGHLIGHT == fltk_color)
  {
    highlight = true;
    fltk_color = FL_COLOR_BUTTON;
  }
  
  const Color & color_value = global()->color_value();
  djv_image::Color color = down ?
    djv_image::color::scale(0.8, fltk_to_color(fltk_color)) :
    fltk_to_color(fltk_color);
  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  
  // Draw border.
  
  int x1 = x;
  int y1 = y;
  int x2 = x + w - 1;
  int y2 = y + h - 1;
  
  if (select && prefs()->focus())
    fl_color(FL_SELECTION_COLOR);
  else if (highlight && prefs()->highlight())
    fl_color(FL_COLOR_HIGHLIGHT);
  else
    fl_color(FL_COLOR_BORDER);
  
  if (west)
    fl_line(x1, y1 + north, x1, y2 - south);
  if (north)
    fl_line(x1 + west, y1, x2 - east, y1);
  if (east)
    fl_line(x2, y1 + north, x2, y2 - south);
  if (south)
    fl_line(x1 + west, y2, x2 - east, y2);
  
  fl_rectf(
    x + west,
    y + north,
    w - (west + east),
    h - (north + south)
  );
  
  // Draw background.
  
  djv_image::color::convert(color, tmp);
  fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  
  fl_rectf(
    x + west * 2,
    y + north * 2,
    w - (west + east) * 2,
    h - (north + south) * 2
  );
  
  // Draw west/north bevels.
  
  x1 += west;
  y1 += north;
  x2 -= east;
  y2 -= south;
  
  djv_image::color::scale(down ? 0.8 : 1.2, color, tmp);
  fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  
  if (west)
    fl_line(x1, y1 + north, x1, y2 - south);
  if (north)
    fl_line(x1 + west, y1, x2 - east, y1);
  
  // Draw east/south bevels.
  
  djv_image::color::scale(down ? 1.2 : 0.8, color, tmp);
  fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  
  if (east)
    fl_line(x2, y1 + north, x2, y2 - south);
  if (south)
    fl_line(x1 + west, y2, x2 - east, y2);
}

void box_up_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, true, true, true, false, fltk_color);
}

void box_up_h_start_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, true, false, true, false, fltk_color);
}

void box_up_h_middle_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, false, true, false, true, false, fltk_color);
}

void box_up_h_end_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, false, true, true, true, false, fltk_color);
}

void box_up_v_start_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, true, true, false, false, fltk_color);
}

void box_up_v_middle_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, false, true, false, false, fltk_color);
}

void box_up_v_end_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, false, true, true, false, fltk_color);
}

void box_down_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, true, true, true, true, fltk_color);
}

void box_down_h_start_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, true, false, true, true, fltk_color);
}

void box_down_h_middle_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, false, true, false, true, true, fltk_color);
}

void box_down_h_end_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, false, true, true, true, true, fltk_color);
}

void box_down_v_start_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, true, true, false, true, fltk_color);
}

void box_down_v_middle_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, false, true, false, true, fltk_color);
}

void box_down_v_end_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_draw(x, y, w, h, true, false, true, true, true, fltk_color);
}

void box_thin_draw(
  int x, int y, int w, int h,
  bool down,
  Fl_Color fltk_color
) {
  bool select = false;
  bool highlight = false;
  
  if (FL_SELECTION_COLOR == fltk_color)
  {
    select = true;
    fltk_color = FL_COLOR_BUTTON;
  }
  else if (FL_COLOR_HIGHLIGHT == fltk_color)
  {
    highlight = true;
    fltk_color = FL_COLOR_BUTTON;
  }
  
  const Color & color_value = global()->color_value();
  djv_image::Color color = fltk_to_color(fltk_color);
  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  
  // Draw background.
  
  djv_image::color::convert(color, tmp);
  fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  
  fl_rectf(x, y, w, h);

  if (select && prefs()->focus())
  {
    djv_image::color::convert(color_value.select, tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
    
    fl_rect(x, y, w, h);
  }
  else if (highlight && prefs()->highlight())
  {
    djv_image::color::convert(color_value.highlight, tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
    
    fl_rect(x, y, w, h);
  }
  else
  {
    // Draw west/north bevels.
  
    const int x1 = x;
    const int y1 = y;
    const int x2 = x + w - 1;
    const int y2 = y + h - 1;
    
    djv_image::color::scale(down ? 0.8 : 1.2, color, tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
    
    fl_line(x1, y1, x1, y2); // west
    fl_line(x1, y1, x2, y1); // north
    
    // Draw east/south bevels.
    
    djv_image::color::scale(down ? 1.2 : 0.8, color, tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
    
    fl_line(x2, y1 + 1, x2, y2); // east
    fl_line(x1 + 1, y2, x2, y2); // south
  }
}

void box_up_thin_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_thin_draw(x, y, w, h, false, fltk_color);
}

void box_down_thin_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_thin_draw(x, y, w, h, true, fltk_color);
}

void box_border_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  const bool select = FL_SELECTION_COLOR == fltk_color;
  const bool highlight = FL_COLOR_HIGHLIGHT == fltk_color;

  const Color & color_value = global()->color_value();
  djv_image::Color color = fltk_to_color(fltk_color);
  
  // Draw border.
  
  if (select && prefs()->focus())
    fl_color(FL_SELECTION_COLOR);
  else if (highlight && prefs()->highlight())
    fl_color(FL_COLOR_HIGHLIGHT);
  else
    fl_color(FL_COLOR_BORDER);
  
  fl_rect(x, y , w, h);
}

void box_menu_draw(int x, int y, int w, int h, Fl_Color fltk_color)
{
  box_thin_draw(x, y, w, h, false, FL_BACKGROUND_COLOR);
}

}

Style::Style() :
  _color(prefs()->color()),
  _size(prefs()->size())
{
  DJV_DEBUG("Style::Style");
  DJV_DEBUG_PRINT("color = " << _color);

  for (int i = 0; i < _COLOR_SIZE; ++i)
    _color_value[i] = prefs()->color_value(static_cast<COLOR>(i));
  for (int i = 0; i < _SIZE_SIZE; ++i)
    _size_value[i] = prefs()->size_value_(static_cast<SIZE>(i));
  
  Fl::set_boxtype(FL_BOX_UP, box_up_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_UP_H1, box_up_h_start_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_UP_H2, box_up_h_middle_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_UP_H3, box_up_h_end_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_UP_V1, box_up_v_start_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_UP_V2, box_up_v_middle_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_UP_V3, box_up_v_end_draw, 2, 2, 4, 4);

  Fl::set_boxtype(FL_BOX_DOWN, box_down_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_DOWN_H1, box_down_h_start_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_DOWN_H2, box_down_h_middle_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_DOWN_H3, box_down_h_end_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_DOWN_V1, box_down_v_start_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_DOWN_V2, box_down_v_middle_draw, 2, 2, 4, 4);
  Fl::set_boxtype(FL_BOX_DOWN_V3, box_down_v_end_draw, 2, 2, 4, 4);
 
  Fl::set_boxtype(FL_BOX_THIN_UP, box_up_thin_draw, 1, 1, 2, 2);
  Fl::set_boxtype(FL_BOX_THIN_DOWN, box_down_thin_draw, 1, 1, 2, 2);
  
  Fl::set_boxtype(FL_BOX_BORDER, box_border_draw, 1, 1, 2, 2);

  Fl::set_boxtype(FL_BOX_MENU, box_menu_draw, 1, 1, 2, 2);

  color_update(true);
  size_update();
}

void Style::color(COLOR in)
{
  if (in == _color)
    return;

  _color = in;

  color_update(true);
}

COLOR Style::color() const
{
  return _color;
}

void Style::color_value(COLOR color, const Color & in)
{
  if (in == _color_value[color])
    return;

  _color_value[color] = in;

  color_update(true);
}

void Style::color_value(const Color & in)
{
  color_value(_color, in);
}

const Color & Style::color_value(COLOR color) const
{
  return _color_value[color];
}

const Color & Style::color_value() const
{
  return color_value(_color);
}

void Style::color_update()
{
  color_update(false);
}

void Style::color_update(bool redraw)
{
  DJV_DEBUG("Style::color_update");
  DJV_DEBUG_PRINT("redraw = " << redraw);
  
  DJV_DEBUG_PRINT("foreground = " << _color_value[_color].foreground);
  DJV_DEBUG_PRINT("background = " << _color_value[_color].background);
  DJV_DEBUG_PRINT("background 2 = " << _color_value[_color].background2);
  DJV_DEBUG_PRINT("button = " << _color_value[_color].button);
  DJV_DEBUG_PRINT("select = " << _color_value[_color].select);
  DJV_DEBUG_PRINT("highlight = " << _color_value[_color].highlight);

  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  djv_image::color::convert(_color_value[_color].background, tmp);
  Fl::background(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  
  djv_image::color::convert(_color_value[_color].background2, tmp);
  Fl::background2(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  
  djv_image::color::convert(_color_value[_color].foreground, tmp);
  Fl::foreground(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  
  djv_image::color::convert(_color_value[_color].button, tmp);
  Fl::set_color(FL_COLOR_BUTTON, tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
  
  djv_image::color::convert(_color_value[_color].select, tmp);
  Fl::set_color(FL_SELECTION_COLOR, tmp.get_u8(0), tmp.get_u8(1),
    tmp.get_u8(2));

  djv_image::color::convert(_color_value[_color].highlight, tmp);
  Fl::set_color(FL_COLOR_HIGHLIGHT, tmp.get_u8(0), tmp.get_u8(1),
    tmp.get_u8(2));

  djv_image::color::scale(0.6, _color_value[_color].background, tmp);
  Fl::set_color(FL_COLOR_BORDER, tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

  if (redraw)
    djv_ui::global()->window_redraw();
  
  /*uchar r, g, b;
  Fl::get_color(FL_SELECTION_COLOR, r, g, b);
  
  DJV_DEBUG_PRINT("r = " << r);
  DJV_DEBUG_PRINT("g = " << g);
  DJV_DEBUG_PRINT("b = " << b);*/
}

void Style::size(SIZE in)
{
  if (in == _size)
    return;
  
  _size = in;
  
  size_update();
}

SIZE Style::size() const
{
  return _size;
}

void Style::size_value_(SIZE size, int in)
{
  if (in == _size_value[size])
    return;
  
  _size_value[size] = in;
  
  size_update();
}

int Style::size_value_(SIZE size) const
{
  return _size_value[size];
}

void Style::size_value(int in)
{
  size_value_(_size, in);
}

int Style::size_value() const
{
  return size_value_(_size);
}

int Style::size_pick() const
{
  return math::round(_size_value[_size] * 0.8);
}

int Style::size_handle() const
{
  return math::round(_size_value[_size] * 3.0);
}

void Style::size_update()
{
  DJV_DEBUG("Style::size_update");

  Fl_Tooltip::size(_size_value[_size]);
  
  djv_ui::global()->window_update();
}

Fl_Font Style::font() const
{
  return FL_HELVETICA;
}

Fl_Font Style::font_bold() const
{
  return FL_HELVETICA_BOLD;
}

int Style::font_size() const
{
  return _size_value[_size];
}

int Style::spacing() const
{
  return _spacing[_size];
}

int Style::spacing_large() const
{
  return _spacing_large[_size];
}

int Style::margin() const
{
  return _margin[_size];
}

int Style::margin_text() const
{
  return _margin_text[_size];
}

int Style::margin_widget() const
{
  return _margin_widget[_size];
}

int Style::margin_large() const
{
  return _margin_large[_size];
}

int Style::size_icon() const
{
  return _size_icon;
}

int Style::size_tool_icon() const
{
  return _size_tool_icon;
}

int Style::size_button() const
{
  return math::round(_size_value[_size] * 4.2);
}

int Style::size_text() const
{
  return math::round(_size_value[_size] * 10.0);
}

int Style::size_swatch() const
{
  return math::round(_size_value[_size] * 4.2);
}

int Style::size_thumbnail() const
{
  return math::round(_size_value[_size] * 6.5);
}

//------------------------------------------------------------------------------

}
}

