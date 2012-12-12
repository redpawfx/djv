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

//! \file djv_view_window.h

#ifndef DJV_VIEW_WINDOW_H
#define DJV_VIEW_WINDOW_H

#include <djv_view_image.h>
#include <djv_view_input.h>
#include <djv_view_playback.h>
#include <djv_view_view.h>

#include <djv_menu.h>
#include <djv_window.h>

#include <djv_gl_image.h>
#include <djv_image.h>

namespace djv
{

class Frame;
class Label;
class Menu;
class Tool_Button;

} // djv

namespace djv_view
{
using namespace djv;

struct Display_Profile;
class File_Group;
class Help_Group;
class Image_Group;
class Info_Swatch;
class Tool_Group;
class View_Group;
class View_Widget;

//------------------------------------------------------------------------------
//! \class Window
//!
//! This class provides a main window.
//------------------------------------------------------------------------------

class Window : public djv::Window
{
public:

    //! Maximum percentage of the screen to use when resizing.

    enum RESIZE_MAX
    {
        RESIZE_MAX_UNLIMITED,
        RESIZE_MAX_25,
        RESIZE_MAX_50,
        RESIZE_MAX_75,

        _RESIZE_SIZE
    };

    //! Get the resize labels.

    static const List<String> & label_resize_max();

    //! Get the resize amount.

    static double resize_max(RESIZE_MAX);

    //! Toolbar options.

    enum TOOLBAR
    {
        TOOLBAR_BUTTONS,
        TOOLBAR_PLAYBACK,
        TOOLBAR_INFO,

        _TOOLBAR_SIZE
    };

    //! Get the toolbar option labels.

    static const List<String> & label_toolbar();

    //! Constructor.

    Window(const Window * copy = 0);

    //! Destructor.

    virtual ~Window();

    virtual void del();

    //! Open a file.

    void file_open(const File &, bool init = true);

    //! Set whether automatic file sequencing is enabled.

    void file_seq_auto(bool);

    //! Set the layer to open.

    void file_layer(int);

    //! Set the proxy scale.

    void file_proxy(Pixel_Data_Info::PROXY);

    //! Set whether the cache is enabled.

    void file_cache(bool);

    //! Set whether fullscreen is enabled.

    void window_fullscreen(bool);

    //! Set the toolbar visibility.

    void window_toolbar(const List<bool> &);

    //! Get the toolbar visibility.

    const List<bool> & window_toolbar() const;

    //! Set the view grid.

    void view_grid(View::GRID);

    //! Set whether the view HUD is enabled.

    void view_hud(bool);

    //! Set the playback.

    void playback(Playback::PLAYBACK);

    //! Set the playback frame.

    void playback_frame(int64_t);

    //! Set the playback speed.

    void playback_speed(const Speed &);

    virtual int handle(int);

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

private:

    void window_fit();
    void menu_update(Menu_Item_Group *);
    void toolbar_update();

    Gl_Image_Options image_options() const;

    V2i image_pick() const;

    DJV_CALLBACK(Window, file_open_callback, const File &);
    DJV_CALLBACK(Window, file_reload_callback, bool);
    DJV_CALLBACK(Window, file_save_callback, const File &);
    DJV_CALLBACK(Window, file_save_frame_callback, const File &);

    DJV_CALLBACK(Window, window_new_callback, bool);
    DJV_CALLBACK(Window, window_copy_callback, bool);
    DJV_CALLBACK(Window, window_close_callback, bool);
    DJV_CALLBACK(Window, window_close2_callback, bool);
    DJV_CALLBACK(Window, window_fit_callback, bool);
    DJV_CALLBACK(Window, window_fullscreen_callback, bool);
    DJV_CALLBACK(Window, window_toolbar_callback, const List<bool> &);
    DJV_CALLBACK(Window, window_toolbar_buttons_callback, bool);
    DJV_CALLBACK(Window, window_toolbar_playback_callback, bool);
    DJV_CALLBACK(Window, window_toolbar_info_callback, bool);
    DJV_FL_WIDGET_CALLBACK(Window, _window_new_callback);
    DJV_FL_WIDGET_CALLBACK(Window, _window_copy_callback);
    DJV_FL_WIDGET_CALLBACK(Window, _window_close_callback);
    DJV_FL_WIDGET_CALLBACK(Window, _window_fit_callback);
    DJV_FL_WIDGET_CALLBACK(Window, _window_fullscreen_callback);
    DJV_FL_WIDGET_CALLBACK(Window, _window_toolbar_buttons_callback);
    DJV_FL_WIDGET_CALLBACK(Window, _window_toolbar_playback_callback);
    DJV_FL_WIDGET_CALLBACK(Window, _window_toolbar_info_callback);

