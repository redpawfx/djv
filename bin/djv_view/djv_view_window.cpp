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

//! \file djv_view_window.cpp

#include <djv_view_window.h>

#include <djv_view_color_picker.h>
#include <djv_view_display_profile_dialog.h>
#include <djv_view_file_group.h>
#include <djv_view_file_prefs.h>
#include <djv_view_file_save.h>
#include <djv_view_help_group.h>
#include <djv_view_histogram_dialog.h>
#include <djv_view_info_dialog.h>
#include <djv_view_image_group.h>
#include <djv_view_magnify_dialog.h>
#include <djv_view_playback_group.h>
#include <djv_view_playback_prefs.h>
#include <djv_view_shortcut.h>
#include <djv_view_shortcut_prefs.h>
#include <djv_view_tool_group.h>
#include <djv_view_view_group.h>
#include <djv_view_view_prefs.h>
#include <djv_view_view_widget.h>
#include <djv_view_window_prefs.h>

#include <djv_application.h>
#include <djv_color_swatch.h>
#include <djv_color_dialog.h>
#include <djv_frame_group.h>
#include <djv_icon.h>
#include <djv_image_io.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_menu.h>
#include <djv_style.h>
#include <djv_tool_button.h>

#include <djv_gl_offscreen_buffer.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/x.H>

#if defined(DJV_WINDOWS)
#include "../../lib/djv_gui/winrc.h"
#elif defined(DJV_MACOS)
#elif defined(DJV_X11)
#include <X11/xpm.h>
#include "../../etc/X11/projector32x32.xpm"
#endif

#include <algorithm>

namespace djv_view
{

//------------------------------------------------------------------------------
// Info_Swatch
//------------------------------------------------------------------------------

class Info_Swatch : public Color_Swatch
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
        Color_Swatch::dirty();

        fl_font(Style::global()->font(), Style::global()->font_size());

        size_hint(V2i(fl_height()));
    }

private:

    DJV_CALLBACK(Info_Swatch, dialog_callback, bool)
    {
        Color_Dialog::dialog(get(), this, 0);
    }
};

//------------------------------------------------------------------------------
// Window
//------------------------------------------------------------------------------

const List<String> & Window::label_resize_max()
{
    static const List<String> data = List<String>() <<
        "Unlimited" <<
        "25% of Screen" <<
        "50% of Screen" <<
        "75% of Screen";

    DJV_ASSERT(data.size() == _RESIZE_SIZE);

    return data;
}

double Window::resize_max(RESIZE_MAX in)
{
    switch (in)
    {
        case RESIZE_MAX_25:
            return 0.25;

        case RESIZE_MAX_50:
            return 0.50;

        case RESIZE_MAX_75:
            return 0.75;

        default:
            break;
    }

    return 1.0;
}

const List<String> & Window::label_toolbar()
{
    static const List<String> data = List<String>() <<
        "Buttons" <<
        "Playback" <<
        "Information";

    DJV_ASSERT(data.size() == _TOOLBAR_SIZE);

    return data;
}

namespace
{

const String
    label_toolbar_info_pixel_size =
        "Pixel: 4096, 4096, RGBA F32 1.0 1.0 1.0 1.0";

} // namespace

