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

//! \file djv_file_browser.cpp

#include <djv_file_browser_private.h>

#include <djv_cursor.h>
#include <djv_font.h>
#include <djv_frame.h>
#include <djv_frame_group.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_menu.h>
#include <djv_multi_choice_dialog.h>
#include <djv_prefs.h>
#include <djv_push_button.h>
#include <djv_question_dialog.h>
#include <djv_shortcut.h>

#include <djv_directory.h>

#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// File_Browser
//------------------------------------------------------------------------------

namespace
{

const String
    label_seq = "Sequence:",
    label_type_all = "All",
    label_type_all_image = "All images",
    label_ok = "O&k",
    label_close = "Clos&e",
    label_pin = "Pin",
    label_bookmark_del = "Delete bookmark:",
    label_bookmark_del_all = "Delete all bookmarks?",
    tooltip_pin = "Pin the dialog.",
    tooltip_value = "File name",
    tooltip_up = "Go up a directory\n\nShortcut: %%",
    tooltip_prev = "Go to the previous directory\n\nShortcut: %%",
    tooltip_reload = "Reload the current directory\n\nShortcut: %%";

List<int> columns_sizes()
{
    return List<int>() <<
        0 <<
        (Font::string_width(4)) <<
        (Font::string_width("000.0000") + Style::global()->margin() * 2) <<
#       if ! defined(DJV_WINDOWS)
        (Font::string_width("000000") + Style::global()->margin() * 2) <<
#       endif
        (Font::string_width("000 000 00 00:00:00 0000") + 16 +
            Style::global()->margin() * 2);
}

} // namespace

File_Browser::File_Browser(File_Browser_Prefs_Instance * prefs) :
    Dialog("File Browser"),
    signal         (this),
    _prefs(
        prefs ?
        prefs :
        new File_Browser_Prefs_Instance(Prefs::prefs_global(), "file_browser")),
    _image         (_prefs->image()),
    _seq           (_prefs->seq()),
    _hidden        (_prefs->hidden()),
    _sort          (_prefs->sort()),
    _sort_reverse  (_prefs->sort_reverse()),
    _sort_directory(_prefs->sort_directory()),
    _type          (_prefs->type()),
    _pin           (_prefs->pin()),
    _menu          (0),
    _value_widget  (0),
    _up_widget     (0),
    _prev_widget   (0),
    _reload_widget (0),
    _seq_widget    (0),
    _browser       (0),
    _columns       (0),
    _pin_widget    (0),
    _browser_frame (0)
{
    debug_path("File_Browser");

    //DJV_DEBUG("File_Browser::File_Browser");

    // Create widgets.

    _menu = new Menu;

    _value_widget = new Text_Edit;

    _up_widget = new Tool_Button("directory_up");
    _prev_widget = new Tool_Button("directory_prev");
    _reload_widget = new Tool_Button("directory_reload");
    _seq_widget = new Choice_Widget(Seq::label_compress());
    Label * seq_label = new Label(label_seq);

    _browser = new File_Browser_Browser;
    _columns = new Columns;
    _columns->names(label_sort());

    _pin_widget = new Check_Button(label_pin);
    _pin_widget->tooltip(tooltip_pin);
    Push_Button * ok_button = new Push_Button(label_ok);
    Push_Button * close_button = new Push_Button(label_close);

    _browser_frame = new Frame;
    _browser_frame->style(Frame::STYLE_BORDER);

    // Layout.

    Horizontal_Layout * layout_h  = 0;
    Horizontal_Layout * layout_h2 = 0;
    Vertical_Layout *   layout_v  = 0;

    Vertical_Layout * layout_window = new Vertical_Layout(this);
    layout_window->spacing(0);
    layout_window->margin(0);
    layout_window->add(_menu);
    Vertical_Layout * layout = new Vertical_Layout(layout_window);
    layout_window->stretch(layout);

    layout_h = new Horizontal_Layout(layout);
    layout_h->spacing(Style::global()->spacing_large());
    layout_h->margin(0);
    layout_h->add(new Horizontal_Frame_Group(List<Frame *>() <<
        _up_widget <<
        _prev_widget <<
        _reload_widget));
    layout_h2 = new Horizontal_Layout(layout_h);
    layout_h2->margin(0);
    layout_h2->add(seq_label);
    layout_h2->add(_seq_widget);

    layout->add(_browser_frame);
    layout->stretch(_browser_frame);
    layout_v = new Vertical_Layout(_browser_frame);
    layout_v->spacing(0);
    layout_v->margin(0);
    layout_v->add(_columns);
    layout_v->add(_browser);
    layout_v->stretch(_browser);

    layout->add(_value_widget);

    layout_h = new Horizontal_Layout(layout);
    layout_h->spacing(Style::global()->spacing_large());
    layout_h->margin(0);
    layout_h->add(_pin_widget);
    layout_h->add_spacer(-1, true);
    layout_h2 = new Horizontal_Layout(layout_h);
    layout_h2->margin(0);
    layout_h2->add(ok_button);
    layout_h2->add(close_button);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    _value = File_Util::path_absolute(".");
    _directory_prev = _value.path();

    //directory_update(); //! \todo Can take a long time.
    widget_update();
    type_update();
    menu_update();
    _pin_widget->set(_pin);

    size(V2i(600, 600));

    fl_font(Style::global()->font(), Style::global()->font_size());
    _columns->sizes(columns_sizes());
    _browser->columns(_columns->sizes());

    // Callbacks.

    _value_widget->signal.set(this, value_callback);
    _value_widget->action_signal.set(this, value_action_callback);
    _value_widget->change_signal.set(this, value_change_callback);
    _columns->signal.set(this, columns_callback);
    _columns->action_signal.set(this, columns_action_callback);
    _browser->signal.set(this, browser_callback);
    _browser->mouse_signal.set(this, browser_mouse_callback);
    _browser->action_signal.set(this, browser_action_callback);

    _up_widget->signal.set(this, up_callback);
    _prev_widget->signal.set(this, prev_callback);
    _reload_widget->signal.set(this, reload_callback);
    _seq_widget->signal.set(this, seq_callback);

    _pin_widget->signal.set(this, pin_callback);
    ok_button->signal.set(this, ok_callback);
    close_button->signal.set(this, close_callback);
}

