/*------------------------------------------------------------------------------
 bin/djv_view/application.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include "cache.h"
#include "file_prefs.h"
#include "file_save.h"
#include "image_display_profile.h"
#include "image_prefs.h"
#include "input_prefs.h"
#include "playback_prefs.h"
#include "shortcut.h"
#include "tool_color_picker.h"
#include "tool_histogram.h"
#include "tool_info.h"
#include "tool_magnify.h"
#include "view_prefs.h"
#include "window.h"
#include "window_prefs.h"
#include <djv_ui/prefs_dialog.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
// global()
//------------------------------------------------------------------------------

namespace {

struct _Global
{
  void init()
  {
    input_prefs = std::auto_ptr<input::Prefs>(new input::Prefs);

    shortcut_prefs = std::auto_ptr<shortcut::Prefs>(new shortcut::Prefs);

    cache = std::auto_ptr<cache::Cache>(new cache::Cache);

    file_prefs = std::auto_ptr<file::Prefs>(new file::Prefs);
    file_save = std::auto_ptr<file::Save>(new file::Save);

    window_prefs = std::auto_ptr<window::Prefs>(new window::Prefs);

    view_prefs = std::auto_ptr<view::Prefs>(new view::Prefs);

    image_prefs = std::auto_ptr<image::Prefs>(new image::Prefs);

    playback_prefs = std::auto_ptr<playback::Prefs>(new playback::Prefs);
    playback_global = std::auto_ptr<playback::Global>(new playback::Global);
  }
  
  std::auto_ptr<input::Prefs> input_prefs;

  std::auto_ptr<shortcut::Prefs> shortcut_prefs;

  std::auto_ptr<cache::Cache> cache;

  std::auto_ptr<file::Prefs> file_prefs;
  std::auto_ptr<file::Save> file_save;

  std::auto_ptr<window::Prefs> window_prefs;

  std::auto_ptr<view::Prefs> view_prefs;

  std::auto_ptr<image::Prefs> image_prefs;

  std::auto_ptr<playback::Prefs> playback_prefs;
  std::auto_ptr<playback::Global> playback_global;
};

std::auto_ptr<_Global> _global;

struct _Global_Ui
{
  void init()
  {
    image_display_profile.set(new image_display_profile::Dialog);
    
    tool_magnify.set(new tool_magnify::Dialog);
    tool_color_picker.set(new tool_color_picker::Dialog);
    tool_histogram.set(new tool_histogram::Dialog);
    tool_info.set(new tool_info::Dialog);
    
    window_factory = std::auto_ptr<window::Factory>(new window::Factory);
  }
  
  djv_ui::widget::Auto_Ptr<image_display_profile::Dialog>
    image_display_profile;
  
  djv_ui::widget::Auto_Ptr<tool_magnify::Dialog> tool_magnify;
  djv_ui::widget::Auto_Ptr<tool_color_picker::Dialog> tool_color_picker;
  djv_ui::widget::Auto_Ptr<tool_histogram::Dialog> tool_histogram;
  djv_ui::widget::Auto_Ptr<tool_info::Dialog> tool_info;
  
  std::auto_ptr<window::Factory> window_factory;
};

std::auto_ptr<_Global_Ui> _global_ui;

}

namespace input {

Prefs * prefs() { return _global->input_prefs.get(); }

}

namespace shortcut {

Prefs * prefs() { return _global->shortcut_prefs.get(); }

}

namespace cache {

Cache * global() { return _global->cache.get(); }

}

namespace file {

Prefs * prefs() { return _global->file_prefs.get(); }
Save * save() { return _global->file_save.get(); }

}

namespace window {

Prefs * prefs() { return _global->window_prefs.get(); }

}

namespace view {

Prefs * prefs() { return _global->view_prefs.get(); }

}

namespace image {

Prefs * prefs() { return _global->image_prefs.get(); }

}

namespace playback {

Prefs * prefs() { return _global->playback_prefs.get(); }
Global * global() { return _global->playback_global.get(); }

}

namespace image_display_profile {

Dialog * dialog() { return _global_ui->image_display_profile.get(); }

}

namespace tool_magnify {

Dialog * dialog() { return _global_ui->tool_magnify.get(); }

}

namespace tool_color_picker {

Dialog * dialog() { return _global_ui->tool_color_picker.get(); }

}

namespace tool_histogram {

Dialog * dialog() { return _global_ui->tool_histogram.get(); }

}

namespace tool_info {

Dialog * dialog() { return _global_ui->tool_info.get(); }

}

namespace window {

Factory * factory() { return _global_ui->window_factory.get(); }

}

namespace application {

Application * global()
{
  return static_cast<Application *>(djv_ui::application::global());
}

//------------------------------------------------------------------------------
// Application
//------------------------------------------------------------------------------

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("djv_view", argc, argv)
{
  using namespace djv_base;
  
  DJV_DEBUG("Application::Application");
  
  _global = std::auto_ptr<_Global>(new _Global);
  _global->init();
  
  // Command line.
  
  try
  {
    String tmp = command_line_args();
    command_line(tmp);
  }
  catch (Error in)
  {
    error(Error(string::Format(djv_base::application::error_command_line).
      arg(in)));
    djv_image::application::Application_Base::exit(
      djv_base::application::EXIT_ERROR);
  }
  
  if (
    djv_image::application::Application_Base::exit() !=
    djv_base::application::EXIT_DEFAULT
  )
    return;

  // Initialize user-interface.
  
  is_valid(true);
  
  _global_ui = std::auto_ptr<_Global_Ui>(new _Global_Ui);
  _global_ui->init();
  
  // Initialize preferences dialog.
  
  djv_ui::prefs_dialog::global()->add_shortcut(shortcut::prefs_title,
    new shortcut::Prefs_Widget);
  djv_ui::prefs_dialog::global()->add(input::prefs_title,
    new input::Prefs_Widget);
  djv_ui::prefs_dialog::global()->add(playback::prefs_title,
    new playback::Prefs_Widget);
  djv_ui::prefs_dialog::global()->add(image::prefs_title,
    new image::Prefs_Widget);
  djv_ui::prefs_dialog::global()->add(view::prefs_title,
    new view::Prefs_Widget);
  djv_ui::prefs_dialog::global()->add(window::prefs_title,
    new window::Prefs_Widget);
  djv_ui::prefs_dialog::global()->add(file::prefs_title,
    new file::Prefs_Widget);

  // Show main window(s).
  
  if (_input.size())
  {
    // Combine command line arguments.
    
    if (file::prefs()->command_line_combine())
    {
      djv_base::File file(_input[0]);
      
      if (file.is_seq_valid())
        file.type(djv_base::file::SEQ);
      
      for (list::size_type i = 1; i < _input.size(); ++i)
        file.seq_add(_input[i]);

      djv_base::file::seq_sort(file);

      _input.resize(1);
      _input[0] = file;
    }
    
    // Create and show a window for each input.
    
    for (list::size_type i = 0; i < _input.size(); ++i)
    {
      djv_base::File file(_input[i]);
      
      if (file.is_seq_valid())
        file.type(djv_base::file::SEQ);

      window::Window * window = this->window();
      
      window->file_open(file);
      
      // XXX
      
      if (_command_line.file_layer.get())
        window->file_layer(*_command_line.file_layer);
      if (_command_line.playback.get())
        window->playback(*_command_line.playback);
      if (_command_line.playback_frame.get())
        window->playback_frame(*_command_line.playback_frame);
      if (_command_line.playback_speed.get())
        window->playback_speed(*_command_line.playback_speed);
      
      window->show();
    }
  }
  else
  {
    // Create and show an empty window.
    
    window()->show();
  }
}

Application::~Application()
{
  DJV_DEBUG("Application::~Application");
  
  _global_ui.reset();
  _global.reset();
}

window::Window * Application::window() const
{
  window::Window * out = window::factory()->add();
  
  // Apply command line file options.
  
  if (_command_line.file_seq_auto.get())
    out->file_seq_auto(*_command_line.file_seq_auto);

  if (_command_line.file_layer.get())
    out->file_layer(*_command_line.file_layer);

  if (_command_line.file_proxy.get())
    out->file_proxy(*_command_line.file_proxy);

  if (_command_line.file_cache.get())
    out->file_cache(*_command_line.file_cache);
  
  // Apply command line window options.
  
  if (_command_line.window_fullscreen.get())
    out->window_fullscreen(*_command_line.window_fullscreen);

  // Apply command line playback options.
  
  if (_command_line.playback.get())
    out->playback(*_command_line.playback);
  
  if (_command_line.playback_frame.get())
    out->playback_frame(*_command_line.playback_frame);
  
  if (_command_line.playback_speed.get())
    out->playback_speed(*_command_line.playback_speed);
  
  return out;
}

//------------------------------------------------------------------------------

}
}
}

