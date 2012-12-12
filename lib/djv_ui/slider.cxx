/*------------------------------------------------------------------------------
 lib/djv_ui/slider.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "slider.h"

#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace slider {

//------------------------------------------------------------------------------
// Slider_Base
//------------------------------------------------------------------------------

Slider_Base::Slider_Base() :
  _handle_size(10)
{
  // Layout.
  
  size(layout_item::STRETCH, layout_item::FIXED);

  // Initialize.
  
  Fl_Group::color(FL_BACKGROUND2_COLOR);
  Fl_Group::set_visible_focus();

  style(frame::STYLE_BORDER);
  
  highlight(true);
}

Slider_Base::~Slider_Base()
{}

int Slider_Base::handle(int in)
{
  switch (in)
  {
    case FL_FOCUS:
    case FL_UNFOCUS:
      redraw();
      return 1;
  }
  
  return Frame::handle(in);
}

void Slider_Base::draw()
{
  const Box2i & geom = frame_geom();
  
  //const bool inside = Widget_Base::inside();
  
  fl_color(FL_BACKGROUND2_COLOR);
  fl_rectf(geom.x, geom.y, geom.w, geom.h);

  Frame::draw();
}

void Slider_Base::handle_size(int in)
{
  _handle_size = in;
}
  
int Slider_Base::handle_size() const
{
  return _handle_size;
}

Box2i Slider_Base::slider_geom() const
{
  return box::border(frame_geom(), V2i(-_handle_size / 2, 0));
}

void Slider_Base::handle_draw(double in, const Box2i & contents, bool inside)
{
  DJV_DEBUG("Slider_Base::handle_draw");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("contents = " << contents);
  DJV_DEBUG_PRINT("inside = " << inside);
  
  fl_color(fl_darker(FL_BACKGROUND2_COLOR));
  fl_rectf(
    contents.x + _handle_size / 2,
    contents.y + contents.h / 2 - 1,
    contents.w - _handle_size,
    2
  );

  const Box2i box(
    contents.x + static_cast<int>(in * (contents.w - _handle_size)),
    contents.y,
    _handle_size,
    contents.h
  );
  
  fl_draw_box(
    style::FL_BOX_THIN_UP,
    box.x, box.y, box.w, box.h,
    this == Fl::focus() && active() ?
      FL_SELECTION_COLOR :
      (inside && active() ? style::FL_COLOR_HIGHLIGHT : style::FL_COLOR_BUTTON)
  );
}

void Slider_Base::dirty()
{
  Frame::dirty();

  fl_font(style::global()->font(), style::global()->font_size());
  
  const int frame_margin = 2;
  
  size_hint(
    V2i(style::global()->size_button() * 2, fl_height()) +
    style::global()->margin_text() * 2 + frame_margin * 2
  );
}

//------------------------------------------------------------------------------
// Int_Slider
//------------------------------------------------------------------------------

Int_Slider::Int_Slider() :
  signal(this)
{
  debug_path("Int_Slider");
  
  range(0, 100);
}

Int_Slider::~Int_Slider()
{}

int Int_Slider::handle(int in)
{
  const Box2i & box = frame_geom();
  
  switch (in)
  { 
    case FL_PUSH:
    case FL_DRAG:
    {
      take_focus();
      
      const int size = handle_size();
      
      const double v =
        (
          Fl::event_x() - box.x - ((size - 1) / 2) +
          ((box.w - size - 1) / static_cast<double>(max() - min()) / 2)
        ) /
        static_cast<double>(box.w - size - 1);
      
      set(min() + static_cast<int>(v * (max() - min())));
    }
    return 1;
    
    case FL_KEYBOARD:
      if (! Fl::event_state(FL_SHIFT | FL_CTRL | FL_ALT))
        switch (Fl::event_key())
        {
          case FL_Home: min_action(); return 1;
          case FL_End: max_action(); return 1;
          case FL_Left:
          case FL_Down: dec_small_action(); return 1;
          case FL_Right:
          case FL_Up: inc_small_action(); return 1;
          case FL_Page_Down: dec_large_action(); return 1;
          case FL_Page_Up: inc_large_action(); return 1;
        }
      break;
  }
  
  if (Int_Input_Base::handle(in, visible_focus()))
    return 1;
  
  return Slider_Base::handle(in);
}

void Int_Slider::draw()
{
  const Box2i & geom = this->geom();
  
  const bool inside = Widget_Base::inside();
  
  fl_color(FL_BACKGROUND2_COLOR);
  fl_rectf(geom.x, geom.y, geom.w, geom.h);
  
  Slider_Base::draw();
  
  handle_draw(
    min() != max() ?
      ((get() - min()) / static_cast<double>(max() - min())) :
      0.0,
    frame_geom(),
    inside
  );
}

void Int_Slider::value_callback()
{
  redraw();
  
  signal.emit(get());
}

//------------------------------------------------------------------------------
// Float_Slider
//------------------------------------------------------------------------------

Float_Slider::Float_Slider() :
  signal(this)
{
  debug_path("Float_Slider");
  
  range(0.0, 1.0);
}

Float_Slider::~Float_Slider()
{}

int Float_Slider::handle(int in)
{
  const Box2i & box = frame_geom();
  
  switch (in)
  {
    case FL_PUSH:
    case FL_DRAG:
    {
      take_focus();
      
      const int size = handle_size();
      
      const int w = box.w - size - 1;
      
      const double v = w ?
        ((Fl::event_x() - box.x - (size - 1) / 2) / static_cast<double>(w)) :
        0.0;
      
      set(min() + v * (max() - min()));
    }
    return 1;
    
    case FL_KEYBOARD:
      if (! Fl::event_state(FL_SHIFT | FL_CTRL | FL_ALT))
        switch (Fl::event_key()) {
          case FL_Home: min_action(); return 1;
          case FL_End: max_action(); return 1;
          case FL_Left:
          case FL_Down: dec_small_action(); return 1;
          case FL_Right:
          case FL_Up: inc_small_action(); return 1;
          case FL_Page_Down: dec_large_action(); return 1;
          case FL_Page_Up: inc_large_action(); return 1;
        }
      break;
  }
  
  if (Float_Input_Base::handle(in, visible_focus()))
    return 1;
  
  return Slider_Base::handle(in);
}

void Float_Slider::draw()
{
  Slider_Base::draw();
  
  const bool inside = Widget_Base::inside();
  
  handle_draw(
    max() != min() ?
      ((get() - min()) / static_cast<double>(max() - min())) :
      0.0,
    frame_geom(),
    inside
  );
}

void Float_Slider::value_callback()
{
  redraw();
  
  signal.emit(get());
}

//------------------------------------------------------------------------------

}
}

