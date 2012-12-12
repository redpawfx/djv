/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_dialog.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_browser_private.h"

#include <djv_ui/frame.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/push_button.h>
#include <djv_ui/shortcut.h>
#include <djv_base/directory.h>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
// Dialog
//------------------------------------------------------------------------------

Dialog::Dialog(Prefs_Instance * prefs) :
  dialog::Base(dialog_title),
  signal(this),
  _prefs(
    prefs ? prefs : new Prefs_Instance(prefs::prefs_global(), "file_browser")
  ),
  _image(_prefs->image()),
  _seq(_prefs->seq()),
  _hidden(_prefs->hidden()),
  _sort(_prefs->sort()),
  _sort_reverse(_prefs->sort_reverse()),
  _sort_directory(_prefs->sort_directory()),
  _type(_prefs->type()),
  _pin(_prefs->pin()),
  _p(new _Dialog)
{
  debug_path("file_browser::Dialog");
  
  DJV_DEBUG("Dialog::Dialog");
  
  // Create widgets.

  _p->menu =
    new menu::Menu;

  _p->value =
    new Text_Edit;

  _p->up =
    new Tool_Button("directory_up");
  _p->prev =
    new Tool_Button("directory_prev");
  _p->reload =
    new Tool_Button("directory_reload");
  _p->seq =
    new Choice(file_seq::label_compress());
  Label * seq_label =
    new Label(label_seq);

  _p->browser =
    new Browser;
  _p->columns =
    new columns::Columns;
  _p->columns->name_list(label_sort());
  
  _p->pin =
    new Check_Button(dialog::label_pin);
  _p->pin->tooltip(dialog::tooltip_pin);
  Push_Button * ok_button =
    new Push_Button(dialog::label_ok);
  Push_Button * close_button =
    new Push_Button(dialog::label_close);
  
  _p->browser_frame =
    new Frame;
  _p->browser_frame->style(frame::STYLE_BORDER);
  
  // Layout.
  
  Layout_H * layout_h = 0;
  Layout_H * layout_h2 = 0;
  Layout_V * layout_v = 0;

  Layout_V * layout_window = new Layout_V(this);
  layout_window->spacing(0);
  layout_window->margin(0);
  layout_window->add(_p->menu);
  Layout_V * layout = new Layout_V(layout_window);
  layout_window->stretch(layout);

  layout_h = new Layout_H(layout);
  layout_h->spacing(style::global()->spacing_large());
  layout_h->margin(0);
  layout_h->add(new Frame_Group_H(List<Frame *>() <<
    _p->up <<
    _p->prev <<
    _p->reload));
  layout_h2 = new Layout_H(layout_h);
  layout_h2->margin(0);
  layout_h2->add(seq_label);
  layout_h2->add(_p->seq);
  
  layout->add(_p->browser_frame);
  layout->stretch(_p->browser_frame);
  layout_v = new Layout_V(_p->browser_frame);
  layout_v->spacing(0);
  layout_v->margin(0);
  layout_v->add(_p->columns);
  layout_v->add(_p->browser);
  layout_v->stretch(_p->browser);
  
  layout->add(_p->value);
  
  layout_h = new Layout_H(layout);
  layout_h->spacing(style::global()->spacing_large());
  layout_h->margin(0);
  layout_h->add(_p->pin);
  layout_h->add_spacer(-1, true);
  layout_h2 = new Layout_H(layout_h);
  layout_h2->margin(0);
  layout_h2->add(ok_button);
  layout_h2->add(close_button);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.
  
  _value = djv_base::file::path_absolute(".");
  _directory_prev = _value.path();
  
  //directory_update(); // XXX Can take a long time.
  widget_update();
  type_update();
  menu_update();
  _p->pin->set(_pin);
  
  size(V2i(650, 650));

  fl_font(style::global()->font(), style::global()->font_size());
  _p->columns->size_list(columns_size_list());
  _p->browser->column_list(_p->columns->size_list());
  
  // Callbacks.

  _p->value->signal.set(this, value_callback);
  _p->value->action_signal.set(this, value_action_callback);
  _p->value->change_signal.set(this, value_change_callback);
  _p->columns->signal.set(this, columns_callback);
  _p->columns->action_signal.set(this, columns_action_callback);
  _p->browser->signal.set(this, browser_callback);
  _p->browser->mouse_signal.set(this, browser_mouse_callback);
  _p->browser->action_signal.set(this, browser_action_callback);

  _p->up->signal.set(this, up_callback);
  _p->prev->signal.set(this, prev_callback);
  _p->reload->signal.set(this, reload_callback);
  _p->seq->signal.set(this, seq_callback);
  
  _p->pin->signal.set(this, pin_callback);
  ok_button->signal.set(this, ok_callback);
  close_button->signal.set(this, close_callback);
}

