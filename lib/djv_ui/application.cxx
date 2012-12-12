/*------------------------------------------------------------------------------
 lib/djv_ui/application.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include <djv_ui/application_dialog.h>
#include <djv_ui/dialog.h>
#include <djv_ui/global.h>
#include <djv_ui/icon.h>
#include <djv_ui/image_io.h>
#include <djv_ui/image_prefs.h>
#include <djv_ui/file_browser.h>
#include <djv_ui/file_browser_prefs.h>
#include <djv_ui/prefs.h>
#include <djv_ui/prefs_dialog.h>
#include <djv_ui/style.h>
#include <djv_base/system.h>
#include <djv_ui/window.h>
#include <FL/Fl.H>
#include <algorithm>

namespace djv_ui {

//------------------------------------------------------------------------------
// global()
//------------------------------------------------------------------------------

namespace {

std::auto_ptr<Global> _global;

std::auto_ptr<prefs::Prefs> _prefs;
std::auto_ptr<prefs::Prefs> _prefs_global;
std::auto_ptr<base::Prefs> _base_prefs;
std::auto_ptr<style::Prefs> _style_prefs;
std::auto_ptr<image::Prefs> _image_prefs;
std::auto_ptr<file_browser::Prefs> _file_browser_prefs;

std::auto_ptr<base::Global> _base;
std::auto_ptr<style::Style> _style;
std::auto_ptr<icon::Factory> _icon;
std::auto_ptr<image_io::Global> _image_io;
std::auto_ptr<window::Global> _window;
std::auto_ptr<dialog::Global> _dialog;
widget::Auto_Ptr<prefs_dialog::Dialog> _prefs_dialog;
widget::Auto_Ptr<file_browser::Dialog> _file_browser;
std::auto_ptr<application_dialog::Global> _application_dialog;

}

Global * global() { return _global.get(); }

namespace prefs {
Prefs * prefs() { return _prefs.get(); }
Prefs * prefs_global() { return _prefs_global.get(); }
}

namespace base {
Prefs * prefs() { return _base_prefs.get(); }
}

namespace style {
Prefs * prefs() { return _style_prefs.get(); }
}

namespace image {
Prefs * prefs() { return _image_prefs.get(); }
}

namespace file_browser {
Prefs * prefs() { return _file_browser_prefs.get(); }
}

namespace base {
Global * global() { return _base.get(); }
}

namespace style {
Style * global() { return _style.get(); }
}

namespace icon {
Factory * global() { return _icon.get(); }
}

namespace image_io {
Global * global() { return _image_io.get(); }
}

namespace dialog {
Global * global()
{
  if (! _dialog.get())
    _dialog = std::auto_ptr<dialog::Global>(new dialog::Global);
  return _dialog.get();
}
}

namespace prefs_dialog {
Dialog * global()
{
  if (! _prefs_dialog.get())
    _prefs_dialog.set(new prefs_dialog::Dialog);
  return _prefs_dialog.get();
}
}

namespace file_browser {
Dialog * global()
{
  if (! _file_browser.get())
    _file_browser.set(new file_browser::Dialog);
  return _file_browser.get();
}
}

namespace application_dialog {
Global * global()
{
  if (! _application_dialog.get())
    _application_dialog = std::auto_ptr<application_dialog::Global>(
      new application_dialog::Global);
  return _application_dialog.get();
}
}

//------------------------------------------------------------------------------
// Application_Base
//------------------------------------------------------------------------------

namespace application {

Application_Base::Application_Base(
  const String & name,
  int & argc,
  char ** argv
)
  throw (Error) :
  djv_image::application::Application_Base(name, argc, argv),
  _is_valid(false)
{
  DJV_DEBUG("Application_Base::Application_Base");
  
  _global = std::auto_ptr<Global>(new Global);
  _prefs = std::auto_ptr<prefs::Prefs>(new prefs::Prefs(
    application::Application_Base::name()));
  _prefs_global = std::auto_ptr<prefs::Prefs>(new prefs::Prefs("global"));
  _base_prefs = std::auto_ptr<base::Prefs>(new base::Prefs);
  _style_prefs = std::auto_ptr<style::Prefs>(new style::Prefs);
  _image_prefs = std::auto_ptr<image::Prefs>(new image::Prefs);
  _file_browser_prefs =
    std::auto_ptr<file_browser::Prefs>(new file_browser::Prefs);
  _base = std::auto_ptr<base::Global>(new base::Global);
  _style = std::auto_ptr<style::Style>(new style::Style);
  _icon = std::auto_ptr<icon::Factory>(new icon::Factory);
  _image_io = std::auto_ptr<image_io::Global>(new image_io::Global);
  _window = std::auto_ptr<window::Global>(new window::Global);
  //_dialog = std::auto_ptr<dialog::Global>(new dialog::Global);
  //_prefs_dialog.set(new prefs_dialog::Dialog);
  //_file_browser.set(new file_browser::Dialog);
  //_application_dialog = std::auto_ptr<application_dialog::Global>(
  //  new application_dialog::Global);
}

Application_Base::~Application_Base()
{
  DJV_DEBUG("Application_Base::~Application_Base");
  
  _application_dialog.reset();
  _file_browser.del();
  _prefs_dialog.del();
  _dialog.reset();
  _window.reset();
  _image_io.reset();
  _icon.reset();
  _style.reset();
  _base.reset();
  _file_browser_prefs.reset();
  _image_prefs.reset();
  _style_prefs.reset();
  _base_prefs.reset();
  _prefs_global.reset();
  _prefs.reset();
  _global.reset();
}

int Application_Base::exec()
{
  DJV_DEBUG("Application_Base::exec");
  DJV_DEBUG_PRINT("valid = " << _is_valid);
  
  if (
    djv_image::application::Application_Base::exit() !=
    djv_base::application::EXIT_DEFAULT
  )
    exit();
  
  Fl::run();
  
  return djv_image::application::Application_Base::exec();
}

void Application_Base::is_valid(bool in)
{
  DJV_DEBUG("Application_Base::is_valid");
  DJV_DEBUG_PRINT("in = " << in);
  
  _is_valid = in;
}

bool Application_Base::is_valid() const
{
  return _is_valid;
}

void Application_Base::exit(djv_base::application::EXIT in)
{
  DJV_DEBUG("Application_Base::exit");
  
  djv_ui::global()->window_hide();
  
  if (in != djv_base::application::EXIT_DEFAULT)
    djv_image::application::Application_Base::exit(in);
}

String Application_Base::info() const
{
  return string::Format(label_info).
    arg(djv_image::application::Application_Base::info()).
    arg(image_io::global()->load()->name_list(), ", ").
    arg(image_io::global()->save()->name_list(), ", ").
    arg(prefs::prefs()->file_name()).
    arg(prefs::prefs_global()->file_name());
}

void Application_Base::message(const String & in) const
{
  if (_is_valid)
    application_dialog::message(in);
  else
    djv_image::application::Application_Base::message(in);
}

void Application_Base::error(const Error & in) const
{
  if (_is_valid)
    application_dialog::error(in);
  else
    djv_image::application::Application_Base::error(in);
}

void Application_Base::help() const
{
  DJV_DEBUG("Application_Base::help");
  
  const File url(file::path_fix(path() + "../doc") + "index.html");
  
  DJV_DEBUG_PRINT("url = " << url);

  djv_base::system::open_url(url);
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String label_info =
"%%"
"\n"
" Image I/O user-interface plugins:\n"
"\n"
" * Load: %%\n"
" * Save: %%\n"
"\n"
" Preferences:\n"
"\n"
" * Application: %%\n"
" * Global: %%\n";

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Application_Base * global()
{
  return static_cast<Application_Base *>(djv_image::application::global());
}

//------------------------------------------------------------------------------

}
}

