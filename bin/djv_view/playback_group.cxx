/*------------------------------------------------------------------------------
 bin/djv_view/playback_group.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "playback_group.h"

#include "playback_prefs.h"
#include "shortcut.h"
#include "widget.h"
#include <djv_ui/layout_row.h>
#include <djv_ui/menu.h>
#include <djv_ui/shuttle.h>
#include <djv_ui/text.h>
#include <djv_ui/tool_button.h>

namespace djv_bin {
namespace view {
namespace playback {

//------------------------------------------------------------------------------
// Group::_Group
//------------------------------------------------------------------------------

struct Group::_Group
{
  djv_ui::menu::Menu * menu;
  List<int> menu_loop;
  List<int> menu_layout;
  
  djv_ui::Tool_Button * stop;
  djv_ui::Tool_Button * forward;
  djv_ui::Tool_Button * reverse;
  djv_ui::shuttle::Shuttle * playback_shuttle;
  djv_ui::Tool_Choice_Button * loop;
  
  widget::Speed_Widget * speed;
  widget::Speed_Display_Widget * speed_real;
  djv_ui::Tool_Button * every_frame;
  
  widget::Frame_Widget * frame;
  widget::Frame_Slider * frame_slider;
  djv_ui::shuttle::Shuttle * frame_shuttle;  
  widget::Frame_Display_Widget * start;
  widget::Frame_Display_Widget * end;
  djv_ui::Tool_Button * start_button;
  djv_ui::Tool_Button * end_button;
  djv_ui::Tool_Button * prev;
  djv_ui::Tool_Button * next;
  widget::Frame_Display_Widget * duration;
  
  djv_ui::Tool_Button * in_out;
  widget::Frame_Widget * in;
  widget::Frame_Widget * out;
  djv_ui::Tool_Button * in_mark;
  djv_ui::Tool_Button * in_reset;
  djv_ui::Tool_Button * out_mark;
  djv_ui::Tool_Button * out_reset;
  
  djv_ui::Layout_H * layout_toolbar;
  djv_ui::Frame_Group_H * layout_playback_group;
  djv_ui::Frame_Group_H * layout_frame_group;
  djv_ui::Frame_Group_H * layout_in_group;
  djv_ui::Frame_Group_H * layout_out_group;

  djv_ui::Layout_Spacer * window_handle_spacer;
};

//------------------------------------------------------------------------------
// Group
//------------------------------------------------------------------------------

Group::Group(
  djv_ui::menu::Menu * menu,
  djv_ui::layout_row::Layout_Row_Base * toolbar,
  Group * copy
) :
  image_signal(this),
  overlay_signal(this),
  update_signal(this),
  _playback(copy ? copy->_playback : STOP),
  _loop(copy ? copy->_loop : prefs()->loop()),
  _time(copy ? copy->_time : djv_image::Time()),
  _speed(copy ? copy->_speed : djv_image::time::Speed()),
  _speed_real(0.0),
  _dropped_frames(false),
  _dropped_frames_tmp(false),
  _every_frame(copy ? copy->_every_frame : prefs()->every_frame()),
  _frame(copy ? copy->_frame : 0),
  _frame_tmp(0),
  _in_out(copy ? copy->_in_out : true),
  _in(copy ? copy->_in : 0),
  _out(copy ? copy->_out : 0),
  _shuttle(false),
  _shuttle_speed(0.0),
  _idle_pause(false),
  _idle_init(true),
  _idle_frame(0),
  _speed_counter(0),
  _layout(copy ? copy->_layout : prefs()->layout()),
  _p(new _Group)
{
  using namespace djv_ui;

  // Create widgets.
  
  _p->menu = menu;
  
  // Create playback widgets.

  _p->stop =
    new Tool_Button("playback_stop");
  _p->stop->type(tool_button::RADIO);

  _p->forward =
    new Tool_Button("playback_forward");
  _p->forward->type(tool_button::RADIO);
  
  _p->reverse =
    new Tool_Button("playback_reverse");
  _p->reverse->type(tool_button::RADIO);

  tool_button::radio_group(List<Tool_Button *>() <<
    _p->reverse <<
    _p->stop <<
    _p->forward);

  _p->playback_shuttle =
    new shuttle::Shuttle("playback_");

  _p->loop =
    new Tool_Choice_Button(List<String>() <<
    "playback_loop_once" <<
    "playback_loop_repeat" <<
    "playback_loop_ping_pong");
  
  // Create speed widgets.
  
  _p->speed =
    new widget::Speed_Widget;
  
  _p->speed_real =
    new widget::Speed_Display_Widget;
  
  _p->every_frame =
    new Tool_Button("tool_lock");
  _p->every_frame->type(tool_button::TOGGLE);

  // Create frame widgets.
  
  _p->frame =
    new widget::Frame_Widget;
  
  _p->frame_slider =
    new widget::Frame_Slider;
  
  _p->frame_shuttle =
    new shuttle::Shuttle("playback_frame_");

  _p->start =
    new widget::Frame_Display_Widget;

  _p->start_button =
    new Tool_Button("playback_start");
  
  _p->end =
    new widget::Frame_Display_Widget;
  
  _p->end_button =
    new Tool_Button("playback_end");
  
  _p->prev =
    new Tool_Button("playback_prev");
  _p->prev->type(tool_button::REPEAT);
  
  _p->next =
    new Tool_Button("playback_next");
  _p->next->type(tool_button::REPEAT);
  
  _p->duration =
    new widget::Frame_Display_Widget;

  // Create in/out point widgets.
  
  _p->in_out =
    new Tool_Button("playback_in_out");
  _p->in_out->type(tool_button::TOGGLE);
  
  _p->in =
    new widget::Frame_Widget;
  _p->in->text_color(FL_SELECTION_COLOR);
  
  _p->in_mark =
    new Tool_Button("tool_start");
  
  _p->in_reset =
    new Tool_Button("tool_x");

  _p->out =
    new widget::Frame_Widget;
  _p->out->text_color(FL_SELECTION_COLOR);
  
  _p->out_mark =
    new Tool_Button("tool_end");
  
  _p->out_reset =
    new Tool_Button("tool_x");
  
  // Layout.
  
  _p->layout_toolbar = new Layout_H;
  _p->layout_toolbar->margin(0);
  toolbar->insert(_p->layout_toolbar, 0);

  _p->layout_playback_group = new Frame_Group_H(List<Frame *>() <<
    _p->reverse <<
    _p->stop <<
    _p->forward <<
    _p->playback_shuttle);

  _p->layout_frame_group = new Frame_Group_H(List<Frame *>() <<
    _p->start_button <<
    _p->prev <<
    _p->next <<
    _p->end_button <<
    _p->frame_shuttle);

  _p->layout_in_group = new Frame_Group_H(List<Frame *>() <<
    _p->in_mark <<
    _p->in_reset);
  _p->layout_out_group = new Frame_Group_H(List<Frame *>() <<
    _p->out_reset <<
    _p->out_mark);
  
  _p->window_handle_spacer = new Layout_Spacer;
  
  layout_update();
  
  // Initialize.
  
  playback_update();
  time_update();
  speed_update();
  frame_update();
  widget_update();
  
  // Playback callbacks.
  
  _p->stop->signal.set(this, stop_callback);
  _p->forward->signal.set(this, forward_callback);
  _p->reverse->signal.set(this, reverse_callback);
  
  _p->playback_shuttle->signal.set(this, playback_shuttle_callback);
  _p->playback_shuttle->value_signal.set(this, playback_shuttle_value_callback);
  
  _p->loop->signal.set(this, loop_callback);
  
  // Speed callbacks.
  
  _p->speed->signal.set(this, speed_callback);
  _p->every_frame->signal.set(this, every_frame_callback);
  
  // Frame callbacks.
  
  _p->frame->signal.set(this, frame_stop_callback);
  _p->frame_slider->signal.set(this, frame_slider_callback);
  _p->frame_slider->push_signal.set(this, frame_push_callback);
  _p->frame_shuttle->signal.set(this, frame_shuttle_callback);
  _p->frame_shuttle->signal.set(this, frame_push_callback);
  _p->frame_shuttle->value_signal.set(this, frame_shuttle_value_callback);

  _p->start_button->signal.set(this, start_callback);
  _p->start_button->push_signal.set(this, frame_push_callback);
  _p->end_button->signal.set(this, end_callback);
  _p->end_button->push_signal.set(this, frame_push_callback);

  _p->prev->signal.set(this, prev_callback);
  _p->prev->push_signal.set(this, frame_push_callback);
  _p->next->signal.set(this, next_callback);
  _p->next->push_signal.set(this, frame_push_callback);
  
  // In/out callbacks.
  
  _p->in_out->signal.set(this, in_out_callback);
  
  _p->in->signal.set(this, in_callback);
  _p->in_mark->signal.set(this, in_mark_callback);
  _p->in_reset->signal.set(this, in_reset_callback);
  
  _p->out->signal.set(this, out_callback);
  _p->out_mark->signal.set(this, out_mark_callback);
  _p->out_reset->signal.set(this, out_reset_callback);
  
  // Callbacks.
  
  prefs()->every_frame_signal.set(this, every_frame_callback);
  prefs()->layout_signal.set(this, layout_callback);
}

Group::~Group()
{
  global()->timer_del(this);
}

void Group::menu_update(djv_ui::menu::Group * in)
{
  using namespace djv_ui::menu;
  
  DJV_DEBUG("Group::menu_update");
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();

  // Menu:
  //
  // * Stop
  // * Forward
  // * Reverse
  // * Loop
  //   * ...
  // * Every Frame
  //   ---
  // * Start
  // * Start Absolute
  // * End
  // * End Absolute
  // * Previous
  // * Previous X10
  // * Previous X100
  // * Next
  // * Next X10
  // * Next X100
  //   ---
  // * In/Out Points
  // * In Mark
  // * In Reset
  // * Out Mark
  // * Out Reset
  //   ---
  // * Layout
  //   * ...
  
  in->add(menu_title, 0, 0, 0, SUB_MENU);
  
  in->add(
    menu_stop,
    shortcut[shortcut::PLAYBACK_STOP].value,
    _stop_callback,
    this,
    RADIO,
    STOP == _playback
  );
  
  in->add(
    menu_forward,
    shortcut[shortcut::PLAYBACK_FORWARD].value,
    _forward_callback,
    this,
    RADIO,
    FORWARD == _playback
  );
  
  in->add(
    menu_reverse,
    shortcut[shortcut::PLAYBACK_REVERSE].value,
    _reverse_callback,
    this,
    RADIO,
    REVERSE == _playback
  );

  in->add(menu_loop, 0, 0, 0, SUB_MENU);
  _p->menu_loop = in->add(
    label_loop(),
    List<int>(),
    _loop_callback,
    this,
    RADIO,
    _loop
  );
  in->end();

  in->add(
    menu_every_frame,
    0,
    _every_frame_callback,
    this,
    TOGGLE | DIVIDER,
    _every_frame
  );

  in->add(
    menu_start,
    shortcut[shortcut::PLAYBACK_START].value,
    _start_callback,
    this
  );
  
  in->add(
    menu_start_abs,
    shortcut[shortcut::PLAYBACK_START_ABS].value,
    _start_abs_callback,
    this
  );
  
  in->add(
    menu_end,
    shortcut[shortcut::PLAYBACK_END].value,
    _end_callback,
    this
  );
  
  in->add(
    menu_end_abs,
    shortcut[shortcut::PLAYBACK_END_ABS].value,
    _end_abs_callback,
    this
  );

  in->add(
    menu_prev,
    shortcut[shortcut::PLAYBACK_PREV].value,
    _prev_callback,
    this
  );
  
  in->add(
    menu_prev_x10,
    shortcut[shortcut::PLAYBACK_PREV_X10].value,
    _prev_x10_callback,
    this
  );
  
  in->add(
    menu_prev_x100,
    shortcut[shortcut::PLAYBACK_PREV_X100].value,
    _prev_x100_callback,
    this
  );

  in->add(
    menu_next,
    shortcut[shortcut::PLAYBACK_NEXT].value,
    _next_callback,
    this
  );
  
  in->add(
    menu_next_x10,
    shortcut[shortcut::PLAYBACK_NEXT_X10].value,
    _next_x10_callback,
    this
  );
  
  in->add(
    menu_next_x100,
    shortcut[shortcut::PLAYBACK_NEXT_X100].value,
    _next_x100_callback,
    this,
    DIVIDER
  );
  
  in->add(
    menu_in_out,
    shortcut[shortcut::PLAYBACK_IN_OUT].value,
    _in_out_callback,
    this,
    TOGGLE,
    _in_out
  );
  
  in->add(
    menu_in_mark,
    shortcut[shortcut::PLAYBACK_IN_MARK].value,
    _in_mark_callback,
    this
  );
  
  in->add(
    menu_in_reset,
    shortcut[shortcut::PLAYBACK_IN_RESET].value,
    _in_reset_callback,
    this
  );
  
  in->add(
    menu_out_mark,
    shortcut[shortcut::PLAYBACK_OUT_MARK].value,
    _out_mark_callback,
    this
  );
  
  in->add(
    menu_out_reset,
    shortcut[shortcut::PLAYBACK_OUT_RESET].value,
    _out_reset_callback,
    this,
    DIVIDER
  );

  in->add(menu_layout, 0, 0, 0, SUB_MENU);
  _p->menu_layout = in->add(
    label_layout(),
    List<int>(),
    _layout_callback,
    this,
    RADIO,
    _layout
  );
  in->end();

  in->end();
}

void Group::toolbar_update()
{
  DJV_DEBUG("Group::toolbar_update");
  
  // Update shortcuts.
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
  
  _p->loop->shortcut(shortcut[shortcut::PLAYBACK_LOOP].value);
  
  // Update tooltips.
  
  _p->stop->tooltip(string::Format(tooltip_stop).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_STOP].value)));
  _p->forward->tooltip(string::Format(tooltip_forward).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_FORWARD].value)));
  _p->reverse->tooltip(string::Format(tooltip_reverse).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_REVERSE].value)));

  _p->playback_shuttle->tooltip(tooltip_playback_shuttle);
  
  _p->loop->tooltip(string::Format(tooltip_loop).
    arg(string::label(_loop)).
    arg(string::join(label_loop(), ", ")).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_LOOP].value)));
  
  _p->speed->tooltip(tooltip_speed);
  _p->speed_real->tooltip(tooltip_speed_real);
  _p->every_frame->tooltip(tooltip_every_frame);

  _p->frame->tooltip(tooltip_frame);
  _p->frame_slider->tooltip(tooltip_frame_slider);
  _p->frame_shuttle->tooltip(tooltip_frame_shuttle);

  _p->start->tooltip(tooltip_start);
  _p->start_button->tooltip(string::Format(tooltip_start_button).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_START].value)));

  _p->end->tooltip(tooltip_end);
  _p->end_button->tooltip(string::Format(tooltip_end_button).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_END].value)));

  _p->prev->tooltip(string::Format(tooltip_prev).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_PREV].value)));
  _p->next->tooltip(string::Format(tooltip_next).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_NEXT].value)));

  _p->in_out->tooltip(string::Format(tooltip_in_out).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_IN_OUT].value)));
  _p->in->tooltip(tooltip_start);
  _p->in_mark->tooltip(string::Format(tooltip_in_mark).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_IN_MARK].value)));
  _p->in_reset->tooltip(string::Format(tooltip_in_reset).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_IN_RESET].value)));
  _p->out->tooltip(tooltip_end);
  _p->out_mark->tooltip(string::Format(tooltip_out_mark).
    arg(djv_ui::shortcut::label(shortcut[shortcut::PLAYBACK_OUT_MARK].value)));
  _p->out_reset->tooltip(string::Format(tooltip_out_reset).
    arg(djv_ui::shortcut::label(
      shortcut[shortcut::PLAYBACK_OUT_RESET].value)));

  _p->duration->tooltip(tooltip_duration);
}

void Group::toolbar_info_update(bool in)
{
  _p->window_handle_spacer->spacer_size(
    V2i(in ? 0 : djv_ui::layout::window_handle_size(), 0)
  );
}

void Group::playback(PLAYBACK in)
{
  if (in == _playback)
    return;

  DJV_DEBUG("Group::playback");
  DJV_DEBUG_PRINT("in = " << in);
  
  _playback = in;
  
  playback_update();
  
  update_signal.emit(true);
}

void Group::playback_callback(PLAYBACK in)
{
  _playback = (in == _playback) ? STOP : in;

  playback_update();
  
  update_signal.emit(true);
}

void Group::stop_callback(bool)
{
  playback_callback(STOP);
}

void Group::forward_callback(bool)
{
  playback_callback(FORWARD);
}

void Group::reverse_callback(bool)
{
  playback_callback(REVERSE);
}

void Group::_stop_callback()
{
  stop_callback(true);
}

void Group::_forward_callback()
{
  forward_callback(true);
}

void Group::_reverse_callback()
{
  reverse_callback(true);
}

void Group::playback_update()
{
  DJV_DEBUG("Group::playback_update");

  callbacks(false);
  
  switch (_playback)
  {
    case STOP: _p->stop->set(); break;
    case FORWARD: _p->forward->set(); break;
    case REVERSE: _p->reverse->set(); break;
  }
  
  _p->loop->set(_loop);

  if (LOOP_ONCE == _loop)
    switch (_playback)
    {
      case REVERSE:
        if (_frame == frame_min())
          _frame = frame_max();
        break;
      
      case FORWARD:
        if (_frame == frame_max())
          _frame = frame_min();
        break;
    
      default: break;
    }
  
  global()->timer_del(this);

  switch (_playback)
  {
    case REVERSE:
    case FORWARD:
    
      _idle_init = true;
      
      global()->timer_add(this);
      
      break;
    
    case STOP: break;
  }

  callbacks(true);
}

void Group::loop(LOOP in)
{
  if (in == _loop)
    return;

  DJV_DEBUG("Group::loop");
  DJV_DEBUG_PRINT("in = " << in);
  
  _loop = in;
  
  playback_update();
  update_signal.emit(true);
}

void Group::loop_callback(int in)
{
  loop(static_cast<LOOP>(in));
}

void Group::_loop_callback()
{
  loop_callback(list::find(_p->menu->item(), _p->menu_loop));
}

namespace {

int64_t time_max(const djv_image::Time & in)
{
  return in.list.size() ? (static_cast<int64_t>(in.list.size()) - 1) : 0;
}

}

void Group::time(const djv_image::Time & time)
{
  DJV_DEBUG("Group::time");
  DJV_DEBUG_PRINT("time = " << time);
  
  _time = time;
  _in = 0;
  _out = time_max(_time);
  _speed = time.speed;
  _speed_real = 0.0;
  
  time_update();
  speed_update();
}

const djv_image::Time & Group::time() const
{
  return _time;
}

void Group::time_update()
{
  DJV_DEBUG("Group::time_update");
  
  callbacks(false);
  
  const int64_t max = time_max(_time);
  
  _p->frame->list(_time.list);
  _p->frame_slider->range(0, max);
  _p->frame_slider->in_out(_in_out);
  _p->frame_slider->in_out(_in, _out);

  _p->in_out->set(_in_out);
  
  if (_in_out && _in != 0)
  {
    _p->in->list(_time.list);
    _p->in->set(_in);
  }
  else
  {
    _p->start->set(_time.start());
  }

  if (_in_out && max != _out)
  {
    _p->out->list(_time.list);
    _p->out->set(_out);
  }
  else
  {
    _p->end->set(_time.end());
  }
  
  _p->duration->set(
    _in_out ?
    (_out - _in + 1) :
    (max + 1)
  );
  _p->duration->text_color(
    (_in_out && (_in != 0 || _out != max)) ?
    FL_SELECTION_COLOR :
    FL_FOREGROUND_COLOR
  );
  
  callbacks(true);
  
  _p->duration->update(); // XXX
}

void Group::speed_update()
{
  DJV_DEBUG("Group::speed_update");
  
  callbacks(false);
  
  _p->speed->set(_speed);
  _p->speed->set_default(_time.speed);
  _p->speed_real->set(_speed_real);
  _p->speed_real->dropped_frames(_dropped_frames);
  _p->every_frame->set(_every_frame);

  _p->frame->speed(_speed);
  _p->start->speed(_speed);
  _p->end->speed(_speed);
  _p->in->speed(_speed);
  _p->out->speed(_speed);
  _p->duration->speed(_speed);
  
  callbacks(true);
}

void Group::playback_shuttle_callback(bool in)
{
  if (in)
  {
    playback(STOP);
    
    _shuttle = true;
    _shuttle_speed = 0.0;
    _idle_init = true;

    global()->timer_del(this);
    global()->timer_add(this);
  }
  else
  {
    _shuttle = false;
    _dropped_frames = false;
  
    global()->timer_del(this);
    
    _p->speed->set(_speed);
  }
}

void Group::playback_shuttle_value_callback(int in)
{
  _shuttle_speed =
    math::pow(static_cast<double>(math::abs(in)), 1.5) *
    (in >= 0 ? 1.0 : -1.0);
  //_shuttle_speed = static_cast<double>(in);
  
  _idle_frame = uint64_t(_idle_timer.seconds() * _shuttle_speed);
}

void Group::speed(const djv_image::time::Speed & in)
{
  if (in == _speed)
    return;
  
  DJV_DEBUG("Group::playback_speed");
  DJV_DEBUG_PRINT("in = " << in);
  
  _speed = in;
  _speed_real = 0.0;
  _dropped_frames = false;
  
  playback_update();
  speed_update();
  
  overlay_signal.emit(true);
  update_signal.emit(true);
}

const djv_image::time::Speed & Group::speed() const
{
  return _speed; }

double Group::speed_real() const
{
  return _speed_real;
}

bool Group::dropped_frames() const
{
  return _dropped_frames;
}

void Group::speed_callback(const djv_image::time::Speed & in)
{
  speed(in);
}

void Group::every_frame_callback(bool in)
{
  DJV_DEBUG("Group::playback_every_frame_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  _every_frame = in;
  
  speed_update();
  
  overlay_signal.emit(true);
  update_signal.emit(true);
}

void Group::_every_frame_callback()
{
  every_frame_callback(_p->menu->value());
}

void Group::frame(int64_t in, bool in_out)
{
  const int64_t frame_in = in_out ? frame_min() : 0;
  const int64_t frame_out = in_out ? frame_max() : time_max(_time);
  
  switch (_loop)
  {
    case LOOP_ONCE:
      if (in <= frame_in || in >= frame_out)
        playback(STOP);
      in = math::clamp(in, frame_in, frame_out);
      break;
    
    case LOOP_REPEAT:
      in = math::wrap(in, frame_in, frame_out);
      break;
    
    case LOOP_PING_PONG:
      if (_playback != STOP)
      {
        if (in <= frame_in)
          playback(FORWARD);
        else if (in >= frame_out)
          playback(REVERSE);
      }
      in = math::clamp(in, frame_in, frame_out);
      break;
  }
  
  if (in == _frame)
    return;

  DJV_DEBUG("Group::frame");
  DJV_DEBUG_PRINT("in = " << in);
  
  _frame = in;
  
  frame_update();
  
  image_signal.emit(true);
}

int64_t Group::frame() const
{
  return _frame;
}

void Group::frame_callback(int64_t in)
{
  frame(in, _in_out);
}

void Group::frame_stop_callback(int64_t in)
{
  playback(STOP);
  
  frame_callback(in);
}

void Group::frame_slider_callback(int in)
{
  frame(in, false);
}

void Group::frame_shuttle_callback(bool in)
{
  _frame_tmp = _frame;
}

void Group::frame_shuttle_value_callback(int in)
{
  frame(_frame_tmp + in, _in_out);
}

void Group::start_callback(bool)
{
  frame_callback(_in);
}

void Group::_start_callback()
{
  start_callback(true);
}

void Group::start_abs_callback(bool)
{
  frame_slider_callback(0);
}

void Group::_start_abs_callback()
{
  start_abs_callback(true);
}

void Group::end_callback(bool)
{
  frame_callback(_out);
}

void Group::_end_callback()
{
  end_callback(true);
}

void Group::end_abs_callback(bool)
{
  frame_slider_callback(time_max(_time));
}

void Group::_end_abs_callback()
{
  end_abs_callback(true);
}

void Group::prev_callback(bool)
{
  frame_callback(_frame - 1);
}

void Group::_prev_callback()
{
  prev_callback(true);
}

void Group::_prev_x10_callback()
{
  frame_callback(_frame - 10);
}

void Group::_prev_x100_callback()
{
  frame_callback(_frame - 100);
}

void Group::next_callback(bool)
{
  frame_callback(_frame + 1);
}

void Group::_next_callback()
{
  next_callback(true);
}

void Group::_next_x10_callback()
{
  frame_callback(_frame + 10);
}

void Group::_next_x100_callback()
{
  frame_callback(_frame + 100);
}

void Group::frame_push_callback(bool in)
{
  DJV_DEBUG("Group::frame_push_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  _idle_pause = in;
  _idle_init = ! in;
}

void Group::frame_update()
{
  DJV_DEBUG("Group::frame_update");

  callbacks(false);
  
  _p->frame->set(_frame);
  _p->frame_slider->set(_frame);
  
  callbacks(true);
}

int64_t Group::in() const
{
  return _in;
}

void Group::in_out_callback(bool in)
{
  _in_out = in;
  
  //frame(_frame, _in_out);
  
  time_update();
  widget_update();
  
  update_signal.emit(true);
}

void Group::_in_out_callback()
{
  in_out_callback(! _in_out);
}

void Group::in_callback(int64_t in)
{
  _in = in;
  
  frame(_frame, _in_out);
  
  time_update();
  widget_update();
  
  update_signal.emit(true);
}

void Group::_in_goto_callback()
{
  frame(_in, _in_out);
}

void Group::in_mark_callback(bool)
{
  in_callback(_frame);
}

void Group::_in_mark_callback()
{
  in_mark_callback(true);
}

void Group::in_reset_callback(bool)
{
  in_callback(0);
}

void Group::_in_reset_callback()
{
  in_reset_callback(true);
}

int64_t Group::out() const
{
  return _out;
}

void Group::out_callback(int64_t in)
{
  _out = in;
  
  frame(_frame, _in_out);
  
  time_update();
  widget_update();
  
  update_signal.emit(true);
}

void Group::_out_goto_callback()
{
  frame(_out, _in_out);
}

void Group::out_mark_callback(bool)
{
  out_callback(_frame);
}

void Group::_out_mark_callback()
{
  out_mark_callback(true);
}

void Group::out_reset_callback(bool)
{
  out_callback(time_max(_time));
}

void Group::_out_reset_callback()
{
  out_reset_callback(true);
}

int64_t Group::frame_min() const
{
  return math::max(static_cast<int64_t>(0), _in);
}

int64_t Group::frame_max() const
{
  return math::min(time_max(_time), _out);
}

void Group::idle()
{
  if (_idle_pause)
    return;

  DJV_DEBUG("Group::idle");
  DJV_DEBUG_PRINT("playback = " << _playback);
  DJV_DEBUG_PRINT("loop = " << _loop);
  DJV_DEBUG_PRINT("every frame = " << _every_frame);
  DJV_DEBUG_PRINT("frame = " << _frame);

  if (_idle_init)
  {
    DJV_DEBUG_PRINT("init");

    _idle_timer.start();
    _idle_frame = 0;
    _speed_timer = _idle_timer;
    _speed_counter = 0;
    _dropped_frames = false;
    _dropped_frames_tmp = false;
    _idle_init = false;
  }
  else
  {
    _idle_timer.check();
  }
  
  const double speed =
    _shuttle ? _shuttle_speed : djv_image::time::speed_to_float(_speed);
  
  DJV_DEBUG_PRINT("speed = " << speed);

  // XXX Overflow?

  const uint64_t absolute_frame = uint64_t(_idle_timer.seconds() * speed);
  int inc = static_cast<int>(absolute_frame - _idle_frame);
  
  DJV_DEBUG_PRINT("absolute frame = " << static_cast<int>(absolute_frame));
  DJV_DEBUG_PRINT("inc = " << inc);
  
  if (! inc)
    return;

  // Calculate current frame.

  _idle_frame = absolute_frame;
  
  if (REVERSE == _playback)
    inc = -inc;
  
  const bool every_frame = _every_frame && ! _shuttle;
  
  if (every_frame)
    inc = inc > 0 ? 1 : (inc < 0 ? -1 : 0);
  else
    _dropped_frames_tmp |= inc > 1 || inc < -1;
  
  _speed_counter += math::abs(inc);
  
  // Calculate real playback speed.

  _speed_timer.check();
  
  if (_speed_timer.seconds() >= 1.0)
  {
    _speed_real = _speed_counter / _speed_timer.seconds();
    _speed_timer.start();

    _dropped_frames = _dropped_frames_tmp;
    _dropped_frames_tmp = false;
    _speed_counter = 0;

    _p->speed_real->set(_speed_real);
    _p->speed_real->dropped_frames(every_frame ? false : _dropped_frames);
  }

  frame(_frame + inc, _in_out);
}

void Group::cached_frames(const List<int64_t> & in)
{
  _p->frame_slider->cached_frames(in);
}

void Group::layout(LAYOUT in)
{
  if (in == _layout)
    return;

  //DJV_DEBUG("Group::layout");
  //DJV_DEBUG_PRINT("in = " << in);
  
  _layout = in;
  
  layout_update();
  update_signal.emit(true);
}

void Group::layout_callback(LAYOUT in)
{
  layout(in);
}

void Group::_layout_callback()
{
  layout_callback(
    static_cast<LAYOUT>(list::find(_p->menu->item(), _p->menu_layout))
  );
}

void Group::widget_update()
{
  DJV_DEBUG("Group::widget_update");
  
  callbacks(false);
  
  const int64_t max = time_max(_time);
  
  switch (_layout)
  {
    case LAYOUT_DEFAULT:
    case LAYOUT_LEFT:
    case LAYOUT_CENTER:
    
      _p->layout_playback_group->show();
      _p->layout_frame_group->show();
      _p->speed->show();
      _p->speed_real->show();
      _p->every_frame->show();
      _p->loop->show();
      _p->in_out->show();
      
      _p->frame_slider->show();
      _p->frame->show();
      _p->layout_in_group->show();

      if (_in_out && _in != 0)
      {
        _p->in->show();
        _p->start->hide();
      }
      else
      {
        _p->start->show();
        _p->in->hide();
      }
    
      if (_in_out && max != _out)
      {
        _p->out->show();
        _p->end->hide();
      }
      else
      {
        _p->end->show();
        _p->out->hide();
      }
      
      _p->layout_out_group->show();
      _p->duration->show();

      break;
    
    case LAYOUT_MINIMAL:
    
      _p->layout_playback_group->show();
      _p->layout_frame_group->hide();
      _p->speed->hide();
      _p->speed_real->hide();
      _p->every_frame->hide();
      _p->loop->hide();
      _p->in_out->hide();
      
      _p->frame_slider->show();
      _p->frame->hide();
      _p->layout_in_group->hide();
      _p->in->hide();
      _p->start->hide();
      _p->end->hide();
      _p->out->hide();
      _p->layout_out_group->hide();
      _p->duration->hide();
      
      break;
  }
  
  callbacks(true);
}

void Group::layout_update()
{
  using namespace djv_ui;
  
  //DJV_DEBUG("Group::layout_update");
  
  widget_update();
  
  callbacks(false);
  
  Layout_V * layout_v = 0;
  Layout_H * layout_h = 0;
  Layout_H * layout_h2 = 0;
  
  Layout_Item * item = _p->layout_toolbar->remove_child(_p->loop);
  
  //DJV_DEBUG_PRINT("remove loop = " << long(item));
  
  _p->layout_toolbar->remove_child(_p->speed);
  _p->layout_toolbar->remove_child(_p->speed_real);
  _p->layout_toolbar->remove_child(_p->every_frame);

  _p->layout_toolbar->remove_child(_p->frame);
  _p->layout_toolbar->remove_child(_p->frame_slider);
  _p->layout_toolbar->remove_child(_p->start);
  _p->layout_toolbar->remove_child(_p->end);
  _p->layout_toolbar->remove_child(_p->duration);

  _p->layout_toolbar->remove_child(_p->in_out);
  _p->layout_toolbar->remove_child(_p->in);
  _p->layout_toolbar->remove_child(_p->out);

  _p->layout_toolbar->remove_child(_p->layout_playback_group);
  _p->layout_toolbar->remove_child(_p->layout_frame_group);
  _p->layout_toolbar->remove_child(_p->layout_in_group);
  _p->layout_toolbar->remove_child(_p->layout_out_group);
  item = _p->layout_toolbar->remove_child(_p->window_handle_spacer);
  
  //DJV_DEBUG_PRINT("window handle spacer = " << long(item));
  
  _p->layout_toolbar->del();
  
  switch (_layout)
  {
    case LAYOUT_DEFAULT:
    case LAYOUT_LEFT:
    {
      Layout_V * left_layout = new Layout_V(_p->layout_toolbar);
      left_layout->margin(0);
      layout_h = new Layout_H(left_layout);
      layout_h->margin(0);
      layout_h->add(_p->layout_playback_group);
      layout_h->add(_p->layout_frame_group);
      layout_h = new Layout_H(left_layout);
      layout_h->margin(0);
      layout_h->add(_p->speed);
      layout_h->add(_p->speed_real);
      layout_h->add(_p->every_frame);
      layout_h->add_spacer(-1, true);
      layout_h->add(_p->loop);
      layout_h->add(_p->in_out);
      
      Layout_V * right_layout = new Layout_V(_p->layout_toolbar);
      right_layout->margin(0);
      _p->layout_toolbar->stretch(right_layout);
      layout_h = new Layout_H(right_layout);
      layout_h->margin(0);
      layout_h->add(_p->frame_slider);
      layout_h->stretch(_p->frame_slider);
      right_layout->add_stretch();
      layout_h = new Layout_H(right_layout);
      layout_h->margin(0);
      layout_h->add(_p->frame);
      layout_h2 = new Layout_H(layout_h);
      layout_h2->spacing(0);
      layout_h2->margin(0);
      layout_h2->add(_p->layout_in_group);
      layout_h2->add(_p->in);
      layout_h2->add(_p->start);
 
      if (LAYOUT_DEFAULT == _layout)
        layout_h->add_spacer(-1, true);
      
      layout_h2 = new Layout_H(layout_h);
      layout_h2->spacing(0);
      layout_h2->margin(0);
      layout_h2->add(_p->end);
      layout_h2->add(_p->out);
      layout_h2->add(_p->layout_out_group);
      layout_h->add(_p->duration);
      
      if (LAYOUT_LEFT == _layout)
        layout_h->add_stretch();
      
      layout_h->add(_p->window_handle_spacer);
    }
    break;
    
    case LAYOUT_CENTER:
    {
      Layout_V * layout = new Layout_V(_p->layout_toolbar);
      layout->margin(0);
      _p->layout_toolbar->stretch(layout);
      
      layout->add(_p->frame_slider);
      
      Layout_H * layout1 = new Layout_H(layout);
      layout1->size(layout_item::FIXED, layout_item::FIXED);
      layout1->align(layout_item::CENTER, layout_item::CENTER);
      layout1->margin(0);
      layout1->add(_p->frame);
      layout_h = new Layout_H(layout1);
      layout_h->spacing(0);
      layout_h->margin(0);
      layout_h->add(_p->layout_in_group);
      layout_h->add(_p->in);
      layout_h->add(_p->start);
      layout1->add(_p->layout_playback_group);
      layout1->add(_p->layout_frame_group);
      layout_h = new Layout_H(layout1);
      layout_h->spacing(0);
      layout_h->margin(0);
      layout_h->add(_p->end);
      layout_h->add(_p->out);
      layout_h->add(_p->layout_out_group);
      layout1->add(_p->duration);
      layout1->add(_p->window_handle_spacer);
    }
    break;
    
    case LAYOUT_MINIMAL:
    {
      _p->layout_toolbar->add(_p->layout_playback_group);
      _p->layout_toolbar->add(_p->frame_slider);
      _p->layout_toolbar->stretch(_p->frame_slider);
      _p->layout_toolbar->add(_p->window_handle_spacer);
    }
    break;
  }
  
  _p->layout_toolbar->dirty();
  _p->layout_toolbar->update();
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}
}

