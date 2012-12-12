/*------------------------------------------------------------------------------
 bin/djv_view/tool_color_picker.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "tool_color_picker.h"

#include "application.h"
#include "image_base.h"
#include "view_widget.h"
#include <djv_ui/check_button.h>
#include <djv_ui/choice.h>
#include <djv_ui/color.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/prefs.h>
#include <djv_ui/push_button.h>
#include <djv_ui/tool_button.h>
#include <djv_image/gl_offscreen.h>

namespace djv_bin {
namespace view {
namespace tool_color_picker {

//------------------------------------------------------------------------------
// Dialog::_Dialog
//------------------------------------------------------------------------------

struct Dialog::_Dialog
{
  std::auto_ptr<djv_image::gl_offscreen::Buffer> buffer;
  //std::auto_ptr<djv_image::gl_data::Data> data;

  djv_ui::Color_Widget * widget;
  djv_ui::Color_Swatch * swatch;
  djv_ui::Choice * size;
  djv_ui::Tool_Button * color_profile;
  djv_ui::Tool_Button * display_profile;
  djv_ui::Tool_Button * lock;
  djv_ui::Push_Button * close;
};

//------------------------------------------------------------------------------
// Dialog
//------------------------------------------------------------------------------

Dialog::Dialog() :
  Base(dialog_title),
  _view(0),
  _value(djv_image::pixel::RGBA_F32),
  _size(SIZE_3),
  _color_profile(true),
  _display_profile(true),
  _lock(false),
  _p(new _Dialog)
{
  using namespace djv_ui;
  
  DJV_DEBUG("Dialog::Dialog");
  
  // Create widgets.

  _p->widget =
    new Color_Widget;
  
  _p->swatch =
    new Color_Swatch;
  _p->swatch->size(layout_item::STRETCH, layout_item::STRETCH);
  
  _p->size =
    new Choice(label_size());
  _p->size->tooltip(tooltip_size);
  
  _p->color_profile =
    new Tool_Button("grey_ramp");
  _p->color_profile->type(tool_button::TOGGLE);
  _p->color_profile->tooltip(tooltip_color_profile);
  
  _p->display_profile =
    new Tool_Button("grey_ramp");
  _p->display_profile->type(tool_button::TOGGLE);
  _p->display_profile->tooltip(tooltip_display_profile);

  _p->lock =
    new Tool_Button("tool_lock");
  _p->lock->type(tool_button::TOGGLE);
  _p->lock->tooltip(tooltip_lock);

  _p->close =
    new Push_Button(djv_ui::dialog::label_close);
  
  // Layout.
  
  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  Layout_H * layout_h = 0;
  
  layout = new Layout_V(this);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(_p->swatch);
  layout_h->add(_p->widget);
  layout_h->stretch(_p->widget);
  
  layout->add_spacer(-1, true);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(_p->size);
  layout_h->add(new Frame_Group_H(List<Frame *>() <<
    _p->color_profile <<
    _p->display_profile));
  layout_h->add_spacer(-1, true);
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  _p->widget->bottom_layout()->insert(_p->lock, 1);
  
  // Preferences.
  
  prefs::Prefs prefs(prefs::prefs(), "tool_color_picker");
  prefs::get(&prefs, "size", &_size);
  djv_image::PIXEL pixel = _value.pixel();
  prefs::get(&prefs, "pixel", &pixel);
  _value.pixel(pixel);
  prefs::get(&prefs, "color_profile", &_color_profile);
  prefs::get(&prefs, "display_profile", &_display_profile);
  prefs::get(&prefs, "lock", &_lock);
  
  // Initialize.

  widget_update();
  
  size(size_hint());
  
  // Callbacks.

  _p->widget->signal.set(this, widget_callback);
  _p->size->signal.set(this, size_callback);
  _p->color_profile->signal.set(this, color_profile_callback);
  _p->display_profile->signal.set(this, display_profile_callback);
  _p->lock->signal.set(this, lock_callback);
  _p->close->signal.set(this, close_callback);
}

Dialog::~Dialog()
{
  DJV_DEBUG("Dialog::~Dialog");
}

void Dialog::del()
{
  using namespace djv_ui;
  
  DJV_DEBUG("Dialog::del");
  
  djv_ui::dialog::Base::del();

  prefs::Prefs prefs(prefs::prefs(), "tool_color_picker");
  prefs::set(&prefs, "size", _size);
  prefs::set(&prefs, "pixel", _value.pixel());
  prefs::set(&prefs, "color_profile", _color_profile);
  prefs::set(&prefs, "display_profile", _display_profile);
  prefs::set(&prefs, "lock", _lock);
  
  _p->buffer.reset();
}

void Dialog::set(const view::Widget * in, const V2i & pick)
{
  _view = in;
  _pick = pick;
  
  widget_update();
}

void Dialog::pick(const view::Widget * in, const V2i & pick)
{
  if (! shown() || ! visible())
    return;
  
  set(in, pick);
}

void Dialog::update(const view::Widget * in)
{
  if (in != _view || ! shown() || ! visible())
    return;
  
  widget_update();
}

void Dialog::del(const view::Widget * in)
{
  if (in != _view)
    return;
  
  _view = 0;
}

void Dialog::show()
{
  DJV_DEBUG("Dialog::show");
  
  _p->close->take_focus();

  djv_ui::dialog::Base::show();
}

void Dialog::widget_callback(const djv_image::Color & in)
{
  _value = in;
  
  _p->swatch->set(in);
}

void Dialog::size_callback(int in)
{
  _size = static_cast<SIZE>(in);
  
  widget_update();
}

void Dialog::color_profile_callback(bool in)
{
  _color_profile = in;
  
  widget_update();
}

void Dialog::display_profile_callback(bool in)
{
  _display_profile = in;
  
  widget_update();
}

void Dialog::lock_callback(bool in)
{
  _lock = in;
  
  widget_update();
}

void Dialog::close_callback(bool)
{
  hide();
}

void Dialog::widget_update()
{
  if (! _view)
    return;
  
  DJV_DEBUG("Dialog::widget_update");
  DJV_DEBUG_PRINT("color profile = " << _color_profile);
  DJV_DEBUG_PRINT("display profile = " << _display_profile);
  DJV_DEBUG_PRINT("lock = " << _lock);
  
  callbacks(false);

  const djv_image::Data * data = _view->get();
  
  if (! _lock && data)
    _value.pixel(data->pixel());
  else
    _value.pixel(_p->widget->get().pixel());
  
  // Pick.
  
  const V2i pick = vector::floor<double, int>(V2f(_pick) / _view->zoom());
  
  DJV_DEBUG_PRINT("pick = " << pick);
  
  // Render color sample.
  
  const int size = tool_color_picker::size(_size);
  
  DJV_DEBUG_PRINT("size = " << size);

  djv_image::Data tmp(djv_image::Info(V2i(size), _value.pixel()));
  
  try
  {
    if (! _p->buffer.get())
      _p->buffer = std::auto_ptr<djv_image::gl_offscreen::Buffer>(
        djv_image::gl_offscreen::global()->buffer_create()
      );
    
    _p->buffer->init(tmp.info());

    //if (! _p->data.get())
    //  _p->data = std::auto_ptr<djv_image::gl_data::Data>(
    //    new djv_image::gl_data::Data
    //  );

    djv_image::gl_data::Option option = _view->option();
    option.xform.position -= pick - V2i((size - 1) / 2);

    if (! _color_profile)
      option.color_profile = djv_image::Color_Profile();

    if (! _display_profile)
    {
      image::Display_Profile tmp;
      
      option.display_profile = tmp;
    }

    djv_image::Data empty;
    if (! data)
      data = &empty;
    
    djv_image::gl_data::copy(
      *data,
      &tmp,
      option,
      //_p->data.get(),
      &_view->gl_data(),
      _p->buffer.get(),
      false
    );

    djv_image::gl_data::average(tmp, &_value);
  }
  catch (Error in)
  {
    application::global()->error(in);
  }
  
  DJV_DEBUG_PRINT("value = " << _value);
  
  _p->widget->set(_value);
  _p->swatch->set(_value);
  _p->size->set(_size);
  _p->color_profile->set(_color_profile);
  _p->display_profile->set(_display_profile);
  _p->lock->set(_lock);
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

int size(SIZE in)
{
  static const int data [] =
  {
    1,
    3,
    5,
    7,
    9,
    51,
    101
  };
  
  DJV_ASSERT(sizeof(data) / sizeof(data[0]) == _SIZE_SIZE);
  
  return data[in];
}

_DJV_STRING_OPERATOR_LABEL(SIZE, label_size())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_size()
{
  static const List<String> data = List<String>() <<
    "1x1" <<
    "3x3" <<
    "5x5" <<
    "7x7" <<
    "9x9" <<
    "51x51" <<
    "101x101";
  
  DJV_ASSERT(data.size() == _SIZE_SIZE);
  
  return data;
}

extern const String
  dialog_title = "Color Picker";

extern const String
  tooltip_size = "Sample size",
  tooltip_color_profile = "Use color profile",
  tooltip_display_profile = "Use display profile",
  tooltip_lock = "Lock pixel";

//------------------------------------------------------------------------------

}
}
}

