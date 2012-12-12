/*------------------------------------------------------------------------------
 lib/djv_ui/dialog_progress.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "dialog.h"

#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/push_button.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/Fl_Progress.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace dialog {

//------------------------------------------------------------------------------
// _Widget
//------------------------------------------------------------------------------

namespace {

class _Widget : public Fl_Progress, public Widget_Base
{
public:

  _Widget() : Fl_Progress(0, 0, 0, 0)
  {
    selection_color(FL_SELECTION_COLOR);
    
    Widget_Base::size(layout_item::STRETCH, layout_item::FIXED);
  }

  int handle(int in)
  {
    switch (Widget_Base::handle(in))
    {
      case -1: return 0;
      case 1: return 1;
    }
    return Fl_Progress::handle(in);
  }
  
  void show()
  {
    Fl_Progress::show();
    Widget_Base::show();
  }
  
  void draw()
  {
    Fl_Progress::draw();
    Widget_Base::draw();
  }

  const Fl_Widget * fltk_widget() const { return this; }

  void dirty()
  {
    Widget_Base::dirty();
  
    fl_font(text_font(), style::global()->font_size());
    
    size_hint(
      V2i(100, fl_height()) + style::global()->margin_widget() * 2
    );
  }
  
  void geom(const Box2i & in)
  {
    Fl_Progress::resize(in.x, in.y, in.w, in.h);
    Layout_Item::geom(in);
  }
  
  const Box2i & geom() const { return Widget_Base::geom(); }

  void resize(int x, int y, int w, int h) { geom(Box2i(x, y, w, h)); }
  
  int visible() const { return Fl_Progress::visible(); }
};

}

//------------------------------------------------------------------------------
// Progress::_Progress
//------------------------------------------------------------------------------

struct Progress::_Progress
{
  _Widget * widget;
  Label * label;
  Label * time_estimate, * time_elapsed;
};

//------------------------------------------------------------------------------
// Progress
//------------------------------------------------------------------------------

Progress::Progress() :
  Message_Base(label_progress),
  signal(this),
  del_signal(this),
  _value(0),
  _current(0),
  _p(new _Progress)
{
  // Create widgets.

  _p->widget = new _Widget;
  
  _p->label = new Label;
  
  _p->time_estimate = new Label;
  _p->time_elapsed = new Label;
  
  Push_Button * cancel_button = new Push_Button(label_cancel);

  // Layout.

  Layout_V * layout = new Layout_V(this);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout->stretch(layout_h);
  layout_h->margin(style::global()->margin_large());
  
  Layout_V * layout_v = new Layout_V(layout_h);
  layout_v->add(_p->label);
  layout_v->add(_p->widget);
  layout_v->add(_p->time_estimate);
  layout_v->add(_p->time_elapsed);

  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(cancel_button);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.

  //set_modal();

  size(size_hint(), false);

  // Callbacks.
  
  cancel_button->signal.set(this, cancel_callback);
  close_signal.set(this, cancel_callback);
}

Progress::~Progress()
{}

void Progress::del()
{
  del_signal.emit(false);
  
  Message_Base::del();
  
  Fl::remove_idle(idle_callback, this);
}

void Progress::set(int in)
{
  _value = in;
  _current = 0;
  _time.start();
  _time_accum = 0.0;
  _elapsed.start();
  
  _p->widget->maximum(static_cast<float>(_value));
  _p->widget->value(static_cast<float>(_current));
  
  Fl::add_idle(idle_callback, this);
}

void Progress::label(const String & in)
{
  _p->label->set(in);
}

const String & Progress::label() const
{
  return _p->label->get();
}

void Progress::cancel_callback(bool)
{
  Fl::remove_idle(idle_callback, this);
  
  del_signal.emit(false);
  
  hide();
}

void Progress::idle_callback()
{
  if (_current >= _value)
  {
    hide();
    
    Fl::remove_idle(idle_callback, this);

    del_signal.emit(true);
  }
  else
  {
    signal.emit(_current);
  }
  
  _p->widget->value(static_cast<float>(_current));

  _time.check();
  _elapsed.check();
  
  _time_accum += _time.seconds();
  _time.start();
  const double estimate =
    _time_accum / static_cast<double>(_current + 1) * (_value - (_current + 1));

  _p->time_estimate->set(
    string::Format(label_progress_estimate).
      arg(djv_base::time::label_time(estimate)).
      arg(_current / _elapsed.seconds(), 0, 2));
  _p->time_elapsed->set(
    string::Format(label_progress_elapsed).
      arg(djv_base::time::label_time(_elapsed.seconds())));

  ++_current;
}

//------------------------------------------------------------------------------

}
}