Window::Window(const Window * copy) :
    djv::Window(DJV_APP->name()),
    _mouse_wheel             (static_cast<Input::MOUSE_WHEEL>(0)),
    _window_fullscreen       (false),
    _window_toolbar          (Window_Prefs::global()->toolbar()),
    _image_p                 (0),
    _playback_frame_tmp      (0),
    _playback_speed_tmp      (0.0),
    _is_valid                (false),
    _sample_buffer           (0),
    _menu                    (0),
    _toolbar0                (0),
    _window_new_widget       (0),
    _window_copy_widget      (0),
    _window_close_widget     (0),
    _window_fit_widget       (0),
    _window_fullscreen_widget(0),
    _toolbar1                (0),
    _toolbar1_playback       (0),
    _toolbar1_info           (0),
    _info_swatch             (0),
    _info_pixel_widget       (0),
    _info_image_widget       (0),
    _info_cache_widget       (0),
    _view_widget             (0)
{
    //DJV_DEBUG("Window::Window");

    if (copy)
    {
        _window_toolbar = copy->_window_toolbar;
        _image_tmp      = copy->_image_tmp;
        _image_pick     = copy->_image_pick;
    }

#if defined(DJV_WINDOWS)

    HICON icon = LoadIcon(fl_display, MAKEINTRESOURCE(DJV_ICON));

    if (! icon)
    {
        //DJV_DEBUG_PRINT("error = " << error::last_error());
    }
    else
    {
        Fl_Double_Window::icon((char *)icon);
    }

#endif

    arrow_navkeys(false);

    // Create widgets.

    _menu = new Menu;

    _toolbar0 = new Frame;
    _toolbar0->style(Frame::STYLE_EMPTY);

    _window_new_widget = new Tool_Button("window_new");

    _window_copy_widget = new Tool_Button("window_copy");

    _window_close_widget = new Tool_Button("window_close");

    _window_fit_widget = new Tool_Button("window_fit");

    _window_fullscreen_widget = new Tool_Button("window_fullscreen");
    _window_fullscreen_widget->type(Tool_Button::TOGGLE);

    _toolbar1 = new Frame;
    _toolbar1->style(Frame::STYLE_EMPTY);
    _toolbar1_playback = new Widget;
    _toolbar1_info = new Widget;

    _info_swatch = new Info_Swatch;

    _info_pixel_widget = new Label;
    _info_pixel_widget->size_string(label_toolbar_info_pixel_size);

    _info_image_widget = new Label;

    _info_cache_widget = new Label;

    _view_widget = new View_Widget;

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->spacing(0);
    layout->margin(0);
    layout->add(_menu);

    layout->add(_toolbar0);
    Horizontal_Layout * toolbar0_layout = new Horizontal_Layout(_toolbar0);

    Horizontal_Layout * toolbar_file_layout =
        new Horizontal_Layout(toolbar0_layout);
    toolbar_file_layout->margin(0);

    Horizontal_Layout * toolbar_window_layout =
        new Horizontal_Layout(toolbar0_layout);
    toolbar_window_layout->margin(0);
    toolbar_window_layout->add(new Horizontal_Frame_Group(List<Frame *>() <<
       _window_new_widget <<
       _window_copy_widget <<
       _window_close_widget <<
       _window_fit_widget <<
       _window_fullscreen_widget));

    Horizontal_Layout * toolbar_view_layout =
        new Horizontal_Layout(toolbar0_layout);
    toolbar_view_layout->margin(0);

    Horizontal_Layout * toolbar_image_layout =
        new Horizontal_Layout(toolbar0_layout);
    toolbar_image_layout->margin(0);

    Horizontal_Layout * toolbar_tool_layout =
        new Horizontal_Layout(toolbar0_layout);
    toolbar_tool_layout->margin(0);

    toolbar0_layout->add_stretch();

    layout->add(_view_widget);
    layout->stretch(_view_widget);

    layout->add(_toolbar1);
    Vertical_Layout * toolbar1_layout = new Vertical_Layout(_toolbar1);

    toolbar1_layout->add(_toolbar1_playback);
    Horizontal_Layout * toolbar1_playback_layout =
        new Horizontal_Layout(_toolbar1_playback);
    toolbar1_playback_layout->margin(0);

    toolbar1_layout->add(_toolbar1_info);
    Horizontal_Layout * toolbar1_info_layout
        = new Horizontal_Layout(_toolbar1_info);
    toolbar1_info_layout->spacing(Style::global()->spacing_large());
    toolbar1_info_layout->margin(0);
    Horizontal_Layout * layout_h = new Horizontal_Layout(toolbar1_info_layout);
    layout_h->size(FIXED, FIXED);
    layout_h->margin(0);
    layout_h->add(_info_swatch);
    layout_h->add(_info_pixel_widget);
    toolbar1_info_layout->add_spacer(-1, true);
    toolbar1_info_layout->add(_info_image_widget);
    toolbar1_info_layout->add(_info_cache_widget);
    toolbar1_info_layout->add_spacer(Layout::window_handle_size());

    // Initialize.

    _file = new File_Group(_menu, toolbar_file_layout, copy ? copy->_file : 0);

    _view = new View_Group(_menu, toolbar_view_layout, copy ? copy->_view : 0);

    _image = new Image_Group(
        _menu,
        toolbar_image_layout,
        copy ? copy->_image : 0);

    _playback = new Playback_Group(
        _menu,
        toolbar1_playback_layout,
        copy ? copy->_playback : 0);

    _tool = new Tool_Group(_menu, toolbar_tool_layout, copy ? copy->_tool : 0);

    _help = new Help_Group(_menu, copy ? copy->_help : 0);

    if (copy)
    {
        _view_widget->view(copy->_view_widget->view());
        _view_widget->zoom(copy->_view_widget->zoom());
    }

    _view_widget->hud(_view->hud());
    _view_widget->grid(_view->grid());

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
        //if (Window_Prefs::global()->resize_fit())
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

    _window_new_widget->signal.set(this, window_new_callback);
    _window_copy_widget->signal.set(this, window_copy_callback);
    _window_close_widget->signal.set(this, window_close_callback);
    _window_fit_widget->signal.set(this, window_fit_callback);
    _window_fullscreen_widget->signal.set(this, window_fullscreen_callback);

    _view->move_signal.set(this, view_move_callback);
    _view->center_signal.set(this, view_center_callback);
    _view->zoom_signal.set(this, view_zoom_callback);
    _view->zoom_reset_signal.set(this, view_zoom_reset_callback);
    _view->reset_signal.set(this, view_reset_callback);
    _view->fit_signal.set(this, view_fit_callback);
    _view->overlay_signal.set(this, view_overlay_callback);
    _view->update_signal.set(this, group_callback);

    _view_widget->signal.set(this, view_callback);
    _view_widget->pick_signal.set(this, view_pick_callback);
    _view_widget->mouse_wheel_signal.set(this, mouse_wheel_callback);
    _view_widget->mouse_wheel_value_signal.set(
        this, mouse_wheel_value_callback);
    _view_widget->file_signal.set(this, file_open_callback);

    _image->redraw_signal.set(this, image_redraw_callback);
    _image->resize_signal.set(this, image_resize_callback);
    _image->frame_store_signal.set(this, image_frame_store_callback);
    _image->display_profile_signal.set(this, image_display_profile_callback);
    _image->display_profile_value_signal.set(
        this, image_display_profile_value_callback);
    _image->update_signal.set(this, group_callback);

    _playback->image_signal.set(this, image_callback);
    _playback->overlay_signal.set(this, view_overlay_callback);
    _playback->update_signal.set(this, group_callback);

    _tool->magnify_signal.set(this, tool_magnify_callback);
    _tool->color_picker_signal.set(this, tool_color_picker_callback);
    _tool->histogram_signal.set(this, tool_histogram_callback);
    _tool->info_signal.set(this, tool_info_callback);

    Window_Prefs::global()->toolbar_signal.set(this, window_toolbar_callback);

    close_signal.set(this, window_close2_callback);

    _is_valid = true;
}