File_Browser::~File_Browser()
{
    //DJV_DEBUG("File_Browser::~File_Browser");
}

void File_Browser::del()
{
    //DJV_DEBUG("File_Browser::del");

    _prefs.reset();

    Dialog::del();
}

void File_Browser::set(const File & in)
{
    if (in == _value)
    {
        return;
    }

    //DJV_DEBUG("File_Browser::set");
    //DJV_DEBUG_PRINT("in = " << in);

    const String directory = _value.path();

    _value = in;

    if (_seq && _value.is_seq_valid())
    {
        _value.type(File::SEQ);
    }

    if (_value.path() != directory)
    {
        _directory_prev = directory;

        directory_update();
    }

    widget_update();
}

const File & File_Browser::get() const
{
    return _value;
}

const List<String> & File_Browser::label_image()
{
    static const List<String> data = List<String>() <<
        "Off" <<
        "Low Quality" <<
        "High Quality";

    DJV_ASSERT(data.size() == _IMAGE_SIZE);

    return data;
}

void File_Browser::image(IMAGE in)
{
    if (in == _image)
    {
        return;
    }

    _image = in;

    _prefs->image(_image);

    directory_update();
    menu_update();
}

File_Browser::IMAGE File_Browser::image() const
{
    return _image;
}

void File_Browser::seq(Seq::COMPRESS in)
{
    if (in == _seq)
    {
        return;
    }

    _seq = in;

    _prefs->seq(_seq);

    directory_update();
    widget_update();
    menu_update();
}

Seq::COMPRESS File_Browser::seq() const
{
    return _seq;
}

void File_Browser::type(int in)
{
    if (in == _type)
    {
        return;
    }

    _type = in;

    _prefs->type(_type);

    directory_update();
    type_update();
    menu_update();
}

int File_Browser::type() const
{
    return _type;
}

void File_Browser::hidden(bool in)
{
    if (in == _hidden)
    {
        return;
    }

    _hidden = in;

    _prefs->hidden(_hidden);

    directory_update();
    menu_update();
}

bool File_Browser::hidden() const
{
    return _hidden;
}

const List<String> & File_Browser::label_sort()
{
    static const List<String> data = List<String>() <<
        File_Util::label_sort()[File_Util::SORT_NAME] <<
        File_Util::label_sort()[File_Util::SORT_TYPE] <<
        File_Util::label_sort()[File_Util::SORT_SIZE] <<
#       if ! defined(DJV_WINDOWS)
        File_Util::label_sort()[File_Util::SORT_USER] <<
#       endif
        File_Util::label_sort()[File_Util::SORT_TIME];

    DJV_ASSERT(data.size() == _SORT_SIZE);

    return data;
}

