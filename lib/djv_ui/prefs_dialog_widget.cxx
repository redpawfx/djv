/*------------------------------------------------------------------------------
 lib/djv_ui/prefs_dialog_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "prefs_dialog.h"

#include <djv_ui/check_button.h>
#include <djv_ui/choice.h>
#include <djv_ui/color.h>
#include <djv_ui/group_box.h>
#include <djv_ui/image_prefs.h>
#include <djv_ui/int_value.h>
#include <djv_ui/float_value.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>
#include <djv_ui/text.h>

namespace djv_ui {
namespace prefs_dialog {

//------------------------------------------------------------------------------
// Style::_Style
//------------------------------------------------------------------------------

struct Style::_Style
{
  
  Radio_Button_Group * color;
  
  Color_Swatch_Widget * color_foreground;
  Color_Swatch_Widget * color_background;
  Color_Swatch_Widget * color_background2;
  Color_Swatch_Widget * color_button;
  Color_Swatch_Widget * color_select;
  Color_Swatch_Widget * color_highlight;

  Check_Button * focus;
  Check_Button * highlight;

  Radio_Button_Group * size;
  Int_Edit * size_value;
};

//------------------------------------------------------------------------------
// Style
//------------------------------------------------------------------------------

Style::Style() :
  _p(new _Style)
{
  // Create widgets.

  group_box::Box_Large * color_group =
    new group_box::Box_Large(prefs_color_group);
  
  _p->color =
    new Radio_Button_Group(style::label_color());
  
  _p->color_foreground =
    new Color_Swatch_Widget(prefs_color_foreground);
  _p->color_background =
    new Color_Swatch_Widget(prefs_color_background);
  _p->color_background2 =
    new Color_Swatch_Widget(prefs_color_background2);
  _p->color_button =
    new Color_Swatch_Widget(prefs_color_button);
  _p->color_select =
    new Color_Swatch_Widget(prefs_color_select);
  _p->color_highlight =
    new Color_Swatch_Widget(prefs_color_highlight);

  _p->focus =
    new Check_Button(prefs_focus);
  _p->highlight =
    new Check_Button(prefs_highlight);
  
  group_box::Box_Large * size_group =
    new group_box::Box_Large(prefs_size_group);

  _p->size =
    new Radio_Button_Group(style::label_size());

  _p->size_value =
    new Int_Edit;
  Label * size_value_label =
    new Label(prefs_size_value);

  // Layout.
  
  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  Layout_H * layout_h = 0;
  Layout_H * layout_h2 = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(color_group);
  color_group->layout()->add(_p->color);
  layout_v = new Layout_V(color_group->layout());
  layout_v->margin(0);
  layout_h = new Layout_H(color_group->layout());
  layout_h->spacing(style::global()->spacing_large());
  layout_h->margin(0);
  layout_h->add(_p->color_foreground);
  layout_h->add(_p->color_background);
  layout_h->add(_p->color_background2);
  layout_h->add(_p->color_button);
  layout_h->add(_p->color_select);
  layout_h->add(_p->color_highlight);
  color_group->layout()->add(_p->focus);
  color_group->layout()->add(_p->highlight);

  layout->add(size_group);
  size_group->layout()->add(_p->size);
  layout_h = new Layout_H(size_group->layout());
  layout_h->margin(0);
  layout_h->add(size_value_label);
  layout_h->add(_p->size_value);

  layout->add_stretch();
  
  // Initialize.
  
  color_update();
  size_update();

  // Callbacks.

  _p->color->signal.set(this, color_callback);
  _p->color_foreground->signal.set(this, color_foreground_callback);
  _p->color_background->signal.set(this, color_background_callback);
  _p->color_background2->signal.set(this, color_background2_callback);
  _p->color_button->signal.set(this, color_button_callback);
  _p->color_select->signal.set(this, color_select_callback);
  _p->color_highlight->signal.set(this, color_highlight_callback);
  _p->focus->signal.set(this, focus_callback);
  _p->highlight->signal.set(this, highlight_callback);
  
  _p->size->signal.set(this, size_callback);
  _p->size_value->signal.set(this, size_value_callback);
}

void Style::color_callback(int in)
{
  style::prefs()->color(static_cast<style::COLOR>(in));
  
  color_update();
}

void Style::color_foreground_callback(const djv_image::Color & in)
{
  style::Color tmp = style::prefs()->color_value();
  tmp.foreground = in;
  style::prefs()->color_value(tmp);
}

void Style::color_background_callback(const djv_image::Color & in)
{
  style::Color tmp = style::prefs()->color_value();
  tmp.background = in;
  style::prefs()->color_value(tmp);
}

void Style::color_background2_callback(const djv_image::Color & in)
{
  style::Color tmp = style::prefs()->color_value();
  tmp.background2 = in;
  style::prefs()->color_value(tmp);
}

void Style::color_button_callback(const djv_image::Color & in)
{
  style::Color tmp = style::prefs()->color_value();
  tmp.button = in;
  style::prefs()->color_value(tmp);
}

void Style::color_select_callback(const djv_image::Color & in)
{
  style::Color tmp = style::prefs()->color_value();
  tmp.select = in;
  style::prefs()->color_value(tmp);
}

void Style::color_highlight_callback(const djv_image::Color & in)
{
  style::Color tmp = style::prefs()->color_value();
  tmp.highlight = in;
  style::prefs()->color_value(tmp);
}

void Style::focus_callback(bool in)
{
  style::prefs()->focus(in);
}

void Style::highlight_callback(bool in)
{
  style::prefs()->highlight(in);
}

void Style::size_callback(int in)
{
  style::prefs()->size(static_cast<style::SIZE>(in));
  
  size_update();
}

void Style::size_value_callback(int in)
{
  style::prefs()->size_value(in);
}

void Style::color_update()
{
  callbacks(false);
  
  _p->color->set(style::prefs()->color());

  _p->color_foreground->set(style::prefs()->color_value().foreground);
  _p->color_background->set(style::prefs()->color_value().background);
  _p->color_background2->set(style::prefs()->color_value().background2);
  _p->color_button->set(style::prefs()->color_value().button);
  _p->color_select->set(style::prefs()->color_value().select);
  _p->color_highlight->set(style::prefs()->color_value().highlight);
  
  _p->focus->set(style::prefs()->focus());
  _p->highlight->set(style::prefs()->highlight());

  callbacks(true);
}

void Style::size_update()
{
  callbacks(false);
  
  _p->size->set(style::prefs()->size());
  _p->size_value->set(style::prefs()->size_value());
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// General
//------------------------------------------------------------------------------

General::General()
{
  // Create time widgets.

  group_box::Box_Large * time_group =
    new group_box::Box_Large(prefs_time_group);
  
  Radio_Button_Group * time_units =
    new Radio_Button_Group(prefs_time_units, djv_image::time::label_units());
  
  Choice * speed_widget =
    new Choice(djv_image::time::label_fps());
  Label * speed_label =
    new Label(prefs_speed);

  // Create filter widgets.
  
  group_box::Box_Large * filter_group =
    new group_box::Box_Large(prefs_filter_group);
  
  Label * filter_text =
    new Label(prefs_filter_text);

  Radio_Button_Group * filter_min =
    new Radio_Button_Group(
      prefs_filter_min, djv_image::gl_data::label_filter()
    );
  
  Radio_Button_Group * filter_mag =
    new Radio_Button_Group(
      prefs_filter_mag, djv_image::gl_data::label_filter()
    );

  // Create help widgets.
  
  group_box::Box_Large * help_group =
    new group_box::Box_Large(prefs_help_group);

  Check_Button * help_tooltips =
    new Check_Button(prefs_help_tooltips);

  // Layout.

  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  Layout_H * layout_h = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(time_group);
  time_group->layout()->add(time_units);
  layout_h = new Layout_H(time_group->layout());
  layout_h->margin(0);
  layout_h->add(speed_label);
  layout_h->add(speed_widget);

  layout->add(filter_group);
  filter_group->layout()->add(filter_text);
  layout_h = new Layout_H(filter_group->layout());
  layout_h->spacing(style::global()->spacing_large());
  layout_h->margin(0);
  layout_h->add(filter_min);
  layout_h->add(filter_mag);
  layout_h->add_stretch();

  layout->add(help_group);
  help_group->layout()->add(help_tooltips);

  layout->add_stretch();
  
  // Initialize.
  
  time_units->set(image::prefs()->units());
  speed_widget->set(djv_image::time::global()->speed());
  
  filter_min->set(image::prefs()->filter().min);
  filter_mag->set(image::prefs()->filter().mag);
  
  help_tooltips->set(base::prefs()->tooltips());

  // Callbacks.

  time_units->signal.set(this, time_units_callback);
  speed_widget->signal.set(this, speed_callback);
  
  filter_min->signal.set(this, filter_min_callback);
  filter_mag->signal.set(this, filter_mag_callback);
  
  help_tooltips->signal.set(this, help_tooltips_callback);
}

void General::time_units_callback(int in)
{
  image::prefs()->units(static_cast<djv_image::time::UNITS>(in));
}

void General::speed_callback(int in)
{
  djv_image::time::global()->speed(static_cast<djv_image::time::FPS>(in));
}

void General::filter_min_callback(int in)
{
  image::prefs()->filter(
    djv_image::gl_data::Filter(
      static_cast<djv_image::gl_data::FILTER>(in),
      image::prefs()->filter().mag
    )
  );
}

void General::filter_mag_callback(int in)
{
  image::prefs()->filter(
    djv_image::gl_data::Filter(
      image::prefs()->filter().min,
      static_cast<djv_image::gl_data::FILTER>(in)
    )
  );
}

void General::help_tooltips_callback(bool in)
{
  base::prefs()->tooltips(in);
}

//------------------------------------------------------------------------------

}
}

