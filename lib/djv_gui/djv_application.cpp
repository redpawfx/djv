//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_application.cpp

#include <djv_application.h>

#include <djv_application_about_dialog.h>
#include <djv_application_info_dialog.h>
#include <djv_application_message_dialog.h>
#include <djv_icon.h>
#include <djv_image_prefs.h>
#include <djv_file_browser.h>
#include <djv_file_browser_prefs.h>
#include <djv_image_io_widget.h>
#include <djv_prefs.h>
#include <djv_prefs_dialog.h>
#include <djv_style.h>
#include <djv_window.h>

#include <djv_file.h>
#include <djv_system.h>

#include <FL/Fl.H>

#include <algorithm>

namespace djv
{

namespace
{

Prefs *              _prefs_             = 0;
Prefs *              _prefs_global       = 0;
Style_Prefs *        _style_prefs        = 0;
Image_Prefs *        _image_prefs        = 0;
File_Browser_Prefs * _file_browser_prefs = 0;

} // namespace

Prefs * Prefs::prefs()
{
    return _prefs_;
}

Prefs * Prefs::prefs_global()
{
    return _prefs_global;
}

Style_Prefs * Style_Prefs::global()
{
    return _style_prefs;
}

Image_Prefs * Image_Prefs::global()
{
    return _image_prefs;
}

File_Browser_Prefs * File_Browser_Prefs::global()
{
    return _file_browser_prefs;
}

//------------------------------------------------------------------------------
// Application
//------------------------------------------------------------------------------

Application::Application(
    const String & name,
    int &          argc,
    char **        argv) throw (Error) :
    Core_Application(name, argc, argv),
    window_add_signal  (this),
    window_del_signal  (this),
    window_focus_signal(this),
    _is_valid          (false)
{
    //DJV_DEBUG("Application::Application");

    _prefs_             = new Prefs(name);
    _prefs_global       = new Prefs("global");
    _style_prefs        = new Style_Prefs;
    _image_prefs        = new Image_Prefs;
    _file_browser_prefs = new File_Browser_Prefs;

    //! Force image I/O plugin loading.

    Image_Io_Base_Widget_Factory::global();
    Image_Load_Widget_Factory::global();
    Image_Save_Widget_Factory::global();
}

Application::~Application()
{
    //DJV_DEBUG("Application::~Application");

    delete _file_browser_prefs;
    delete _image_prefs;
    delete _style_prefs;
    delete _prefs_global;
    delete _prefs_;
}

int Application::exec()
{
    //DJV_DEBUG("Application::exec");
    //DJV_DEBUG_PRINT("valid = " << _is_valid);

    if (exit() != EXIT_DEFAULT)
    {
        exit(Core_Application::exit());
    }

    Fl::run();

    return Core_Application::exec();
}

void Application::is_valid(bool in)
{
    //DJV_DEBUG("Application::is_valid");
    //DJV_DEBUG_PRINT("in = " << in);

    _is_valid = in;
}

bool Application::is_valid() const
{
    return _is_valid;
}

void Application::widget_add(Abstract_Widget * in)
{
    //DJV_DEBUG("Application::widget_add");
    //DJV_DEBUG_PRINT("in = " << reinterpret_cast<int64_t>(in));

    _widgets += in;
}

void Application::widget_del(Abstract_Widget * in)
{
    //DJV_DEBUG("Application::widget_del");

    List<Abstract_Widget *>::iterator i = std::find(
        _widgets.begin(),
        _widgets.end(),
        in);

    if (i != _widgets.end())
    {
        _widgets.erase(i);
    }
}

const Abstract_Widget * Application::widget_get(const Fl_Widget * in)
{
    const Abstract_Widget * out = 0;

    for (size_t i = 0; i < _widgets.size(); ++i)
    {
        if (in == _widgets[i]->fltk_widget())
        {
            out = _widgets[i];
            break;
        }
    }

    return out;
}

void Application::window_add(Window * in)
{
    //DJV_DEBUG("Application::window_add");
    //DJV_DEBUG_PRINT("window = " << reinterpret_cast<int64_t>(in));
    //DJV_DEBUG_PRINT("window = " << in->title());

    _windows += in;

    window_add_signal.emit(in);
}

void Application::window_del(Window * in)
{
    //DJV_DEBUG("Application::window_del");

    List<Window *>::iterator i = std::find(
        _windows.begin(),
        _windows.end(),
        in);

    if (i != _windows.end())
    {
        _windows.erase(i);

        window_del_signal.emit(in);
    }
}

void Application::window_focus(Window * in)
{
    //DJV_DEBUG("Application::window_focus");

    window_focus_signal.emit(in);
}

void Application::window_redraw()
{
    //DJV_DEBUG("Application::window_redraw");

    for (size_t i = 0; i < _windows.size(); ++i)
    {
        _windows[i]->redraw();
    }
}

void Application::window_update()
{
    //DJV_DEBUG("Application::window_update");

    for (size_t i = 0; i < _windows.size(); ++i)
    {
        _windows[i]->dirty();
        _windows[i]->update();
    }
}

void Application::window_hide()
{
    // Close all windows.

    for (size_t i = 0; i < _windows.size(); ++i)
    {
        _windows[i]->hide();
    }
}

void Application::exit(EXIT in)
{
    //DJV_DEBUG("Application::exit");

    window_hide();

    if (in != EXIT_DEFAULT)
    {
        Core_Application::exit(in);
    }
}

Application::EXIT Application::exit() const
{
    return Core_Application::exit();
}

namespace
{

const String label_info =
"%%"
"\n"
" Image I/O Widget Plugins\n"
"\n"
"     Base: %%\n"
"     Load: %%\n"
"     Save: %%\n"
"\n"
" Preferences\n"
"\n"
"     Application: %%\n"
"     Global:      %%\n";

} // namespace

String Application::info() const
{
    return String_Format(label_info).
        arg(Core_Application::info()).
        arg(Image_Io_Base_Widget_Factory::global()->names(), ", ").
        arg(Image_Load_Widget_Factory::global()->names(), ", ").
        arg(Image_Save_Widget_Factory::global()->names(), ", ").
        arg(Prefs::prefs()->file_name()).
        arg(Prefs::prefs_global()->file_name());
}

void Application::message(const String & in) const
{
    if (_is_valid)
    {
        Application_Message_Dialog::message(in);
    }
    else
    {
        Core_Application::message(in);
    }
}

void Application::error(const Error & in) const
{
    if (_is_valid)
    {
        Application_Message_Dialog::error(in);
    }
    else
    {
        Core_Application::error(in);
    }
}

void Application::help() const
{
    //DJV_DEBUG("Application::help");

    const File url(File_Util::path_fix(path() + "../doc") + "djv_view.html");

    //DJV_DEBUG_PRINT("url = " << url);

    System::open_url(url);
}

} // djv