void File_Browser::sort(SORT in)
{
    if (in == _sort)
    {
        return;
    }

    _sort = in;

    _prefs->sort(_sort);

    directory_update();
    menu_update();
}

File_Browser::SORT File_Browser::sort() const
{
    return _sort;
}

void File_Browser::sort_reverse(bool in)
{
    if (in == _sort_reverse)
    {
        return;
    }

    _sort_reverse = in;

    _prefs->sort_reverse(_sort_reverse);

    directory_update();
    menu_update();
}

bool File_Browser::sort_reverse() const
{
    return _sort_reverse;
}

void File_Browser::sort_directory(bool in)
{
    if (in == _sort_directory)
    {
        return;
    }

    _sort_directory = in;

    _prefs->sort_directory(_sort_directory);

    directory_update();
    menu_update();
}

bool File_Browser::sort_directory() const
{
    return _sort_directory;
}

const List<String> & File_Browser::label_navigate()
{
    static const List<String> data = List<String>() <<
        "Single-Click" <<
        "Double-Click";

    DJV_ASSERT(data.size() == _NAVIGATE_SIZE);

    return data;
}

void File_Browser::pin(bool in)
{
    if (in == _pin)
    {
        return;
    }

    _pin = in;

    _prefs->pin(_pin);

    _pin_widget->set(_pin);
}

bool File_Browser::pin() const
{
    return _pin;
}

void File_Browser::show()
{
    //DJV_DEBUG("File_Browser::show");

    if (! shown())
    {
        //_value = File_Util::path_absolute(".");

        directory_update();
        widget_update();

        _value_widget->take_focus();
    }

    Dialog::show();
}

void File_Browser::dirty()
{
    //DJV_DEBUG("File_Browser::dirty");

    Dialog::dirty();

    // Update tooltips.

    const List<Shortcut> & shortcut = File_Browser_Prefs::global()->shortcut();

    _value_widget->tooltip(tooltip_value);

    _up_widget->tooltip(String_Format(tooltip_up).
        arg(Shortcut::label(shortcut[File_Browser_Prefs::UP].value)));
    _prev_widget->tooltip(String_Format(tooltip_prev).
        arg(Shortcut::label(shortcut[File_Browser_Prefs::PREV].value)));
    _reload_widget->tooltip(String_Format(tooltip_reload).
        arg(Shortcut::label(shortcut[File_Browser_Prefs::RELOAD].value)));

    // General updates.

    if (visible() && shown())
    {
        directory_update();
    }

    menu_update();
}

void File_Browser::set(
    Callback *                           in,
    Signal<const File &>::Callback_Fnc * callback)
{
    File_Browser * dialog = global();
    
    dialog->signal.del();
    dialog->signal.set(in, callback);
    dialog->show();
}

void File_Browser::del(Callback * in)
{
    File_Browser * dialog = global();
    
    dialog->signal.del(in);
}

File_Browser * File_Browser::global()
{
    static File_Browser * global = 0;

    if (! global)
    {
        global = new File_Browser;
    }

    return global;
}

void File_Browser::value_callback(const String & in)
{
    //DJV_DEBUG("File_Browser::value_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    set(File(in).type() == File::DIRECTORY ? File_Util::path_fix(in) : in);
}

void File_Browser::value_action_callback(const String & in)
{
    value_callback(in);

    if (_value.type() != File::DIRECTORY)
    {
        ok_callback(true);
    }
}

void File_Browser::value_change_callback(const String & in)
{
    File tmp(in, false);

    if (tmp.stat() && tmp.type() == File::DIRECTORY)
    {
        set(tmp);
    }
}

void File_Browser::browser_callback(const File & in)
{
    //DJV_DEBUG("File_Browser::browser_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    set(in);
}

void File_Browser::browser_mouse_callback(const File & in)
{
    //DJV_DEBUG("File_Browser::browser_mouse_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    set((NAVIGATE_SINGLE_CLICK == File_Browser_Prefs::global()->navigate() &&
            in.type() == File::DIRECTORY) ?
        File(File_Util::path_fix(in)) :
        in);
}