Window::~Window()
{
    //DJV_DEBUG("Window::~Window");

    delete _file;
    delete _view;
    delete _image;
    delete _playback;
    delete _tool;
    delete _help;
}

void Window::del()
{
    //DJV_DEBUG("Window::del");

    // Cleanup.

    djv::Window::del();

    Display_Profile_Dialog::global()->del(_view_widget);

    Info_Dialog::global()->del(_view_widget);
    Histogram_Dialog::global()->del(_view_widget);
    Color_Picker::global()->del(_view_widget);
    Magnify_Dialog::global()->del(_view_widget);

    _view_widget->set(0);

    _file->del();
    _view->del();
    _image->del();
    _playback->del();
    _tool->del();
    _help->del();

    delete _sample_buffer;
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

                    //! \todo Needed for shortcut handling when the menubar
                    //! isn't visible?

                    if (! _menu->visible())
                    {
                        if (_menu->test_shortcut())
                        {
                            return 1;
                        }
                    }

                    break;
            }

            break;
    }

    return djv::Window::handle(in);
}

void Window::dirty()
{
    //DJV_DEBUG("Window::dirty");

    djv::Window::dirty();

    if (_is_valid)
    {
        view_overlay_update();
        image_redraw_update();
        group_update();
    }
}

void Window::geom(const Box2i & in)
{
    djv::Window::geom(in);

    view_resize_update();
    view_update();
}

const Box2i & Window::geom() const
{
    return djv::Window::geom();
}

namespace
{

const String
    menu_title = "Window",
    menu_new = "&New",
    menu_copy = "&Copy",
    menu_close = "Clos&e",
    menu_fit = "&Fit",
    menu_fullscreen = "F&ullscreen",
    menu_toolbar_buttons = "Bu&ttons",
    menu_toolbar_playback = "Pla&yback",
    menu_toolbar_info = "Information";

} // namespace

void Window::menu_update(Menu_Item_Group * in)
{
    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

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

    in->add(menu_title, 0, 0, 0, Menu_Item::SUB_MENU);

    in->add(
        menu_new,
        shortcuts[Shortcut::WINDOW_NEW].value,
        _window_new_callback,
        this);

    in->add(
        menu_copy,
        shortcuts[Shortcut::WINDOW_COPY].value,
        _window_copy_callback,
        this);

    in->add(
        menu_close,
        shortcuts[Shortcut::WINDOW_CLOSE].value,
        _window_close_callback,
        this,
        Menu_Item::DIVIDER);

    in->add(
        menu_fit,
        shortcuts[Shortcut::WINDOW_FIT].value,
        _window_fit_callback,
        this);

    in->add(
        menu_fullscreen,
        shortcuts[Shortcut::WINDOW_FULLSCREEN].value,
        _window_fullscreen_callback,
        this,
        Menu_Item::TOGGLE | Menu_Item::DIVIDER,
        _window_fullscreen);

    in->add(
        menu_toolbar_buttons,
        shortcuts[Shortcut::WINDOW_TOOLBAR_BUTTONS].value,
        _window_toolbar_buttons_callback,
        this,
        Menu_Item::TOGGLE,
        _window_toolbar[TOOLBAR_BUTTONS]);

    in->add(
        menu_toolbar_playback,
        shortcuts[Shortcut::WINDOW_TOOLBAR_PLAYBACK].value,
        _window_toolbar_playback_callback,
        this,
        Menu_Item::TOGGLE,
        _window_toolbar[TOOLBAR_PLAYBACK]);

    in->add(
        menu_toolbar_info,
        shortcuts[Shortcut::WINDOW_TOOLBAR_INFO].value,
        _window_toolbar_info_callback,
        this,
        Menu_Item::TOGGLE,
        _window_toolbar[TOOLBAR_INFO]);

    in->end();
}