Dialog::~Dialog()
{}
  
void Dialog::del()
{
  _prefs.reset();
  
  dialog::Base::del();
}

void Dialog::set(const File & in)
{
  if (in == _value)
    return;
  
  DJV_DEBUG("Dialog::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  const String directory = _value.path();

  _value = in;

  if (_seq && _value.is_seq_valid())
    _value.type(djv_base::file::SEQ);

  if (_value.path() != directory)
  {
    _directory_prev = directory;
    
    directory_update();
  }
  
  widget_update();
}

const File & Dialog::get() const
{
  return _value;
}

void Dialog::image(IMAGE in)
{
  if (in == _image)
    return;
  
  _image = in;
  
  _prefs->image(_image);
  
  directory_update();
  menu_update();
}

IMAGE Dialog::image() const
{
  return _image;
}

void Dialog::seq(file_seq::COMPRESS in)
{
  if (in == _seq)
    return;
  
  _seq = in;
  
  _prefs->seq(_seq);
  
  directory_update();
  widget_update();
  menu_update();
}

file_seq::COMPRESS Dialog::seq() const
{
  return _seq;
}

void Dialog::type(int in)
{
  if (in == _type)
    return;
  
  _type = in;

  _prefs->type(_type);
  
  directory_update();
  type_update();
  menu_update();
}

int Dialog::type() const
{
  return _type;
}

void Dialog::hidden(bool in)
{
  if (in == _hidden)
    return;
  
  _hidden = in;
  
  _prefs->hidden(_hidden);
  
  directory_update();
  menu_update();
}

bool Dialog::hidden() const
{
  return _hidden;
}

void Dialog::sort(SORT in)
{
  if (in == _sort)
    return;
  
  _sort = in;
  
  _prefs->sort(_sort);
  
  directory_update();
  menu_update();
}

SORT Dialog::sort() const
{
  return _sort;
}

void Dialog::sort_reverse(bool in)
{
  if (in == _sort_reverse)
    return;
  
  _sort_reverse = in;
  
  _prefs->sort_reverse(_sort_reverse);
  
  directory_update();
  menu_update();
}

bool Dialog::sort_reverse() const
{
  return _sort_reverse;
}

void Dialog::sort_directory(bool in)
{
  if (in == _sort_directory)
    return;
  
  _sort_directory = in;
  
  _prefs->sort_directory(_sort_directory);
  
  directory_update();
  menu_update();
}

bool Dialog::sort_directory() const
{
  return _sort_directory;
}

void Dialog::pin(bool in)
{
  if (in == _pin)
    return;
  
  _pin = in;
  
  _prefs->pin(_pin);
  
  _p->pin->set(_pin);
}

bool Dialog::pin() const
{
  return _pin;
}

void Dialog::show()
{
  DJV_DEBUG("Dialog::show");
  
  if (! shown())
  {
    //_value = djv_base::file::path_absolute(".");
  
    directory_update();
    widget_update();

    _p->value->take_focus();
  }

  dialog::Base::show();
}

void Dialog::dirty()
{
  DJV_DEBUG("Dialog::dirty");
  
  dialog::Base::dirty();
  
  // Update tooltips.

  const List<shortcut::Shortcut> & shortcut = prefs()->shortcut();

  _p->value->tooltip(tooltip_value);
  
  _p->up->tooltip(string::Format(tooltip_up).
    arg(shortcut::label(shortcut[Prefs::UP].value)));
  _p->prev->tooltip(string::Format(tooltip_prev).
    arg(shortcut::label(shortcut[Prefs::PREV].value)));
  _p->reload->tooltip(string::Format(tooltip_reload).
    arg(shortcut::label(shortcut[Prefs::RELOAD].value)));

  // General updates.
  
  if (visible() && shown())
    directory_update();
  menu_update();
}

void Dialog::directory_update()
{
  DJV_DEBUG("Dialog::directory_update");
  DJV_DEBUG_PRINT("value = " << _value);
  
  callbacks(false);
  
  // Set busy cursor.
  
  base::Cursor cursor(FL_CURSOR_WAIT);
  
  // Get directory contents.
  
  const Directory directory(_value.path());
  
  List<File> list = directory.list();
  
  // File sequence directory contents.
  
  djv_base::file::seq_compress(list, _seq);
  
  // Filter directory contents.
  
  if (! _hidden || _type != -1)
  {
    const file::FILTER filter =
      ! _hidden ? djv_base::file::FILTER_HIDDEN : djv_base::file::FILTER_NONE;
    
    List<String> glob;
    if (
      _type != -1 &&
      _type < static_cast<int>(_type_glob.size())
    ) {
      const List<String> tmp = string::split(_type_glob[_type], ',');
      for (list::size_type i = 0; i < tmp.size(); ++i)
        glob += tmp[i];
    }
    
    DJV_DEBUG_PRINT("filter = " << filter);
    DJV_DEBUG_PRINT("glob = " << glob);
    
    djv_base::file::filter(list, filter, glob, true);
  }
  
  // Sort directory contents.
  
  djv_base::file::SORT sort = djv_base::file::SORT(0);
  
  switch (_sort)
  {
    case SORT_NAME: sort = djv_base::file::SORT_NAME; break;
    case SORT_TYPE: sort = djv_base::file::SORT_TYPE; break;
    case SORT_SIZE: sort = djv_base::file::SORT_SIZE; break;
#if ! defined(DJV_WINDOWS)
    case SORT_USER: sort = djv_base::file::SORT_USER; break;
#endif
    case SORT_TIME: sort = djv_base::file::SORT_TIME; break;
    default: break;
  }
  djv_base::file::sort(list, sort, _sort_reverse);
  
  if (_sort_directory)
    djv_base::file::sort_directories_first(list);
  
  // Add parent directory.
  
  if (directory.is_valid())
    list.push_front(File(_value.path() + ".."));
  
  // Update browser widgets.
  
  _p->columns->sort(_sort);
  _p->columns->sort_reverse(_sort_reverse);
  _p->browser->set(list, _image);

  callbacks(true);
}

void Dialog::widget_update()
{
  DJV_DEBUG("Dialog::widget_update");
  DJV_DEBUG_PRINT("value = " << _value);
  DJV_DEBUG_PRINT("value seq = " << _value.seq());
  
  callbacks(false);
  
  _p->value->set(_value);
  
  _p->seq->set(_seq);

  callbacks(true);
}

namespace {

String _glob(const List<String> & in)
{
  List<String> tmp;
  for (list::size_type i = 0; i < in.size(); ++i)
    tmp += string::Format("*%%").arg(in[i]);
  
  return string::join(tmp, ',');
}

List<String> glob(const List<djv_image::io::Base *> & in)
{
  List<String> out;
  
  List<String> all;
  for (list::size_type i = 0; i < in.size(); ++i)
    all += in[i]->extension_list();
  out += _glob(all);
  
  for (list::size_type i = 0; i < in.size(); ++i)
    out += _glob(in[i]->extension_list());
  
  return out;
}

List<String> glob_label(const List<djv_image::io::Base *> & in)
{
  List<String> out;
  
  out += label_type_all;
  out += label_type_all_image;
  
  for (list::size_type i = 0; i < in.size(); ++i)
    out += string::Format("%% (%%)").
      arg(in[i]->name()).
      arg(string::join(in[i]->extension_list(), ", "));
  
  return out;
}

}

void Dialog::type_update()
{
  DJV_DEBUG("Dialog::type_update");
  
  callbacks(false);
  
  // XXX What about image save plugins?
  
  const List<djv_image::io::Base *> load = djv_image::io::global()->load_list();
  
  _type_glob = glob(load);
  _type_label = glob_label(load);
      
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