void File_Browser::browser_action_callback(const File & in)
{
    //DJV_DEBUG("File_Browser::browser_action_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    set(in.type() == File::DIRECTORY ? File(File_Util::path_fix(in)) : in);

    if (_value.type() != File::DIRECTORY)
    {
        ok_callback(true);
    }
}

void File_Browser::columns_callback(const List<int> & in)
{
    _browser->columns(in);
}

void File_Browser::columns_action_callback(int in)
{
    if (in == _sort)
    {
        sort_reverse(! _sort_reverse);
    }
    else
    {
        sort(static_cast<SORT>(in));
    }
}

void File_Browser::up_callback(bool)
{
    Directory tmp(_value);
    tmp.up();
    set(tmp.get());
}

void File_Browser::_up_callback()
{
    up_callback(true);
}

void File_Browser::prev_callback(bool)
{
    set(_directory_prev);
}

void File_Browser::_prev_callback()
{
    prev_callback(true);
}

void File_Browser::_recent_callback()
{
    const List<String> & recent = File_Browser_Prefs::global()->recent();

    const int i = List_Util::find(_menu->item(), _menu_recent);

    if (i >= 0 && i < static_cast<int>(recent.size()))
    {
        set(recent[i]);
    }
}

void File_Browser::current_callback(bool)
{
    set(Directory(".").get());
}

void File_Browser::_current_callback()
{
    current_callback(true);
}

#if ! defined(DJV_WINDOWS)

void File_Browser::home_callback(bool)
{
    set(Directory::home());
}

void File_Browser::_home_callback()
{
    home_callback(true);
}

#endif // ! DJV_WINDOWS

void File_Browser::desktop_callback(bool)
{
    set(Directory::desktop());
}

void File_Browser::_desktop_callback()
{
    desktop_callback(true);
}

#if defined(DJV_WINDOWS)

void File_Browser::drive_callback(bool)
{
    const List<String> drives = djv_base::directory::drives();

    const int i = List_Util::find(_menu->item(), _menu_drive);

    if (i >= 0 && i < static_cast<int>(drives.size()))
    {
        set(drives[i]);
    }
}

void File_Browser::_drive_callback()
{
    drive_callback(true);
}

#else // DJV_WINDOWS

void File_Browser::root_callback(bool)
{
    set(Directory::root());
}

void File_Browser::_root_callback()
{
    root_callback(true);
}

#endif // DJV_WINDOWS

void File_Browser::reload_callback(bool)
{
    directory_update();
}

void File_Browser::_reload_callback()
{
    reload_callback(true);
}

void File_Browser::_image_callback()
{
    image(static_cast<IMAGE>(List_Util::find(_menu->item(), _menu_image)));
}

void File_Browser::seq_callback(int in)
{
    seq(static_cast<Seq::COMPRESS>(in));
}

void File_Browser::_seq_callback()
{
    seq_callback(List_Util::find(_menu->item(), _menu_seq));
}

void File_Browser::_type_callback()
{
    type(List_Util::find(_menu->item(), _menu_type) - 1);
}

void File_Browser::_hidden_callback()
{
    hidden(! _hidden);
}

void File_Browser::_sort_callback()
{
    sort(static_cast<SORT>(List_Util::find(_menu->item(), _menu_sort)));
}

void File_Browser::_sort_reverse_callback()
{
    sort_reverse(! _sort_reverse);
}

void File_Browser::_sort_directory_callback()
{
    sort_directory(! _sort_directory);
}

void File_Browser::_navigate_callback()
{
    File_Browser_Prefs::global()->navigate(
        static_cast<NAVIGATE>(List_Util::find(_menu->item(), _menu_navigate)));
}

void File_Browser::_bookmark_callback()
{
    const List<String> & bookmark = File_Browser_Prefs::global()->bookmark();

    const int i = List_Util::find(_menu->item(), _menu_bookmark);

    if (i >= 0 && i < static_cast<int>(bookmark.size()))
    {
        set(bookmark[i]);
    }
}

void File_Browser::_bookmark_add_callback()
{
    File_Browser_Prefs::global()->bookmark_add(_value.path());

    menu_update();
}

void File_Browser::bookmark_del_callback(const List<int> & in)
{
    const List<String> & bookmark = File_Browser_Prefs::global()->bookmark();

    List<String> tmp;

    for (size_t i = 0; i < bookmark.size(); ++i)
    {
        size_t j = 0;

        for (; j < in.size(); ++j)
            if (in[j] == static_cast<int>(i))
            {
                break;
            }

        if (j < in.size())
        {
            continue;
        }

        tmp += bookmark[i];
    }

    File_Browser_Prefs::global()->bookmark(tmp);

    menu_update();
}