namespace
{

const String
    tooltip_new = "Create a new window\n\nShortcut: %%",
    tooltip_copy = "Copy this window\n\nShortcut: %%",
    tooltip_close = "Close this window\n\nShortcut: %%",
    tooltip_fit = "Resize window to fit image\n\nShortcut: %%",
    tooltip_fullscreen = "Fullscreen\n\nShortcut: %%",
    tooltip_info_pixel =
        "Pixel information\n\nClick and drag inside the image.",
    tooltip_info_image = "Image information",
    tooltip_info_cache =
        "Cache information\n\nKey: Window-usage / (Total-usage / Maximum)";

} // namespace

void Window::toolbar_update()
{
    // Update tooltips.

    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    _window_new_widget->tooltip(String_Format(tooltip_new).
        arg(djv::Shortcut::label(shortcuts[Shortcut::WINDOW_NEW].value)));

    _window_copy_widget->tooltip(String_Format(tooltip_copy).
        arg(djv::Shortcut::label(shortcuts[Shortcut::WINDOW_COPY].value)));

    _window_close_widget->tooltip(String_Format(tooltip_close).
        arg(djv::Shortcut::label(shortcuts[Shortcut::WINDOW_CLOSE].value)));

    _window_fit_widget->tooltip(String_Format(tooltip_fit).
        arg(djv::Shortcut::label(shortcuts[Shortcut::WINDOW_FIT].value)));

    _window_fullscreen_widget->tooltip(String_Format(tooltip_fullscreen).
        arg(djv::Shortcut::label(
            shortcuts[Shortcut::WINDOW_FULLSCREEN].value)));

    _info_pixel_widget->tooltip(tooltip_info_pixel);
    _info_image_widget->tooltip(tooltip_info_image);
    _info_cache_widget->tooltip(tooltip_info_cache);
}

