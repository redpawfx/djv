/*------------------------------------------------------------------------------
 bin/djv_view/view_prefs_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "view_prefs.h"

#include <djv_ui/check_button.h>
#include <djv_ui/choice.h>
#include <djv_ui/color.h>
#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>
#include <djv_ui/text.h>

namespace djv_bin {
namespace view {
namespace view {

//------------------------------------------------------------------------------
// Prefs_Widget::_Prefs_Widget
//------------------------------------------------------------------------------

struct Prefs_Widget::_Prefs_Widget
{
  djv_ui::Color_Swatch_Widget * background_color;

  djv_ui::Color_Swatch_Widget * grid_color;

  djv_ui::Check_Button_Group * hud_show;
  djv_ui::Color_Swatch_Widget * hud_color;
  djv_ui::Color_Swatch_Widget * hud_background_color;
};

//------------------------------------------------------------------------------
// Prefs_Widget
//------------------------------------------------------------------------------

Prefs_Widget::Prefs_Widget() :
  _p(new _Prefs_Widget)
{
  using namespace djv_ui;
  
  // Create general widgets.

  group_box::Box_Large * general_group =
    new group_box::Box_Large(prefs_group);

  _p->background_color =
    new Color_Swatch_Widget(prefs_background);

  // Create resize widgets.

  group_box::Box_Large * resize_group =
    new group_box::Box_Large(prefs_resize_group);

  Radio_Button_Group * resize_widget =
    new Radio_Button_Group(label_resize());

  // Create grid widgets.
  
  group_box::Box_Large * grid_group =
    new group_box::Box_Large(prefs_grid_group);
  
  Radio_Button_Group * grid_widget =
    new Radio_Button_Group(label_grid());
  
  _p->grid_color =
    new Color_Swatch_Widget(prefs_grid_color);
  
  // Create HUD widgets.
  
  group_box::Box_Large * hud_group =
    new group_box::Box_Large(prefs_hud_group);
  
  Check_Button * hud_widget =
    new Check_Button(prefs_hud);
  
  _p->hud_show =
    new Check_Button_Group(prefs_hud_show, label_hud_show());
  
  _p->hud_color =
    new Color_Swatch_Widget(prefs_hud_color);
  
  Radio_Button_Group * hud_background_widget =
    new Radio_Button_Group(prefs_hud_background, label_hud_background());
  
  _p->hud_background_color =
    new Color_Swatch_Widget(prefs_hud_background_color);

  // Layout.
  
  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  Layout_H * layout_h = 0;
  Layout_H * layout_h2 = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());
  
  layout->add(general_group);
  general_group->layout()->add(_p->background_color);
  
  layout->add(resize_group);
  resize_group->layout()->add(resize_widget);

  layout->add(grid_group);
  layout_h = new Layout_H(grid_group->layout());
  layout_h->spacing(style::global()->spacing_large());
  layout_h->margin(0);
  layout_h->add(grid_widget);
  layout_h->add(_p->grid_color);
  layout_h->add_stretch();
  
  layout->add(hud_group);
  hud_group->layout()->add(hud_widget);
  layout_h = new Layout_H(hud_group->layout());
  layout_h->spacing(style::global()->spacing_large());
  layout_h->margin(0);
  layout_h->add(_p->hud_show);
  layout_h->add(_p->hud_color);
  layout_h->add_stretch();
  layout_h = new Layout_H(hud_group->layout());
  layout_h->spacing(style::global()->spacing_large());
  layout_h->margin(0);
  layout_h->add(hud_background_widget);
  layout_h->add(_p->hud_background_color);
  layout_h->add_stretch();
  
  layout->add_stretch();
  
  // Initialize.
  
  _p->background_color->set(prefs()->background());
  
  resize_widget->set(prefs()->resize());

  grid_widget->set(prefs()->grid());
  _p->grid_color->set(prefs()->grid_color());
  
  hud_widget->set(prefs()->hud());
  
  List<bool> hud_show;
  for (uint i = 0; i < _HUD_SHOW_SIZE; ++i)
    hud_show += prefs()->hud_show(static_cast<HUD_SHOW>(i));
  _p->hud_show->set(hud_show);
  
  _p->hud_color->set(prefs()->hud_color());
  hud_background_widget->set(prefs()->hud_background());
  _p->hud_background_color->set(prefs()->hud_background_color());
  
  // Callbacks.
  
  _p->background_color->signal.set(this, background_callback);

  resize_widget->signal.set(this, resize_callback);

  grid_widget->signal.set(this, grid_callback);
  _p->grid_color->signal.set(this, grid_callback);
  
  hud_widget->signal.set(this, hud_callback);
  _p->hud_show->signal.set(this, hud_show_callback);
  _p->hud_color->signal.set(this, hud_color_callback);
  hud_background_widget->signal.set(this, hud_background_callback);
  _p->hud_background_color->signal.set(this, hud_background_color_callback);
}

Prefs_Widget::~Prefs_Widget()
{}

void Prefs_Widget::background_callback(const djv_image::Color & in)
{
  _p->background_color->set(in);

  prefs()->background(in);
}

void Prefs_Widget::resize_callback(int in)
{
  prefs()->resize(static_cast<RESIZE>(in));
}

void Prefs_Widget::grid_callback(int in)
{
  prefs()->grid(static_cast<GRID>(in));
}

void Prefs_Widget::grid_callback(const djv_image::Color & in)
{
  _p->grid_color->set(in);

  prefs()->grid_color(in);
}

void Prefs_Widget::hud_callback(bool in)
{
  prefs()->hud(in);
}

void Prefs_Widget::hud_show_callback(const List<bool> & in)
{
  for (list::size_type i = 0; i < in.size(); ++i)
    prefs()->hud_show(static_cast<HUD_SHOW>(i), in[i]);
}

void Prefs_Widget::hud_color_callback(const djv_image::Color & in)
{
  _p->hud_color->set(in);
  
  prefs()->hud_color(in);
}

void Prefs_Widget::hud_background_callback(int in)
{
  prefs()->hud_background(static_cast<HUD_BACKGROUND>(in));
}

void Prefs_Widget::hud_background_color_callback(const djv_image::Color & in)
{
  _p->hud_background_color->set(in);
  
  prefs()->hud_background_color(in);
}

//------------------------------------------------------------------------------

}
}
}