void File_Browser::_bookmark_del_callback()
{
    const List<String> & bookmark = File_Browser_Prefs::global()->bookmark();

    if (! bookmark.size())
    {
        return;
    }

    Multi_Choice_Dialog::dialog(
        label_bookmark_del,
        bookmark,
        this,
        bookmark_del_callback);
}

void File_Browser::bookmark_del_all_callback(bool)
{
    File_Browser_Prefs::global()->bookmark(List<String>());

    menu_update();
}

void File_Browser::_bookmark_del_all_callback()
{
    if (! File_Browser_Prefs::global()->bookmark().size())
    {
        return;
    }

    Question_Dialog::dialog(
        label_bookmark_del_all,
        this,
        bookmark_del_all_callback);
}

void File_Browser::pin_callback(bool)
{
    pin(_pin_widget->get());
}

void File_Browser::ok_callback(bool)
{
    //DJV_DEBUG("File_Browser::ok_callback");
    //DJV_DEBUG_PRINT("value = " << _value);
    //DJV_DEBUG_PRINT("value type = " << _value.type());
    //DJV_DEBUG_PRINT("value seq = " << _value.seq());

    File_Browser_Prefs::global()->recent_add(_value.path());

    if (! _pin)
    {
        hide();
    }

    menu_update();

    signal.emit(_value);
}

void File_Browser::close_callback(bool)
{
    hide();
}

void File_Browser::directory_update()
{
    //DJV_DEBUG("File_Browser::directory_update");
    //DJV_DEBUG_PRINT("value = " << _value);

    callbacks(false);

    // Set busy cursor.

    Cursor cursor(FL_CURSOR_WAIT);

    // Get directory contents.

    const Directory directory(_value.path());

    List<File> list = directory.list();

    // File sequence directory contents.

    File_Util::seq_compress(list, _seq);

    // Filter directory contents.

    if (! _hidden || _type != -1)
    {
        const File_Util::FILTER filter =
            ! _hidden ? File_Util::FILTER_HIDDEN : File_Util::FILTER_NONE;

        List<String> glob;

        if (_type != -1 &&
            _type < static_cast<int>(_type_glob.size()))
        {
            const List<String> tmp = String_Util::split(_type_glob[_type], ',');

            for (size_t i = 0; i < tmp.size(); ++i)
            {
                glob += tmp[i];
            }
        }

        //DJV_DEBUG_PRINT("filter = " << filter);
        //DJV_DEBUG_PRINT("glob = " << glob);

        File_Util::filter(list, filter, glob, true);
    }

    // Sort directory contents.

    File_Util::SORT sort = File_Util::SORT(0);

    switch (_sort)
    {
        case SORT_NAME:
            sort = File_Util::SORT_NAME;
            break;

        case SORT_TYPE:
            sort = File_Util::SORT_TYPE;
            break;

        case SORT_SIZE:
            sort = File_Util::SORT_SIZE;
            break;
#   if ! defined(DJV_WINDOWS)

        case SORT_USER:
            sort = File_Util::SORT_USER;
            break;
#   endif

        case SORT_TIME:
            sort = File_Util::SORT_TIME;
            break;

        default:
            break;
    }

    File_Util::sort(list, sort, _sort_reverse);

    if (_sort_directory)
    {
        File_Util::sort_directories_first(list);
    }

    // Add parent directory.

    if (directory.is_valid())
    {
        list.push_front(File(_value.path() + ".."));
    }

    // Update browser widgets.

    _columns->sort(_sort);
    _columns->sort_reverse(_sort_reverse);
    _browser->set(list, _image);

    callbacks(true);
}

void File_Browser::widget_update()
{
    //DJV_DEBUG("File_Browser::widget_update");
    //DJV_DEBUG_PRINT("value = " << _value);
    //DJV_DEBUG_PRINT("value seq = " << _value.seq());

    callbacks(false);

    _value_widget->set(_value);

    _seq_widget->set(_seq);

    callbacks(true);
}