void Window::file_open(const File & in, bool init)
{
    //DJV_DEBUG("Window::file_open");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("init = " << init);

    // Initialize.

    _image_p = 0;

    _view_widget->set(0);

    // Open the file.

    callbacks(false);

    _file->open(in);

    // Set playback.

    const Seq & seq = _file->info().seq;

    _playback->seq(seq);

    if (init)
    {
        _playback->frame(0);

        _playback->playback(
            (seq.list.size() > 1 && Playback_Prefs::global()->start()) ?
            Playback::FORWARD :
            Playback::STOP);
    }

    callbacks(true);

    // Update.

    file_update();
    image_update();

    if (init)
    {
        if (Window_Prefs::global()->resize_fit())
        {
            if (! _window_fullscreen)
            {
                _view_widget->view_zero();

                window_fit();
            }
            else
            {
                _view_widget->view_fit();
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

    Seq seq;
    seq.list = List<int64_t>(
       _playback->seq().list,
       _playback->in(),
       _playback->out() - _playback->in() + 1);
    seq.speed = _playback->speed();

    File_Save::global()->set(
        File_Save_Info(
            _file->file(),
            in,
            _file->info()[_file->layer()],
            seq,
            _file->layer(),
            _file->proxy(),
            _file->u8_conversion(),
            _image->color_profile(),
            image_options()));
}

void Window::file_save_frame_callback(const File & in)
{
    //DJV_DEBUG("Window::file_save_frame_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    // Save a single frame.

    Seq seq;

    const List<int64_t> & list = _playback->seq().list;

    if (list.size())
    {
        seq.list += list[_playback->frame()];
    }

    seq.speed = _playback->speed();

    //DJV_DEBUG_PRINT("seq = " << seq);

    File_Save::global()->set(
        File_Save_Info(
            _file->file(),
            in,
            _file->info()[_file->layer()],
            seq,
            _file->layer(),
            _file->proxy(),
            _file->u8_conversion(),
            _image->color_profile(),
            image_options()));
}

void Window::file_update()
{
    //DJV_DEBUG("Window::file_update");

    callbacks(false);

    // Update window title.

    const File & file = _file->file();

    title(
        ! file.get().empty() ?
        String_Format("%% - %%").arg(DJV_APP->name()).arg(file) :
        String_Format("%%").arg(DJV_APP->name()));

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

void Window::file_proxy(Pixel_Data_Info::PROXY in)
{
    _file->proxy(in);
}

void Window::file_cache(bool in)
{
    _file->cache(in);
}

void Window::window_new_callback(bool)
{
    //DJV_DEBUG("Window::window_new_callback");

    Window_Factory::global()->add()->show();
}

void Window::_window_new_callback()
{
    window_new_callback(true);
}

void Window::window_copy_callback(bool)
{
    //DJV_DEBUG("Window::window_copy_callback");

    Window_Factory::global()->add(this)->show();
}

void Window::_window_copy_callback()
{
    window_copy_callback(true);
}

void Window::window_close_callback(bool)
{
    if (1 == Window_Factory::global()->list().size())
    {
        return;
    }

    //DJV_DEBUG("Window::window_close_callback");

    window_close2_callback(true);
}

void Window::window_close2_callback(bool)
{
    Window_Factory::global()->del(this);
}

void Window::_window_close_callback()
{
    window_close_callback(true);
}

void Window::window_fit()
{
    //DJV_DEBUG("Window::window_fit");

    // Calculate image size.

    const Box2f bbox = _view_widget->bbox().size;

    //DJV_DEBUG_PRINT("bbox = " << bbox);

    V2i image_size = Vector_Util::ceil<double, int>(bbox.size);

    if (! Vector_Util::is_size_valid(image_size))
    {
        image_size = V2i(600, 300);
    }

    //DJV_DEBUG_PRINT("image size = " << image_size);

    // Adjust to screen size.

    const double resize_max = Window::resize_max(
        Window_Prefs::global()->resize_fit() ?
        Window_Prefs::global()->resize_max() :
        RESIZE_MAX_UNLIMITED);

    const V2i max(
        static_cast<int>(Fl::w() * resize_max),
        static_cast<int>(Fl::h() * resize_max));

    if (image_size.x > max.x || image_size.y > max.y)
        image_size =
            image_size.x > image_size.y ?
            V2i(
                max.x,
                Math::ceil(
                    image_size.y / static_cast<double>(image_size.x) * max.x)) :
            V2i(
                Math::ceil(
                    image_size.x / static_cast<double>(image_size.y) * max.y),
                max.y);

    // Adjust to size hint.

    const V2i ui_size = size_hint() - V2i(0, _view_widget->size_hint().y);

    //DJV_DEBUG_PRINT("ui size = " << ui_size);

    const V2i size = V2i(
        Math::max(image_size.x, ui_size.x),
        image_size.y + ui_size.y);

    //DJV_DEBUG_PRINT("size = " << size);

    // Set size.

    fullscreen(false);
    _window_fullscreen = false;

    this->size(size);

    _view_widget->view_fit();
    _view_widget->redraw();

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
    {
        return;
    }

    //DJV_DEBUG("Window::window_fullscreen");
    //DJV_DEBUG_PRINT("in = " << in);

    // Set value.

    _window_fullscreen = in;
    fullscreen(_window_fullscreen);

    // Update.

    window_fullscreen_update();
    group_update();
    update();

    if (Window_Prefs::global()->resize_fit())
    {
        _view_widget->view_fit();
    }
    else
    {
        view_resize_update();
    }

    view_update();
}

void Window::window_fullscreen_callback(bool in)
{
    window_fullscreen(in);
}

void Window::_window_fullscreen_callback()
{
    window_fullscreen(_menu->value());
}

void Window::window_fullscreen_update()
{
    //DJV_DEBUG("Window::window_fullscreen_update");

    callbacks(false);

    _window_fullscreen_widget->set(_window_fullscreen);

    if (_window_fullscreen)
    {
        // Hide widgets.

        _menu->hide();

        _toolbar0->hide();
        _toolbar1->hide();
    }
    else
    {
        // Show widgets.

        _menu->show();

        if (_window_toolbar[TOOLBAR_BUTTONS])
        {
            _toolbar0->show();
        }

        if (
            _window_toolbar[TOOLBAR_PLAYBACK] ||
            _window_toolbar[TOOLBAR_INFO])
        {
            _toolbar1->show();
        }
    }

    callbacks(true);
}

void Window::window_toolbar(const List<bool> & in)
{
    if (in == _window_toolbar)
    {
        return;
    }

    // Set value.

    _window_toolbar = in;

    // Update.

    window_toolbar_update();
    //window_resize_update();
    group_update();
    update();
    window_resize_update(); //! \todo Needs update() first.
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
    window_toolbar_buttons_callback(_menu->value());
}

void Window::window_toolbar_playback_callback(bool in)
{
    List<bool> toolbar = _window_toolbar;
    toolbar[TOOLBAR_PLAYBACK] = in;
    window_toolbar(toolbar);
}

void Window::_window_toolbar_playback_callback()
{
    window_toolbar_playback_callback(_menu->value());
}

void Window::window_toolbar_info_callback(bool in)
{
    List<bool> toolbar = _window_toolbar;
    toolbar[TOOLBAR_INFO] = in;
    window_toolbar(toolbar);
}

void Window::_window_toolbar_info_callback()
{
    window_toolbar_info_callback(_menu->value());
}

void Window::window_toolbar_update()
{
    //DJV_DEBUG("Window::window_toolbar_update");

    callbacks(false);

    // Set toolbar0 visibility.

    if (_window_toolbar[TOOLBAR_BUTTONS] && ! _window_fullscreen)
    {
        _toolbar0->show();
    }
    else
    {
        _toolbar0->hide();
    }

    // Set toolbar1 visibility.

    bool playback = _window_toolbar[TOOLBAR_PLAYBACK];
    bool info = _window_toolbar[TOOLBAR_INFO];

    //DJV_DEBUG_PRINT("playback = " << playback);
    //DJV_DEBUG_PRINT("info = " << info);

    if (playback && ! _window_fullscreen)
    {
        _toolbar1_playback->show();
    }
    else
    {
        _toolbar1_playback->hide();
    }

    if (info && ! _window_fullscreen)
    {
        _toolbar1_info->show();
    }
    else
    {
        _toolbar1_info->hide();
    }

    if ((playback || info) && ! _window_fullscreen)
    {
        //DJV_DEBUG_PRINT("toolbar1 show");

        _toolbar1->show();
    }
    else
    {
        //DJV_DEBUG_PRINT("toolbar1 hide");

        _toolbar1->hide();
    }

    _playback->toolbar_info_update(_window_toolbar[TOOLBAR_INFO]);

    callbacks(true);
}

void Window::window_resize_update()
{
    //DJV_DEBUG("Window::window_resize_update");
    //DJV_DEBUG_PRINT("geom = " << geom());

    callbacks(false);

    if (Window_Prefs::global()->resize_fit())
    {
        if (! _window_fullscreen)
        {
            window_fit();
        }
        else
        {
            _view_widget->view_fit();
        }
    }

    callbacks(true);
}

void Window::view_grid(View::GRID in)
{
    _view->grid(in);
}

void Window::view_hud(bool in)
{
    _view->hud(in);
}

void Window::view_callback(bool)
{
    //DJV_DEBUG("Window::view_callback");

    view_update();
}

void Window::view_move_callback(const V2i & in)
{
    _view_widget->view(_view_widget->view() + in);
    _view_widget->redraw();
}

void Window::view_center_callback(bool)
{
    _view_widget->view_center();
    _view_widget->redraw();
}

void Window::view_zoom_callback(double in)
{
    _view_widget->zoom_focus(_view_widget->zoom() * in);
    _view_widget->redraw();
}

void Window::view_zoom_reset_callback(bool)
{
    _view_widget->zoom_focus(1.0);
    _view_widget->redraw();
}

void Window::view_reset_callback(bool)
{
    _view_widget->view_zero();
    _view_widget->redraw();
}

void Window::view_fit_callback(bool)
{
    _view_widget->view_fit();
    _view_widget->redraw();
}

void Window::view_update()
{
    //DJV_DEBUG("Window::view_update");

    callbacks(false);

    _view->zoom(_view_widget->zoom());

    callbacks(true);
}

void Window::view_resize_update()
{
    //DJV_DEBUG("Window::view_resize_update");

    callbacks(false);

    switch (View_Prefs::global()->resize())
    {
        case View::RESIZE_NONE:
            break;

        case View::RESIZE_FIT:
            _view_widget->view_fit();
            break;

        case View::RESIZE_CENTER:
            _view_widget->view_center();
            break;

        default:
            break;
    }

    callbacks(true);
}

void Window::view_overlay_callback(bool)
{
    view_overlay_update();
}

void Window::view_overlay_update()
{
    //DJV_DEBUG("Window::view_overlay_update");

    callbacks(false);

    // Grid overlay.

    _view_widget->grid(_view->grid());
    _view_widget->grid_color(View_Prefs::global()->grid_color());

    // HUD overlay.

    Hud_Info hud;

    if (_image_p)
    {
        hud.info = _image_p->info();
        hud.tag =  _image_p->tag;
    }

    const Seq & seq = _playback->seq();

    hud.frame = 0;

    if (seq.list.size() &&
        _playback->frame() < static_cast<int64_t>(seq.list.size()))
    {
        hud.frame = seq.list[_playback->frame()];
    }

    hud.speed = _playback->speed();
    hud.speed_real = _playback->speed_real();
    hud.dropped_frames = _playback->dropped_frames();
    _view_widget->hud(_view->hud());
    _view_widget->hud_info(hud);
    _view_widget->hud_show(View_Prefs::global()->hud_show());
    _view_widget->hud_color(View_Prefs::global()->hud_color());
    _view_widget->hud_background(View_Prefs::global()->hud_background());
    _view_widget->hud_background_color(
        View_Prefs::global()->hud_background_color());

    // Update.

    _view_widget->redraw();

    callbacks(true);
}

void Window::view_pick_callback(const V2i & in)
{
    //DJV_DEBUG("Window::view_pick_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    _image_pick = in - _view_widget->view();

    image_pick_update();
}

Gl_Image_Options Window::image_options() const
{
    // Image processing options.

    Gl_Image_Options out;

    out.xform.mirror = _image->mirror();

    if (_image_p)
    {
        out.xform.scale = Image::scale(_image->scale(), _image_p->size());
    }

    out.xform.rotate = Image::rotate(_image->rotate());

    if (_image->color_profile() && _image_p)
    {
        out.color_profile = _image_p->color_profile;
    }

    out.display_profile = _image->display_profile();
    out.channel = _image->channel();
    out.background = View_Prefs::global()->background();

    return out;
}

V2i Window::image_pick() const
{
    return Vector_Util::floor<double, int>(
        V2f(_image_pick) / _view_widget->zoom());
}

void Window::image_callback(bool)
{
    image_update();
}

namespace
{

const String
    label_toolbar_info_image = "Image: %%x%%:%% %%";

} // namespace

void Window::image_update()
{
    //DJV_DEBUG("Window::image_update");

    // Update image.

    _image_p = _file->get(_playback->frame());

    if (_image_p)
    {
        //DJV_DEBUG_PRINT("image = " << *_image_p);
    }

    //! \todo Is this still necessary?

    callbacks(false);

    // Update infobar.

    Pixel_Data_Info info;

    if (_image_p)
    {
        info = _image_p->info();
    }

    _info_image_widget->set(String_Format(label_toolbar_info_image).
        arg(info.size.x).
        arg(info.size.y).
        arg(Vector_Util::aspect(info.size), 0, -2).
        arg(String_Util::label(info.pixel)));

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
    //DJV_DEBUG("Window::image_redraw_update");
    //DJV_DEBUG_PRINT("view = " << _view_widget->view());
    //DJV_DEBUG_PRINT("zoom = " << _view_widget->zoom());

    callbacks(false);

    const Gl_Image_Options prev_option = _view_widget->options();

    // Set image.

    _view_widget->set(_image->frame_store() ? &_image_tmp : _image_p);

    // Set image options.

    const Gl_Image_Options & options = image_options();

    //DJV_DEBUG_PRINT("color = " << options.display_profile.color);

    _view_widget->options(options);
    _view_widget->redraw();

    callbacks(true);
}

void Window::image_resize_callback(bool)
{
    window_resize_update();
}

void Window::image_frame_store_callback(bool)
{
    //DJV_DEBUG("Window::image_frame_store_callback");

    if (_image_p)
    {
        _image_tmp = *_image_p;
    }
}

void Window::image_display_profile_callback(bool)
{
    // Connect image display profile dialog.

    Display_Profile_Dialog::global()->show();
    Display_Profile_Dialog::global()->set(
        _view_widget,
        _image->display_profile());
    Display_Profile_Dialog::global()->signal.del();
    Display_Profile_Dialog::global()->signal.set(
        this,
        image_display_profile_value_callback);
}

void Window::image_display_profile_value_callback(const Display_Profile & in)
{
    //DJV_DEBUG("Window::image_display_profile_value_callback");

    // Set image display profile.

    _image->display_profile(in);

    // Update.

    image_redraw_update();
    image_dialog_update();
    tool_dialog_update();
}

void Window::image_dialog_update()
{
    //DJV_DEBUG("Window::image_dialog_update");

    callbacks(false);

    Display_Profile_Dialog::global()->update(
        _view_widget,
        _image->display_profile());

    callbacks(true);
}

void Window::image_focus_update()
{
    //DJV_DEBUG("Window::image_focus_update");

    callbacks(false);

    Display_Profile_Dialog::global()->pick(
        _view_widget,
        _image->display_profile());
    Display_Profile_Dialog::global()->signal.del();
    Display_Profile_Dialog::global()->signal.set(
        this,
        image_display_profile_value_callback);

    Histogram_Dialog::global()->pick(_view_widget);
    Info_Dialog::global()->pick(_view_widget, _file->info());

    callbacks(true);
}

namespace
{

const String
    label_toolbar_info_pixel = "Pixel: %%, %%, %%";

} // namespace

void Window::image_pick_update()
{
    //DJV_DEBUG("Window::image_pick_update");

    callbacks(false);

    // Update infobar with pixel information.

    const V2i pick = image_pick();
    Gl_Image_Options options = image_options();

    _image_sample = options.background;

    if (_image_p && _window_toolbar[TOOLBAR_INFO])
    {
        //DJV_DEBUG_PRINT("sample");

        try
        {
            Pixel_Data tmp(Pixel_Data_Info(1, _image_p->pixel()));

            if (! _sample_buffer)
            {
                _sample_buffer = new Gl_Offscreen_Buffer(tmp.info());
            }

            Gl_Image_Options _options = options;
            _options.xform.position -= pick;

            Gl_Image::copy(
                *_image_p,
                tmp,
                _options,
                &_gl_state,
                _sample_buffer);

            Gl_Image::average(tmp, &_image_sample);
        }
        catch (Error in)
        {
            DJV_APP->error(in);
        }
    }

    //DJV_DEBUG_PRINT("sample = " << _image_sample);

    _info_swatch->set(_image_sample);

    _info_pixel_widget->set(String_Format(label_toolbar_info_pixel).
        arg(pick.x).
        arg(pick.y).
        arg(String_Util::label(_image_sample)));

    // Update dialogs.

    Magnify_Dialog::global()->pick(_view_widget, _image_pick);
    Color_Picker::global()->pick(_view_widget, _image_pick);

    callbacks(true);
}

void Window::playback(Playback::PLAYBACK in)
{
    _playback->playback(in);
}

void Window::playback_frame(int64_t in)
{
    _playback->frame(in);
}

void Window::playback_speed(const Speed & in)
{
    _playback->speed(in);
}

void Window::tool_magnify_callback(bool)
{
    Magnify_Dialog::global()->show();
    Magnify_Dialog::global()->set(_view_widget, _image_pick);
}

void Window::tool_color_picker_callback(bool)
{
    Color_Picker::global()->show();
    Color_Picker::global()->set(_view_widget, _image_pick);
}

void Window::tool_histogram_callback(bool)
{
    Histogram_Dialog::global()->show();
    Histogram_Dialog::global()->set(_view_widget);
}

void Window::tool_info_callback(bool)
{
    Info_Dialog::global()->show();
    Info_Dialog::global()->set(_view_widget, _file->info());
}

void Window::tool_dialog_update()
{
    //DJV_DEBUG("Window::tool_dialog_update");

    callbacks(false);

    Magnify_Dialog::global()->update(_view_widget);

    Color_Picker::global()->update(_view_widget);

    Histogram_Dialog::global()->update(_view_widget);

    Info_Dialog::global()->update(_view_widget, _file->info());

    callbacks(true);
}

void Window::mouse_wheel_callback(Input::MOUSE_WHEEL in)
{
    _mouse_wheel = in;

    switch (_mouse_wheel)
    {
        case Input::MOUSE_WHEEL_PLAYBACK_SHUTTLE:
            _playback->playback(Playback::STOP);
            _playback_frame_tmp = _playback->frame();
            break;

        case Input::MOUSE_WHEEL_PLAYBACK_SPEED:
            _playback_speed_tmp =
                Speed::speed_to_float(_playback->speed());
            break;

        default:
            break;
    }
}

void Window::mouse_wheel_value_callback(int in)
{
    switch (_mouse_wheel)
    {
        case Input::MOUSE_WHEEL_PLAYBACK_SHUTTLE:
            _playback->frame(_playback_frame_tmp + in);
            break;

        case Input::MOUSE_WHEEL_PLAYBACK_SPEED:
            _playback->speed(
                Speed::float_to_speed(
                    _playback_speed_tmp + static_cast<double>(in)));
            break;

        default:
            break;
    }
}

void Window::cache_callback(const List<int64_t> & in)
{
    //DJV_DEBUG("Window::cache_callback");

    _playback->cached_frames(
        File_Prefs::global()->cache_display() ? in : List<int64_t>());

    cache_update();
}

namespace
{

const String
    label_toolbar_info_cache = "Cache: %% / (%% / %%) MB";

} // namespace

void Window::cache_update()
{
    //DJV_DEBUG("Window::cache_update");

    callbacks(false);

    _info_cache_widget->set(String_Format(label_toolbar_info_cache).
        arg(Cache::global()->size(_file)).
        arg(Cache::global()->size()).
        arg(Cache::global()->max()));

    callbacks(true);
}

void Window::group_callback(bool)
{
    group_update();
}

void Window::group_update()
{
    //DJV_DEBUG("Window::group_update");

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

    Menu_Item_Group group;
    _file->menu_update(&group, _playback->seq().list.size() > 1);
    menu_update(&group);
    _view->menu_update(&group);
    _image->menu_update(&group);
    _playback->menu_update(&group);
    _tool->menu_update(&group);
    _help->menu_update(&group);
    _menu->set(group);

    callbacks(true);
}

//------------------------------------------------------------------------------
// Window_Factory
//------------------------------------------------------------------------------

Window_Factory::~Window_Factory()
{
    //DJV_DEBUG("Window_Factory::~Window_Factory");

    for (size_t i = 0; i < _list.size(); ++i)
    {
        _list[i]->del();
    }
}

Window * Window_Factory::add(const Window * copy)
{
    //DJV_DEBUG("Window_Factory::add");
    //DJV_DEBUG_PRINT("copy = " << static_cast<int>(copy));

    Window * window = new Window(copy);
    _list += window;
    return window;
}

void Window_Factory::del(Window * in)
{
    //DJV_DEBUG("Window_Factory::del");
    //DJV_DEBUG_PRINT("in = " << static_cast<int>(in));

    in->del();

    _list.erase(std::find(_list.begin(), _list.end(), in));

    // Exit application if no windows are left.

    if (! _list.size())
    {
        DJV_APP->exit(Application::EXIT_DEFAULT);
    }
}

const List<Window *> Window_Factory::list() const
{
    return _list;
}

Window_Factory * Window_Factory::global()
{
    static Window_Factory * global = 0;

    if (! global)
    {
        global = new Window_Factory;
    }

    return global;
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(Window::RESIZE_MAX, Window::label_resize_max())
_DJV_STRING_OPERATOR_LABEL(Window::TOOLBAR, Window::label_toolbar())

} // namespace

