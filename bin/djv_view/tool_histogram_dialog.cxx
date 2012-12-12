/*------------------------------------------------------------------------------
 bin/djv_view/tool_histogram_dialog.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "tool_histogram.h"

#include "application.h"
#include "view_widget.h"
#include <djv_ui/application.h>
#include <djv_ui/choice.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/prefs.h>
#include <djv_ui/push_button.h>
#include <djv_ui/text.h>

namespace djv_bin {
namespace view {
namespace tool_histogram {

//------------------------------------------------------------------------------
// Dialog::_Dialog
//------------------------------------------------------------------------------

struct Dialog::_Dialog
{
  tool_histogram::Widget * widget;
  djv_ui::Choice * size;
  djv_ui::Text_Display * min, * max;
  djv_ui::Push_Button * close;
};

//------------------------------------------------------------------------------
// Dialog
//------------------------------------------------------------------------------

Dialog::Dialog() :
  Base(dialog_title),
  _view(0),
  _size(djv_image::gl_data::HISTOGRAM_256),
  _p(new _Dialog)
{
  using namespace djv_ui;
  
  // Create widgets.
  
  _p->widget =
    new tool_histogram::Widget;
  
  _p->size =
    new Choice(djv_image::gl_data::label_histogram());
  Label * size_label =
    new Label(dialog_size);
  
  _p->min =
    new Text_Display;
  Label * min_label =
    new Label(dialog_min);

  _p->max =
    new Text_Display;
  Label * max_label =
    new Label(dialog_max);
  
  _p->close =
    new Push_Button(djv_ui::dialog::label_close);

  // Layout.
  
  layout_item::group(List<Layout_Item *>() << min_label << max_label);
  
  Layout_V * layout = new Layout_V(this);
  
  layout->add(_p->widget);
  layout->stretch(_p->widget);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(min_label);
  layout_h->add(_p->min);  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(max_label);
  layout_h->add(_p->max);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(size_label);
  layout_h->add(_p->size);
  layout_h->add_spacer(-1, true);
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Preferences.

  prefs::Prefs prefs(prefs::prefs(), "tool_histogram");
  prefs::get(&prefs, "size", &_size);

  // Initialize.

  widget_update();

  size(size_hint());
  
  // Callbacks.
  
  _p->size->signal.set(this, size_callback);
  _p->close->signal.set(this, close_callback);
}

Dialog::~Dialog()
{}

void Dialog::del()
{
  using namespace djv_ui;
  
  Base::del();

  prefs::Prefs prefs(prefs::prefs(), "tool_histogram");
  prefs::set(&prefs, "size", _size);
}

void Dialog::set(const view::Widget * in)
{
  _view = in;
  
  widget_update();
}

void Dialog::pick(const view::Widget * in)
{
  if (in == _view || ! shown() || ! visible())
    return;
  
  set(in);
}

void Dialog::update(const view::Widget * in)
{
  if (in != _view || ! shown() || ! visible())
    return;
  
  widget_update();
}

void Dialog::del(const view::Widget * in)
{
  if (in != _view) return;
  
  _view = 0;
}

void Dialog::show()
{
  _p->close->take_focus();
  
  djv_ui::dialog::Base::show();
}

void Dialog::size_callback(int in)
{
  _size = static_cast<djv_image::gl_data::HISTOGRAM>(in);
  
  widget_update();
}

void Dialog::close_callback(bool)
{
  hide();
}

void Dialog::widget_update()
{
  DJV_DEBUG("Dialog::widget_update");
  
  callbacks(false);
  
  if (const djv_image::Data * data = _view ? _view->get() : 0)
  {
    try
    {
      djv_image::Data tmp(djv_image::Info(data->size(), data->pixel()));
      djv_image::gl_data::copy(*data, &tmp, _view->option(), &_view->gl_data());
      djv_image::gl_data::histogram(tmp, &_histogram, _size, &_min, &_max);
    }
    catch (Error in)
    {
      application::global()->error(in);
    }
  }
  else
  {
    _histogram.zero();
    _min.zero();
    _max.zero();
  }
  
  _p->widget->set(&_histogram, _min, _max);
  _p->size->set(_size);
  _p->min->set(string::label(_min));
  _p->max->set(string::label(_max));
  
  callbacks(true);
  
  Base::update();
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  dialog_title = "Histogram",
  dialog_size = "Size:",
  dialog_min = "Min:",
  dialog_max = "Max:";

//------------------------------------------------------------------------------

}
}
}