namespace
{

String _glob(const List<String> & in)
{
    List<String> tmp;

    for (size_t i = 0; i < in.size(); ++i)
    {
        tmp += String_Format("*%%").arg(in[i]);
    }

    return String_Util::join(tmp, ',');
}

List<String> glob(const List<Plugin *> & plugins)
{
    List<String> out;

    List<String> all;

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Load * plugin = dynamic_cast<Image_Load *>(plugins[i]))
        {
            all += plugin->extensions();
        }
    }

    out += _glob(all);

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Load * plugin = dynamic_cast<Image_Load *>(plugins[i]))
        {
            out += _glob(plugin->extensions());
        }
    }

    return out;
}

List<String> glob_label(const List<Plugin *> & plugins)
{
    List<String> out;

    out += label_type_all;
    out += label_type_all_image;

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Load * plugin = dynamic_cast<Image_Load *>(plugins[i]))
        {
            out += String_Format("%% (%%)").
                   arg(plugin->name()).
                   arg(String_Util::join(plugin->extensions(), ", "));
        }
    }

    return out;
}

} // namespace

void File_Browser::type_update()
{
    //DJV_DEBUG("File_Browser::type_update");

    callbacks(false);

    //! \todo What about image save plugins?

    const List<Plugin *> & plugins =
        Image_Load_Factory::global()->plugins();

    _type_glob  = glob      (plugins);
    _type_label = glob_label(plugins);

    callbacks(true);
}

namespace
{

const String
    menu_directory = "Directory",
    menu_directory_up = "&Up",
    menu_directory_prev = "&Previous",
    menu_directory_recent = "&Recent",
    menu_directory_current = "&Current",
    menu_directory_home = "&Home",
    menu_directory_desktop = "&Desktop",
    menu_directory_drive = "Dri&ves",
    menu_directory_root = "&Root",
    menu_directory_reload = "Re&load",
    menu_option = "Options",
    menu_option_image = "Thu&mbnails",
    menu_option_seq = "Se&quence",
    menu_option_type = "&Type",
    menu_option_hidden = "&Hidden",
    menu_option_sort = "&Sort",
    menu_option_sort_reverse = "&Reverse Sort",
    menu_option_sort_directory = "&Directories First",
    menu_option_navigate = "Na&vigate",
    menu_bookmark = "Bookmarks",
    menu_bookmark_add = "&Add",
    menu_bookmark_del = "&Delete",
    menu_bookmark_del_all = "D&elete All";

} // namespace

