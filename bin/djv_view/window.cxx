/*------------------------------------------------------------------------------
 bin/djv_view/window.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "window.h"

#include "application.h"
#include "file_group.h"
#include "file_prefs.h"
#include "file_save.h"
#include "help_group.h"
#include "image_display_profile.h"
#include "image_group.h"
#include "playback_group.h"
#include "playback_prefs.h"
#include "shortcut.h"
#include "tool_color_picker.h"
#include "tool_group.h"
#include "tool_histogram.h"
#include "tool_info.h"
#include "tool_magnify.h"
#include "view_group.h"
#include "view_prefs.h"
#include "view_widget.h"
#include "window_prefs.h"
#include <djv_ui/color.h>
#include <djv_ui/dialog.h>
#include <djv_ui/icon.h>
#include <djv_ui/image_io.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/menu.h>
#include <djv_ui/shortcut.h>
#include <djv_ui/style.h>
#include <djv_ui/tool_button.h>
#include <djv_image/gl_offscreen.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/x.H>

#if defined(DJV_WINDOWS)
#include "../../lib/djv_ui/winrc.h"
#elif defined(DJV_MACOS)
#elif defined(DJV_X11)
#include <X11/xpm.h>
#include "../../etc/X11/projector32x32.xpm"
#endif

namespace djv_bin {
namespace view {
namespace window {

//------------------------------------------------------------------------------
// Info_Swatch
//------------------------------------------------------------------------------

namespace {

class Info_Swatch : public djv_ui::Color_Swatch
{
public:

  Info_Swatch()
  {
    Fl_Group::set_visible_focus();
    
    highlight(true);
  
    action_signal.set(this, dialog_callback);
  }

  void dirty()
  {
    using namespace djv_ui;
    
    Color_Swatch::dirty();
    
    fl_font(style::global()->font(), style::global()->font_size());
    
    size_hint(V2i(fl_height()));
  }
  
private:

  DJV_CALLBACK(Info_Swatch, dialog_callback, bool)
  {
    djv_ui::dialog::global()->color(get(), this, 0);
  }
};

}

//------------------------------------------------------------------------------
// Window::_Window
//------------------------------------------------------------------------------

struct Window::_Window
{
  _Window() :
    menu(0)
  {}

  djv_ui::menu::Menu * menu;
  
  djv_ui::Frame * toolbar0;
  
  djv_ui::Tool_Button * window_new;
  djv_ui::Tool_Button * window_copy;
  djv_ui::Tool_Button * window_close;
  djv_ui::Tool_Button * window_fit;
  djv_ui::Tool_Button * window_fullscreen;
  
  djv_ui::Frame * toolbar1;
  djv_ui::Widget * toolbar1_playback;
  djv_ui::Widget * toolbar1_info;
  
  Info_Swatch * info_swatch;
  djv_ui::Label * info_pixel;
  djv_ui::Label * info_image;
  djv_ui::Label * info_cache;
  
  view::Widget * view;

  std::auto_ptr<djv_image::gl_offscreen::Buffer> sample_buffer;
  //std::auto_ptr<djv_image::gl_data::Data> sample_data;
};

//------------------------------------------------------------------------------
// Window
//------------------------------------------------------------------------------

Window::Window(const Window * copy) :
  djv_ui::window::Window(application::global()->name()),
  _mouse_wheel(static_cast<input::MOUSE_WHEEL>(0)),
  _window_fullscreen(false),
  _window_toolbar(copy ? copy->_window_toolbar : prefs()->toolbar()),
  _image_p(0),
  _image_tmp(copy ? copy->_image_tmp : djv_image::Image()),
  _image_pick(copy ? copy->_image_pick : V2i()),
  _playback_frame_tmp(0),
  _playback_speed_tmp(0.0),
  _p(new _Window),
  _is_valid(false)
{
  using namespace djv_ui;
  
  DJV_DEBUG("Window::Window");

#if defined(DJV_WINDOWS)

  HICON icon = LoadIcon(fl_display, MAKEINTRESOURCE(DJV_ICON));
  if (! icon)
  {
    DJV_DEBUG_PRINT("error = " << error::last_error());
  }
  else
  {
    Fl_Double_Window::icon((char *)icon);
  }

#endif
  
  arrow_navkeys(false);

  // Create widgets.
  
  _p->menu =
    new menu::Menu;
  
  _p->toolbar0 =
    new Frame;
  _p->toolbar0->style(frame::STYLE_EMPTY);
  
  _p->window_new =
    new Tool_Button("window_new");
  _p->window_copy =
    new Tool_Button("window_copy");
  _p->window_close =
    new Tool_Button("window_close");
  _p->window_fit =
    new Tool_Button("window_fit");
  _p->window_fullscreen =
    new Tool_Button("window_fullscreen");
  _p->window_fullscreen->type(tool_button::TOGGLE);

  _p->toolbar1 =
    new Frame;
  _p->toolbar1->style(frame::STYLE_EMPTY);
  _p->toolbar1_playback =
    new widget::Widget;
  _p->toolbar1_info =
    new widget::Widget;

  _p->info_swatch =
    new Info_Swatch;
  _p->info_pixel =
    new Label;
  _p->info_pixel->size_string(label_toolbar_info_pixel_size);
  _p->info_image =
    new Label;
  _p->info_cache =
    new Label;
    
  _p->view =
    new view::Widget;

  // Layout.
  
  Layout_V * layout = 0;
  Layout_H * layout_h = 0;
  
  layout = new Layout_V(this);
  layout->spacing(0);
  layout->margin(0);
  layout->add(_p->menu);
  
  layout->add(_p->toolbar0);
  Layout_H * toolbar0_layout = new Layout_H(_p->toolbar0);
  
  Layout_H * toolbar_file_layout = new Layout_H(toolbar0_layout);
  toolbar_file_layout->margin(0);
  
  Layout_H * toolbar_window_layout = new Layout_H(toolbar0_layout);
  toolbar_window_layout->margin(0);
  toolbar_window_layout->add(new Frame_Group_H(List<Frame *>() <<
    _p->window_new <<
    _p->window_copy <<
    _p->window_close <<
    _p->window_fit <<
    _p->window_fullscreen));
  
  Layout_H * toolbar_view_layout = new Layout_H(toolbar0_layout);
  toolbar_view_layout->margin(0);
  
  Layout_H * toolbar_image_layout = new Layout_H(toolbar0_layout);
  toolbar_image_layout->margin(0);
  
  Layout_H * toolbar_tool_layout = new Layout_H(toolbar0_layout);
  toolbar_tool_layout->margin(0);
  
  toolbar0_layout->add_stretch();
  
  layout->add(_p->view);
  layout->stretch(_p->view);
  
  layout->add(_p->toolbar1);
  Layout_V * toolbar1_layout = new Layout_V(_p->toolbar1);
  
  toolbar1_layout->add(_p->toolbar1_playback);
  Layout_H * toolbar1_playback_layout = new Layout_H(_p->toolbar1_playback);
  toolbar1_playback_layout->margin(0);
  
  toolbar1_layout->add(_p->toolbar1_info);
  Layout_H * toolbar1_info_layout = new Layout_H(_p->toolbar1_info);
  toolbar1_info_layout->spacing(style::global()->spacing_large());
  toolbar1_info_layout->margin(0);
  layout_h = new Layout_H(toolbar1_info_layout);
  layout_h->size(layout_item::FIXED, layout_item::FIXED);
  layout_h->margin(0);
  layout_h->add(_p->info_swatch);
  layout_h->add(_p->info_pixel);
  toolbar1_info_layout->add_spacer(-1, true);
  toolbar1_info_layout->add(_p->info_image);
  toolbar1_info_layout->add(_p->info_cache);
  toolbar1_info_layout->add_spacer(layout::window_handle_size());

  // Initialize.
  
  _file = std::auto_ptr<file::Group>(
    new file::Group(
      _p->menu,
      toolbar_file_layout,
      copy ? copy->_file.get() : 0
    )
  );
  
  _view = std::auto_ptr<view::Group>(
    new view::Group(
      _p->menu,
      toolbar_view_layout,
      copy ? copy->_view.get() : 0
    )
  );
  
  _image = std::auto_ptr<image::Group>(
    new image::Group(
      _p->menu,
      toolbar_image_layout,
      copy ? copy->_image.get() : 0
    )
  );
  
  _playback = std::auto_ptr<playback::Group>(
    new playback::Group(
      _p->menu,
      toolbar1_playback_layout,
      copy ? copy->_playback.get() : 0
    )
  );

  _tool = std::auto_ptr<tool::Group>(
    new tool::Group(
      _p->menu,
      toolbar_tool_layout,
      copy ? copy->_tool.get() : 0
    )
  );

  _help = std::auto_ptr<help::Group>(
    new help::Group(
      _p->menu,
      copy ? copy->_help.get() : 0
    )
  );

  if (copy)
  {
    _p->view->view(copy->_p->view->view());
    _p->view->zoom(copy->_p->view->zoom());
  }
  _p->view->hud(_view->hud());
  _p->view->grid(_view->grid());
  
  file_update();
  window_toolbar_update();
  view_update();
  view_overlay_update();
  image_update();
  image_pick_update();
  cache_update();
  group_update();
  
  if (copy)
  {
    size(copy->geom().size);
  }
  else
  {
    //if (prefs()->resize_fit())
      window_fit();
  }

  // Callbacks.
  
  _file->open_signal.set(this, file_open_callback);
  _file->reload_signal.set(this, file_reload_callback);
  _file->save_signal.set(this, file_save_callback);
  _file->save_frame_signal.set(this, file_save_frame_callback);
  _file->cache_signal.set(this, cache_callback);
  _file->image_signal.set(this, image_callback);
  _file->image_frame_store_signal.set(this, image_frame_store_callback);
  _file->update_signal.set(this, group_callback);
  
  _p->window_new->signal.set(this, window_new_callback);
  _p->window_copy->signal.set(this, window_copy_callback);
  _p->window_close->signal.set(this, window_close_callback);
  _p->window_fit->signal.set(this, window_fit_callback);
  _p->window_fullscreen->signal.set(this, window_fullscreen_callback);
  
  _view->move_signal.set(this, view_move_callback);
  _view->center_signal.set(this, view_center_callback);
  _view->zoom_signal.set(this, view_zoom_callback);
  _view->zoom_reset_signal.set(this, view_zoom_reset_callback);
  _view->reset_signal.set(this, view_reset_callback);
  _view->fit_signal.set(this, view_fit_callback);
  _view->overlay_signal.set(this, view_overlay_callback);
  _view->update_signal.set(this, group_callback);
  
  _p->view->signal.set(this, view_callback);
  _p->view->pick_signal.set(this, view_pick_callback);
  _p->view->mouse_wheel_signal.set(this, mouse_wheel_callback);
  _p->view->mouse_wheel_value_signal.set(this, mouse_wheel_value_callback);
  _p->view->file_signal.set(this, file_open_callback);
  
  _image->redraw_signal.set(this, image_redraw_callback);
  _image->resize_signal.set(this, image_resize_callback);
  _image->frame_store_signal.set(this, image_frame_store_callback);
  _image->display_profile_signal.set(this, image_display_profile_callback);
  _image->display_profile_value_signal.set(this,
    image_display_profile_value_callback);
  _image->update_signal.set(this, group_callback);

  _playback->image_signal.set(this, image_callback);
  _playback->overlay_signal.set(this, view_overlay_callback);
  _playback->update_signal.set(this, group_callback);

  _tool->magnify_signal.set(this, tool_magnify_callback);
  _tool->color_picker_signal.set(this, tool_color_picker_callback);
  _tool->histogram_signal.set(this, tool_histogram_callback);
  _tool->info_signal.set(this, tool_info_callback);
  
  prefs()->toolbar_signal.set(this, window_toolbar_callback);

  close_signal.set(this, window_close2_callback);
  
  _is_valid = true;
}

Window::~Window()
{
  DJV_DEBUG("Window::~Window");
}

void Window::del()
{
  DJV_DEBUG("Window::del");
  
  // Cleanup.
  
  djv_ui::window::Window::del();
  
  image_display_profile::dialog()->del(_p->view);
  
  tool_info::dialog()->del(_p->view);
  tool_histogram::dialog()->del(_p->view);
  tool_color_picker::dialog()->del(_p->view);
  tool_magnify::dialog()->del(_p->view);
  
  _p->view->set(0);
    
  _file->del();
  _view->del();
  _image->del();
  _playback->del();
  _tool->del();
  _help->del();
  
  _p->sample_buffer.reset();
}

int Window::handle(int in)
{
  switch (in)
  {
    case FL_FOCUS:
      image_focus_update();
      break;
    
    case FL_SHORTCUT:
      switch (Fl::event_key())
      {
        case FL_Escape:
        
          // Escape key is hard-wired for exiting fullscreen mode.
          
          if (fullscreen())
          {
            window_fullscreen(false);
            return 1;
          }
          
          break;
        
        default:
        
          // XXX Needed for shortcut handling when the menubar isn't visible?
      
          if (! _p->menu->visible())
            if (_p->menu->test_shortcut())
              return 1;
          
          break;
      }
      break;
  }

  return djv_ui::window::Window::handle(in);
}

void Window::dirty()
{
  DJV_DEBUG("Window::dirty");

  djv_ui::window::Window::dirty();
  
  if (_is_valid)
  {
    view_overlay_update();
    image_redraw_update();
    group_update();
  }
}

void Window::geom(const Box2i & in)
{
  djv_ui::window::Window::geom(in);
  
  view_resize_update();
  view_update();
}

const Box2i & Window::geom() const
{
  return djv_ui::window::Window::geom();
}

void Window::menu_update(djv_ui::menu::Group * in)
{
  using namespace djv_ui::menu;
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
    
  // Menu:
  //
  // * New
  // * Copy
  // * Close
  //   ---
  // * Fit
  // * Fullscreen
  //   ---
  // * Toolbar Buttons
  // * Toolbar Playback
  // * Toolbar Information
  
  in->add(menu_title, 0, 0, 0, SUB_MENU);
  
  in->add(
    menu_new,
    shortcut[shortcut::WINDOW_NEW].value,
    _window_new_callback,
    this
  );
  
  in->add(
    menu_copy,
    shortcut[shortcut::WINDOW_COPY].value,
    _window_copy_callback,
    this
  );
  
  in->add(
    menu_close,
    shortcut[shortcut::WINDOW_CLOSE].value,
    _window_close_callback,
    this,
    DIVIDER
  );

  in->add(
    menu_fit,
    shortcut[shortcut::WINDOW_FIT].value,
    _window_fit_callback,
    this
  );
  
  in->add(
    menu_fullscreen,
    shortcut[shortcut::WINDOW_FULLSCREEN].value,
    _window_fullscreen_callback,
    this,
    TOGGLE | DIVIDER,
    _window_fullscreen
  );
  
  in->add(
    menu_toolbar_buttons,
    shortcut[shortcut::WINDOW_TOOLBAR_BUTTONS].value,
    _window_toolbar_buttons_callback,
    this,
    TOGGLE,
    _window_toolbar[TOOLBAR_BUTTONS]
  );

  in->add(
    menu_toolbar_playback,
    shortcut[shortcut::WINDOW_TOOLBAR_PLAYBACK].value,
    _window_toolbar_playback_callback,
    this,
    TOGGLE,
    _window_toolbar[TOOLBAR_PLAYBACK]
  );

  in->add(
    menu_toolbar_info,
    shortcut[shortcut::WINDOW_TOOLBAR_INFO].value,
    _window_toolbar_info_callback,
    this,
    TOGGLE,
    _window_toolbar[TOOLBAR_INFO]
  );

  in->end();
}

void Window::toolbar_update()
{
  // Update tooltips.
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
  
  _p->window_new->tooltip(string::Format(tooltip_new).
    arg(djv_ui::shortcut::label(shortcut[shortcut::WINDOW_NEW].value)));
  
  _p->window_copy->tooltip(string::Format(tooltip_copy).
    arg(djv_ui::shortcut::label(shortcut[shortcut::WINDOW_COPY].value)));
  
  _p->window_close->tooltip(string::Format(tooltip_close).
    arg(djv_ui::shortcut::label(shortcut[shortcut::WINDOW_CLOSE].value)));
  
  _p->window_fit->tooltip(string::Format(tooltip_fit).
    arg(djv_ui::shortcut::label(shortcut[shortcut::WINDOW_FIT].value)));
  
  _p->window_fullscreen->tooltip(string::Format(tooltip_fullscreen).
    arg(djv_ui::shortcut::label(shortcut[shortcut::WINDOW_FULLSCREEN].value)));

  _p->info_pixel->tooltip(tooltip_info_pixel);
  _p->info_image->tooltip(tooltip_info_image);
  _p->info_cache->tooltip(tooltip_info_cache);
}

void Window::file_open(const File & in, bool init)
{
  DJV_DEBUG("Window::file_open");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("init = " << init);
  
  // Initialize.
  
  _image_p = 0;
  
  _p->view->set(0);
  
  // Open the file.

  callbacks(false);
  
  _file->open(in);
  
  // Set playback.
  
  const djv_image::Time & time = _file->info().time;
  
  _playback->time(time);
  
  if (init)
  {
    _playback->frame(0);
    
    _playback->playback(
      (time.list.size() > 1 && playback::prefs()->start()) ?
        playback::FORWARD :
        playback::STOP
    );
  }
  
  callbacks(true);
  
  // Update.
  
  file_update();
  image_update();
  
  if (init)
  {
    if (prefs()->resize_fit())
    {
      if (! _window_fullscreen)
      {
        _p->view->view_zero();
        
        window_fit();
      }
      else
      {
        _p->view->view_fit();
      }
    }
    else
    {
      view_resize_update();
    }
  }
  
  view_update();
  group_update();
}

void Window::file_open_callback(const File & in)
{
  file_open(in);
}

void Window::file_reload_callback(bool in)
{
  file_open(_file->file(), false);
}

void Window::file_save_callback(const File & in)
{
  // Save a sequence.
  
  djv_image::Time time;
  time.list = List<int64_t>(
    _playback->time().list,
    _playback->in(),
    _playback->out() - _playback->in() + 1
  );
  time.speed = _playback->speed();
  
  file::save()->set(
    file::Save_Info(
      _file->file(),
      in,
      _file->info()[_file->layer()],
      time,
      _file->layer(),
      _file->proxy(),
      image_option()
    )
  );
}

void Window::file_save_frame_callback(const File & in)
{
  DJV_DEBUG("Window::file_save_frame_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Save a single frame.

  djv_image::Time time;
  
  const List<int64_t> & list = _playback->time().list;
  if (list.size())
    time.list += list[_playback->frame()];
  
  time.speed = _playback->speed();
  
  DJV_DEBUG_PRINT("time = " << time);
  
  file::save()->set(
    file::Save_Info(
      _file->file(),
      in,
      _file->info()[_file->layer()],
      time,
      _file->layer(),
      _file->proxy(),
      image_option()
    )
  );
}

void Window::file_update()
{
  DJV_DEBUG("Window::file_update");

  callbacks(false);
  
  // Update window title.
  
  const File & file = _file->file();
  
  title(
    ! file.get().empty() ?
      string::Format("%% - %%").arg(application::global()->name()).arg(file) :
      string::Format("%%").arg(application::global()->name())
  );

  callbacks(true);
}

void Window::file_seq_auto(bool in)
{
  _file->seq_auto(in);
}

void Window::file_layer(int in)
{
  _file->layer(in);
}

void Window::file_proxy(djv_image::data::PROXY in)
{
  _file->proxy(in);
}

void Window::file_cache(bool in)
{
  _file->cache(in);
}

void Window::window_new_callback(bool)
{
  DJV_DEBUG("Window::window_new_callback");

  factory()->add()->show();
}

void Window::_window_new_callback()
{
  window_new_callback(true);
}

void Window::window_copy_callback(bool)
{
  DJV_DEBUG("Window::window_copy_callback");

  factory()->add(this)->show();
}

void Window::_window_copy_callback()
{
  window_copy_callback(true);
}

void Window::window_close_callback(bool)
{
  if (1 == factory()->list().size())
    return;
  
  DJV_DEBUG("Window::window_close_callback");

  window_close2_callback(true);
}

void Window::window_close2_callback(bool)
{
  factory()->del(this);
}

void Window::_window_close_callback()
{
  window_close_callback(true);
}

void Window::window_fit()
{
  DJV_DEBUG("Window::window_fit");
  
  // Calculate image size.
  
  const Box2f bbox = _p->view->bbox().size;
  
  DJV_DEBUG_PRINT("bbox = " << bbox);
    
  V2i image_size = vector::ceil<double, int>(bbox.size);
  if (! vector::is_size_valid(image_size))
    image_size = V2i(600, 300);
  
  DJV_DEBUG_PRINT("image size = " << image_size);

  // Adjust to screen size.
  
  const double resize_max = window::resize_max(
    prefs()->resize_fit() ?
      prefs()->resize_max() :
      RESIZE_MAX_UNLIMITED
  );
  
  const V2i max(
    static_cast<int>(Fl::w() * resize_max),
    static_cast<int>(Fl::h() * resize_max)
  );
  
  if (image_size.x > max.x || image_size.y > max.y)
    image_size =
      image_size.x > image_size.y ?
        V2i(
          max.x,
          math::ceil(image_size.y / static_cast<double>(image_size.x) * max.x)
        ) :
        V2i(
          math::ceil(image_size.x / static_cast<double>(image_size.y) * max.y),
          max.y
        );

  // Adjust to size hint.

  const V2i ui_size = size_hint() - V2i(0, _p->view->size_hint().y);
  
  DJV_DEBUG_PRINT("ui size = " << ui_size);
  
  const V2i size = V2i(
    math::max(image_size.x, ui_size.x),
    image_size.y + ui_size.y
  );
  
  DJV_DEBUG_PRINT("size = " << size);
  
  // Set size.
  
  fullscreen(false);
  _window_fullscreen = false;
  
  this->size(size);
  
  _p->view->view_fit();
  _p->view->redraw();
  
  view_update();
}

void Window::window_fit_callback(bool)
{
  window_fit();
}

void Window::_window_fit_callback()
{
  window_fit_callback(true);
}

void Window::window_fullscreen(bool in)
{
  if (in == _window_fullscreen)
    return;
  
  DJV_DEBUG("Window::window_fullscreen");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Set value.
  
  _window_fullscreen = in;
  fullscreen(_window_fullscreen);
  
  // Update.
  
  window_fullscreen_update();
  group_update();
  update();
  if (prefs()->resize_fit())
    _p->view->view_fit();
  else
    view_resize_update();
  view_update();
}

void Window::window_fullscreen_callback(bool in)
{
  window_fullscreen(in);
}

void Window::_window_fullscreen_callback()
{
  window_fullscreen(_p->menu->value());
}

void Window::window_fullscreen_update()
{
  DJV_DEBUG("Window::window_fullscreen_update");

  callbacks(false);
  
  _p->window_fullscreen->set(_window_fullscreen);
  
  if (_window_fullscreen)
  {
    // Hide widgets.
    
    _p->menu->hide();

    _p->toolbar0->hide();
    _p->toolbar1->hide();
  }
  else
  {
    // Show widgets.
    
    _p->menu->show();
    
    if (_window_toolbar[TOOLBAR_BUTTONS])
      _p->toolbar0->show();
    if (
      _window_toolbar[TOOLBAR_PLAYBACK] ||
      _window_toolbar[TOOLBAR_INFO]
    )
      _p->toolbar1->show();
  }
  
  callbacks(true);
}

void Window::window_toolbar(const List<bool> & in)
{
  if (in == _window_toolbar)
    return;
  
  // Set value.
  
  _window_toolbar = in;
  
  // Update.
  
  window_toolbar_update();
  //window_resize_update();
  group_update();
  update();
  window_resize_update(); // XXX Needs update() first.
}
  
const List<bool> & Window::window_toolbar() const
{
  return _window_toolbar;
}

void Window::window_toolbar_callback(const List<bool> & in)
{
  window_toolbar(in);
}

void Window::window_toolbar_buttons_callback(bool in)
{
  List<bool> toolbar = _window_toolbar;
  toolbar[TOOLBAR_BUTTONS] = in;
  window_toolbar(toolbar);
}

void Window::_window_toolbar_buttons_callback()
{
  window_toolbar_buttons_callback(_p->menu->value());
}

void Window::window_toolbar_playback_callback(bool in)
{
  List<bool> toolbar = _window_toolbar;
  toolbar[TOOLBAR_PLAYBACK] = in;
  window_toolbar(toolbar);
}

void Window::_window_toolbar_playback_callback()
{
  window_toolbar_playback_callback(_p->menu->value());
}

void Window::window_toolbar_info_callback(bool in)
{
  List<bool> toolbar = _window_toolbar;
  toolbar[TOOLBAR_INFO] = in;
  window_toolbar(toolbar);
}

void Window::_window_toolbar_info_callback()
{
  window_toolbar_info_callback(_p->menu->value());
}

void Window::window_toolbar_update()
{
  DJV_DEBUG("Window::window_toolbar_update");

  callbacks(false);
  
  // Set toolbar0 visibility.
  
  if (_window_toolbar[TOOLBAR_BUTTONS] && ! _window_fullscreen)
  {
    _p->toolbar0->show();
  }
  else
  {
    _p->toolbar0->hide();
  }
  
  // Set toolbar1 visibility.

  bool playback = _window_toolbar[TOOLBAR_PLAYBACK];
  bool info = _window_toolbar[TOOLBAR_INFO];
  
  DJV_DEBUG_PRINT("playback = " << playback);
  DJV_DEBUG_PRINT("info = " << info);
  
  if (playback && ! _window_fullscreen)
  {
    _p->toolbar1_playback->show();
  }
  else
  {
    _p->toolbar1_playback->hide();
  }
  
  if (info && ! _window_fullscreen)
  {
    _p->toolbar1_info->show();
  }
  else
  {
    _p->toolbar1_info->hide();
  }
  
  if ((playback || info) && ! _window_fullscreen)
  {
    DJV_DEBUG_PRINT("toolbar1 show");
    
    _p->toolbar1->show();
  }
  else
  {
    DJV_DEBUG_PRINT("toolbar1 hide");

    _p->toolbar1->hide();
  }
  
  _playback->toolbar_info_update(_window_toolbar[TOOLBAR_INFO]);
  
  callbacks(true);
}

void Window::window_resize_update()
{
  DJV_DEBUG("Window::window_resize_update");
  DJV_DEBUG_PRINT("geom = " << geom());
  
  callbacks(false);
  
  if (prefs()->resize_fit())
  {
    if (! _window_fullscreen)
    {
      window_fit();
    }
    else
    {
      _p->view->view_fit();
    }
  }
  
  callbacks(true);
}

void Window::view_grid(view::GRID in)
{
  _view->grid(in);
}

void Window::view_hud(bool in)
{
  _view->hud(in);
}

void Window::view_callback(bool)
{
  DJV_DEBUG("Window::view_callback");
  
  view_update();
}

void Window::view_move_callback(const V2i & in)
{
  _p->view->view(_p->view->view() + in);
  _p->view->redraw();
}

void Window::view_center_callback(bool)
{
  _p->view->view_center();
  _p->view->redraw();
}

void Window::view_zoom_callback(double in)
{
  _p->view->zoom_focus(_p->view->zoom() * in);
  _p->view->redraw();
}

void Window::view_zoom_reset_callback(bool)
{
  _p->view->zoom_focus(1.0);
  _p->view->redraw();
}

void Window::view_reset_callback(bool)
{
  _p->view->view_zero();
  _p->view->redraw();
}

void Window::view_fit_callback(bool)
{
  _p->view->view_fit();
  _p->view->redraw();
}

void Window::view_update()
{
  DJV_DEBUG("Window::view_update");

  callbacks(false);

  _view->zoom(_p->view->zoom());

  callbacks(true);
}

void Window::view_resize_update()
{
  DJV_DEBUG("Window::view_resize_update");

  callbacks(false);
  
  switch (view::prefs()->resize())
  {
    case view::RESIZE_NONE: break;
    case view::RESIZE_FIT: _p->view->view_fit(); break;
    case view::RESIZE_CENTER: _p->view->view_center(); break;
  }

  callbacks(true);
}

void Window::view_overlay_callback(bool)
{
  view_overlay_update();
}

void Window::view_overlay_update()
{
  DJV_DEBUG("Window::view_overlay_update");
  
  callbacks(false);

  // Grid overlay.
  
  _p->view->grid(_view->grid());
  _p->view->grid_color(view::prefs()->grid_color());

  // HUD overlay.
  
  view::Hud_Info hud;
  
  if (_image_p)
  {
    hud.info = _image_p->info();
    hud.tag = _image_p->tag;
  }
  
  const djv_image::Time & time = _playback->time();

  hud.frame = 0;
  if (
    time.list.size() &&
    _playback->frame() < static_cast<int64_t>(time.list.size())
  )
    hud.frame = time.list[_playback->frame()];

  hud.speed = _playback->speed();
  hud.speed_real = _playback->speed_real();
  hud.dropped_frames = _playback->dropped_frames();
  _p->view->hud(_view->hud());
  _p->view->hud_info(hud);
  _p->view->hud_show(view::prefs()->hud_show());
  _p->view->hud_color(view::prefs()->hud_color());
  _p->view->hud_background(view::prefs()->hud_background());
  _p->view->hud_background_color(view::prefs()->hud_background_color());
  
  // Update.
  
  _p->view->redraw();

  callbacks(true);
}

void Window::view_pick_callback(const V2i & in)
{
  DJV_DEBUG("Window::view_pick_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  _image_pick = in - _p->view->view();
  
  image_pick_update();
}
  
djv_image::gl_data::Option Window::image_option() const
{
  // Image processing options.
  
  djv_image::gl_data::Option out;

  out.xform.mirror = _image->mirror();
  if (_image_p)
    out.xform.scale = image::scale(_image->scale(), _image_p->size());
  out.xform.rotate = image::rotate(_image->rotate());

  if (_image->color_profile() && _image_p)
    out.color_profile = _image_p->color_profile;
  out.display_profile = _image->display_profile();
  out.channel = _image->channel();
  out.background = view::prefs()->background();
  
  return out;
}

V2i Window::image_pick() const
{
  return vector::floor<double, int>(V2f(_image_pick) / _p->view->zoom());
}

void Window::image_callback(bool)
{
  image_update();
}

void Window::image_update()
{
  DJV_DEBUG("Window::image_update");
  
  // Update image.
  
  _image_p = _file->get(_playback->frame());
  
  if (_image_p)
  {
    DJV_DEBUG_PRINT("image = " << *_image_p);
  }

  // XXX
  
  callbacks(false);
  
  // Update infobar.

  djv_image::Info info;
  if (_image_p)
    info = _image_p->info();

  _p->info_image->set(string::Format(label_toolbar_info_image).
    arg(info.size.x).
    arg(info.size.y).
    arg(vector::aspect(info.size), 0, -2).
    arg(string::label(info.pixel)));

  callbacks(true);
  
  // Update.
  
  view_overlay_update();
  image_redraw_update();
  image_dialog_update();
  tool_dialog_update();
}

void Window::image_redraw_callback(bool)
{
  image_redraw_update();
}

void Window::image_redraw_update()
{
  DJV_DEBUG("Window::image_redraw_update");
  DJV_DEBUG_PRINT("view = " << _p->view->view());
  DJV_DEBUG_PRINT("zoom = " << _p->view->zoom());
  
  callbacks(false);
  
  const djv_image::Data * prev = _p->view->get();
  const djv_image::gl_data::Option prev_option = _p->view->option();
  
  // Set image.
  
  _p->view->set(_image->frame_store() ? &_image_tmp : _image_p);

  // Set image options.
  
  const djv_image::gl_data::Option & option = image_option();
  
  DJV_DEBUG_PRINT("color = " << option.display_profile.color);
  
  _p->view->option(option);
  _p->view->redraw();

  callbacks(true);
}

void Window::image_resize_callback(bool)
{
  window_resize_update();
}

void Window::image_frame_store_callback(bool)
{
  DJV_DEBUG("Window::image_frame_store_callback");
  
  if (_image_p)
    _image_tmp = *_image_p;
}

void Window::image_display_profile_callback(bool)
{
  // Connect image display profile dialog.
  
  image_display_profile::dialog()->show();
  image_display_profile::dialog()->set(_p->view, _image->display_profile());
  image_display_profile::dialog()->signal.del();
  image_display_profile::dialog()->signal.set(
    this, image_display_profile_value_callback
  );
}

void Window::image_display_profile_value_callback(
  const image::Display_Profile & in
) {
  DJV_DEBUG("Window::image_display_profile_value_callback");
  
  // Set image display profile.
  
  _image->display_profile(in);

  // Update.
  
  image_redraw_update();
  image_dialog_update();
  tool_dialog_update();
}

void Window::image_dialog_update()
{
  DJV_DEBUG("Window::image_dialog_update");
  
  callbacks(false);

  image_display_profile::dialog()->update(_p->view, _image->display_profile());

  callbacks(true);
}

void Window::image_focus_update()
{
  DJV_DEBUG("Window::image_focus_update");
  
  callbacks(false);

  image_display_profile::dialog()->pick(_p->view, _image->display_profile());
  image_display_profile::dialog()->signal.del();
  image_display_profile::dialog()->signal.set(
    this, image_display_profile_value_callback
  );
  
  tool_histogram::dialog()->pick(_p->view);
  tool_info::dialog()->pick(_p->view, _file->info());

  callbacks(true);
}

void Window::image_pick_update()
{
  DJV_DEBUG("Window::image_pick_update");
  
  callbacks(false);
  
  // Update infobar with pixel information.
  
  const V2i pick = image_pick();
  djv_image::gl_data::Option option = image_option();
  
  _image_sample = option.background;
  
  if (_image_p && _window_toolbar[TOOLBAR_INFO])
  {
    DJV_DEBUG_PRINT("sample");

    try
    {
      djv_image::Data tmp(djv_image::Info(1, _image_p->pixel()));
    
      if (! _p->sample_buffer.get())
        _p->sample_buffer = std::auto_ptr<djv_image::gl_offscreen::Buffer>(
          djv_image::gl_offscreen::global()->buffer_create()
        );
    
      _p->sample_buffer->init(tmp.info());

      //if (! _p->sample_data.get())
      //  _p->sample_data = std::auto_ptr<djv_image::gl_data::Data>(
      //    new djv_image::gl_data::Data
      //  );
    
      djv_image::gl_data::Option _option = option;
      _option.xform.position -= pick;
    
      djv_image::gl_data::copy(
        *_image_p,
        &tmp,
        _option,
        //_p->sample_data.get(),
        &_p->view->gl_data(),
        _p->sample_buffer.get(),
        false
      );
    
      djv_image::gl_data::average(tmp, &_image_sample);
    }
    catch (Error in)
    {
      application::global()->error(in);
    }
  }
  
  DJV_DEBUG_PRINT("sample = " << _image_sample);
  
  _p->info_swatch->set(_image_sample);
  
  _p->info_pixel->set(string::Format(label_toolbar_info_pixel).
    arg(pick.x).
    arg(pick.y).
    arg(string::label(_image_sample)));
  
  // Update dialogs.
  
  tool_magnify::dialog()->pick(_p->view, _image_pick);
  tool_color_picker::dialog()->pick(_p->view, _image_pick);
  
  callbacks(true);
}

void Window::playback(playback::PLAYBACK in)
{
  _playback->playback(in);
}

void Window::playback_frame(int64_t in)
{
  _playback->frame(in);
}

void Window::playback_speed(const djv_image::time::Speed & in)
{
  _playback->speed(in);
}

void Window::tool_magnify_callback(bool)
{
  tool_magnify::dialog()->show();
  tool_magnify::dialog()->set(_p->view, _image_pick);
}

void Window::tool_color_picker_callback(bool)
{
  tool_color_picker::dialog()->show();
  tool_color_picker::dialog()->set(_p->view, _image_pick);
}

void Window::tool_histogram_callback(bool)
{
  tool_histogram::dialog()->show();
  tool_histogram::dialog()->set(_p->view);
}

void Window::tool_info_callback(bool)
{
  tool_info::dialog()->show();
  tool_info::dialog()->set(_p->view, _file->info());
}

void Window::tool_dialog_update()
{
  DJV_DEBUG("Window::tool_dialog_update");
  
  callbacks(false);

  tool_magnify::dialog()->update(_p->view);
  
  tool_color_picker::dialog()->update(_p->view);

  tool_histogram::dialog()->update(_p->view);
  
  tool_info::dialog()->update(_p->view, _file->info());
  
  callbacks(true);
}

void Window::mouse_wheel_callback(input::MOUSE_WHEEL in)
{
  _mouse_wheel = in;
  
  switch (_mouse_wheel)
  {
    case input::MOUSE_WHEEL_PLAYBACK_SHUTTLE:
      _playback->playback(playback::STOP);
      _playback_frame_tmp = _playback->frame();
      break;
    
    case input::MOUSE_WHEEL_PLAYBACK_SPEED:
      _playback_speed_tmp =
        djv_image::time::speed_to_float(_playback->speed());
      break;
    
    default: break;
  }
}

void Window::mouse_wheel_value_callback(int in)
{
  switch (_mouse_wheel)
  {
    case input::MOUSE_WHEEL_PLAYBACK_SHUTTLE:
      _playback->frame(_playback_frame_tmp + in);
      break;
    
    case input::MOUSE_WHEEL_PLAYBACK_SPEED:
      _playback->speed(
        djv_image::time::float_to_speed(
          _playback_speed_tmp + static_cast<double>(in)
        )
      );
      break;
    
    default: break;
  }
}

void Window::cache_callback(const List<int64_t> & in)
{
  DJV_DEBUG("Window::cache_callback");
  
  _playback->cached_frames(
    file::prefs()->cache_display() ? in : List<int64_t>()
  );
  
  cache_update();
}

void Window::cache_update()
{
  DJV_DEBUG("Window::cache_update");

  callbacks(false);
  
  _p->info_cache->set(string::Format(label_toolbar_info_cache).
    arg(cache::global()->size(_file.get())).
    arg(cache::global()->size()).
    arg(cache::global()->max()));
  
  callbacks(true);
}

void Window::group_callback(bool)
{
  group_update();
}

void Window::group_update()
{
  DJV_DEBUG("Window::group_update");

  callbacks(false);
  
  // Update toolbar.
    
  _file->toolbar_update();
  toolbar_update();
  _view->toolbar_update();
  _image->toolbar_update();
  _tool->toolbar_update();
  
  // Update playabar.
  
  _playback->toolbar_update();
  
  // Update menus.
  
  djv_ui::menu::Group group;
  _file->menu_update(&group, _playback->time().list.size() > 1);
  menu_update(&group);
  _view->menu_update(&group);
  _image->menu_update(&group);
  _playback->menu_update(&group);
  _tool->menu_update(&group);
  _help->menu_update(&group);
  _p->menu->set(group);
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}
}