    DJV_CALLBACK(Window, view_callback, bool);
    DJV_CALLBACK(Window, view_move_callback, const V2i &);
    DJV_CALLBACK(Window, view_center_callback, bool);
    DJV_CALLBACK(Window, view_zoom_callback, double);
    DJV_CALLBACK(Window, view_zoom_reset_callback, bool);
    DJV_CALLBACK(Window, view_reset_callback, bool);
    DJV_CALLBACK(Window, view_fit_callback, bool);
    DJV_CALLBACK(Window, view_grid_callback, View::GRID);
    DJV_CALLBACK(Window, view_hud_callback, bool);
    DJV_CALLBACK(Window, view_pick_callback, const V2i &);
    DJV_CALLBACK(Window, view_overlay_callback, bool);

    DJV_CALLBACK(Window, image_callback, bool);
    DJV_CALLBACK(Window, image_redraw_callback, bool);
    DJV_CALLBACK(Window, image_resize_callback, bool);
    DJV_CALLBACK(Window, image_frame_store_callback, bool);
    DJV_CALLBACK(Window, image_display_profile_callback, bool);
    DJV_CALLBACK(Window, image_display_profile_value_callback,
                 const Display_Profile &);

    DJV_CALLBACK(Window, tool_magnify_callback, bool);
    DJV_CALLBACK(Window, tool_color_picker_callback, bool);
    DJV_CALLBACK(Window, tool_histogram_callback, bool);
    DJV_CALLBACK(Window, tool_info_callback, bool);

    DJV_CALLBACK(Window, mouse_wheel_callback, Input::MOUSE_WHEEL);
    DJV_CALLBACK(Window, mouse_wheel_value_callback, int);

    DJV_CALLBACK(Window, cache_callback, const List<int64_t> &);

    DJV_CALLBACK(Window, group_callback, bool);

    void file_update();
    void window_toolbar_update();
    void window_fullscreen_update();
    void window_resize_update();
    void view_update();
    void view_resize_update();
    void view_overlay_update();
    void image_update();
    void image_redraw_update();
    void image_dialog_update();
    void image_focus_update();
    void image_pick_update();
    void tool_dialog_update();
    void cache_update();
    void group_update();

    Input::MOUSE_WHEEL    _mouse_wheel;
    File_Group *          _file;
    bool                  _window_fullscreen;
    List<bool>            _window_toolbar;
    View_Group *          _view;
    Image_Group *         _image;
    const djv::Image *    _image_p;
    djv::Image            _image_tmp;
    V2i                   _image_pick;
    Color                 _image_sample;
    Playback_Group *      _playback;
    int64_t               _playback_frame_tmp;
    double                _playback_speed_tmp;
    Tool_Group *          _tool;
    Help_Group *          _help;
    bool                  _is_valid;
    Gl_Offscreen_Buffer * _sample_buffer;
    Gl_Image_State        _gl_state;
    Menu *                _menu;
    Frame *               _toolbar0;
    Tool_Button *         _window_new_widget;
    Tool_Button *         _window_copy_widget;
    Tool_Button *         _window_close_widget;
    Tool_Button *         _window_fit_widget;
    Tool_Button *         _window_fullscreen_widget;
    Frame *               _toolbar1;
    Widget *              _toolbar1_playback;
    Widget *              _toolbar1_info;
    Info_Swatch *         _info_swatch;
    Label *               _info_pixel_widget;
    Label *               _info_image_widget;
    Label *               _info_cache_widget;
    View_Widget *         _view_widget;
};

//------------------------------------------------------------------------------
//! \class Window_Factory
//!
//! This class provides a main window factory.
//------------------------------------------------------------------------------

class Window_Factory
{
public:

    //! Constructor.

    ~Window_Factory();

    //! Add a window.

    Window * add(const Window * copy = 0);

    //! Remove a window.

    void del(Window *);

    //! Get the list of windows.

    const List<Window *> list() const;

    //! Get the global factory.

    static Window_Factory * global();

private:

    List<Window *> _list;
};

//------------------------------------------------------------------------------

String & operator >> (String &, Window::RESIZE_MAX &) throw (String);
String & operator >> (String &, Window::TOOLBAR &) throw (String);

String & operator << (String &, Window::RESIZE_MAX);
String & operator << (String &, Window::TOOLBAR);

} // djv_view

#endif // DJV_VIEW_WINDOW_H