void File_Browser::menu_update()
{
    //DJV_DEBUG("File_Browser::menu_update");

    Menu_Item_Group group;

    const List<Shortcut> & shortcut = File_Browser_Prefs::global()->shortcut();

    // Menu:
    //
    // Directory
    // * Up
    // * Previous
    // * Recent
    //   * ...
    //   ---
    // * Current
    // * Home
    // * Desktop
    // * Root
    //   ---
    // * Reload
    //
    // Options
    // * Image Thumbnails
    //   * ...
    // * File Sequencing
    //   * ...
    //   ---
    // * File Types
    //   * ...
    // * Hidden Files
    //   ---
    // * Sort By
    //   * ...
    // * Reverse Sort
    // * Sort Directories First
    //   ---
    // * Navigate
    //   * ...
    //
    // Bookmarks
    // * Add
    // * Delete
    // * Delete All
    //   ---
    // * ...

    group.add(menu_directory, 0, 0, 0, Menu_Item::SUB_MENU);

    group.add(
        menu_directory_up,
        shortcut[File_Browser_Prefs::UP].value,
        _up_callback,
        this);

    group.add(
        menu_directory_prev,
        shortcut[File_Browser_Prefs::PREV].value,
        _prev_callback,
        this);

    group.add(
        menu_directory_recent,
        0,
        0,
        0,
        Menu_Item::SUB_MENU | Menu_Item::DIVIDER);

    _menu_recent = group.add(
        File_Browser_Prefs::global()->recent(),
        List<int>(),
        _recent_callback,
        this);

    group.end();

    group.add(
        menu_directory_current,
        shortcut[File_Browser_Prefs::CURRENT].value,
        _current_callback,
        this);

# if ! defined(DJV_WINDOWS)

    group.add(
        menu_directory_home,
        shortcut[File_Browser_Prefs::HOME].value,
        _home_callback,
        this);

# endif // DJV_WINDOWS

    group.add(
        menu_directory_desktop,
        shortcut[File_Browser_Prefs::DESKTOP].value,
        _desktop_callback,
        this);

# if defined(DJV_WINDOWS)

    group.add(
        menu_directory_drive,
        0,
        0,
        0,
        Menu_Item::SUB_MENU | Menu_Item::DIVIDER);

    _menu_drive = group.add(
        directory::drives(),
        List<int>(),
        _drive_callback,
        this);

    group.end();

# else // DJV_WINDOWS

    group.add(
        menu_directory_root,
        shortcut[File_Browser_Prefs::ROOT].value,
        _root_callback,
        this,
        Menu_Item::DIVIDER);

# endif // DJV_WINDOWS

    group.add(
        menu_directory_reload,
        shortcut[File_Browser_Prefs::RELOAD].value,
        _reload_callback,
        this);

    group.end();

    group.add(menu_option, 0, 0, 0, Menu_Item::SUB_MENU);

    group.add(menu_option_image, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_image = group.add(
        label_image(),
        List<int>(),
        _image_callback,
        this,
        Menu_Item::RADIO,
        _image);
    
    group.end();

    group.add(menu_option_seq, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_seq = group.add(
        Seq::label_compress(),
        List<int>() <<
        shortcut[File_Browser_Prefs::SEQ_OFF].value <<
        shortcut[File_Browser_Prefs::SEQ_SPARSE].value <<
        shortcut[File_Browser_Prefs::SEQ_RANGE].value,
        _seq_callback,
        this,
        Menu_Item::RADIO,
        _seq);
    
    group.end();

    group.add(menu_option_type, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_type = group.add(
        _type_label,
        List<int>(),
        _type_callback,
        this,
        Menu_Item::RADIO,
        _type + 1);

    group.end();

    group.add(
        menu_option_hidden,
        shortcut[File_Browser_Prefs::HIDDEN].value,
        _hidden_callback,
        this,
        Menu_Item::TOGGLE | Menu_Item::DIVIDER,
        _hidden);

    group.add(menu_option_sort, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_sort = group.add(
        label_sort(),
        List<int>() <<
        shortcut[File_Browser_Prefs::SORT_NAME].value <<
        shortcut[File_Browser_Prefs::SORT_SIZE].value <<
#       if ! defined(DJV_WINDOWS)
        shortcut[File_Browser_Prefs::SORT_USER].value <<
#       endif
        shortcut[File_Browser_Prefs::SORT_TIME].value,
        _sort_callback,
        this,
        Menu_Item::RADIO,
        _sort);

    group.end();

    group.add(
        menu_option_sort_reverse,
        shortcut[File_Browser_Prefs::SORT_REVERSE].value,
        _sort_reverse_callback,
        this,
        Menu_Item::TOGGLE,
        _sort_reverse);

    group.add(
        menu_option_sort_directory,
        shortcut[File_Browser_Prefs::SORT_DIRECTORY].value,
        _sort_directory_callback,
        this,
        Menu_Item::TOGGLE | Menu_Item::DIVIDER,
        _sort_directory);

    group.add(menu_option_navigate, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_navigate = group.add(
        label_navigate(),
        List<int>(),
        _navigate_callback,
        this,
        Menu_Item::RADIO,
        File_Browser_Prefs::global()->navigate());
    
    group.end();

    group.end();

    group.add(menu_bookmark, 0, 0, 0, Menu_Item::SUB_MENU);

    group.add(
        menu_bookmark_add,
        shortcut[File_Browser_Prefs::BOOKMARK_ADD].value,
        _bookmark_add_callback,
        this);

    group.add(
        menu_bookmark_del,
        0,
        _bookmark_del_callback,
        this);

    group.add(
        menu_bookmark_del_all,
        0,
        _bookmark_del_all_callback,
        this,
        Menu_Item::DIVIDER);

    _menu_bookmark = group.add(
        File_Browser_Prefs::global()->bookmark(), List<int>() <<
        shortcut[File_Browser_Prefs::BOOKMARK_1].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_2].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_3].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_4].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_5].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_6].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_7].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_8].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_9].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_10].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_11].value <<
        shortcut[File_Browser_Prefs::BOOKMARK_12].value,
        _bookmark_callback,
        this);

    group.end();

    group.end();

    _menu->set(group);
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(File_Browser::IMAGE, File_Browser::label_image())
_DJV_STRING_OPERATOR_LABEL(File_Browser::SORT, File_Browser::label_sort())
_DJV_STRING_OPERATOR_LABEL
    (File_Browser::NAVIGATE,
    File_Browser::label_navigate())

} // djv

