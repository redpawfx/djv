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

//! \file djv_view_file_prefs.cpp

#include <djv_view_file_prefs.h>

#include <djv_application.h>
#include <djv_check_button.h>
#include <djv_choice_widget.h>
#include <djv_file_widget.h>
#include <djv_group_box.h>
#include <djv_int_edit.h>
#include <djv_label.h>
#include <djv_menu.h>
#include <djv_multiline_label.h>
#include <djv_prefs.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// File_Prefs
//------------------------------------------------------------------------------

File_Prefs::File_Prefs() :
    recent_signal(this),
    seq_auto_signal(this),
    proxy_signal(this),
    u8_conversion_signal(this),
    cache_signal(this),
    _seq_auto(true),
    _command_line_combine(false),
    _proxy(Pixel_Data_Info::PROXY(0)),
    _u8_conversion(false),
    _cache(false),
    _cache_size(Cache::default_size()[4]),
    _cache_type(Cache::CACHE_LRU_PLAYBACK),
    _cache_display(true)
{
    Prefs prefs(Prefs::prefs(), "file");

    Prefs::get_(&prefs, "recent", &_recent);
    Prefs::get_(&prefs, "seq_auto", &_seq_auto);
    Prefs::get_(&prefs, "command_line_combine", &_command_line_combine);
    Prefs::get_(&prefs, "proxy", &_proxy);
    Prefs::get_(&prefs, "u8_conversion", &_u8_conversion);
    Prefs::get_(&prefs, "cache", &_cache);
    Prefs::get_(&prefs, "cache_size", &_cache_size);
    Prefs::get_(&prefs, "cache_type", &_cache_type);
    Prefs::get_(&prefs, "cache_display", &_cache_display);

    Cache::global()->max(_cache_size);
    Cache::global()->type(_cache_type);
}

File_Prefs::~File_Prefs()
{
    Prefs prefs(Prefs::prefs(), "file");

    Prefs::set_(&prefs, "recent", _recent);
    Prefs::set_(&prefs, "seq_auto", _seq_auto);
    Prefs::set_(&prefs, "command_line_combine", _command_line_combine);
    Prefs::set_(&prefs, "proxy", _proxy);
    Prefs::set_(&prefs, "u8_conversion", _u8_conversion);
    Prefs::set_(&prefs, "cache", _cache);
    Prefs::set_(&prefs, "cache_size", _cache_size);
    Prefs::set_(&prefs, "cache_type", _cache_type);
    Prefs::set_(&prefs, "cache_display", _cache_display);
}

void File_Prefs::recent(const String & in)
{
    //DJV_DEBUG("File_Prefs::recent");
    //DJV_DEBUG_PRINT("in = " << in);

    File_Util::recent(in, _recent);

    recent_signal.emit(_recent);
}

const List<String> & File_Prefs::recent() const
{
    return _recent;
}

void File_Prefs::seq_auto(bool in)
{
    if (in == _seq_auto)
    {
        return;
    }

    _seq_auto = in;

    seq_auto_signal.emit(_seq_auto);
}

bool File_Prefs::seq_auto() const
{
    return _seq_auto;
}

void File_Prefs::command_line_combine(bool in)
{
    _command_line_combine = in;
}

bool File_Prefs::command_line_combine() const
{
    return _command_line_combine;
}

void File_Prefs::proxy(Pixel_Data_Info::PROXY in)
{
    if (in == _proxy)
    {
        return;
    }

    _proxy = in;

    proxy_signal.emit(_proxy);
}

Pixel_Data_Info::PROXY File_Prefs::proxy() const
{
    return _proxy;
}

void File_Prefs::u8_conversion(bool in)
{
    if (in == _u8_conversion)
    {
        return;
    }

    _u8_conversion = in;

    u8_conversion_signal.emit(_u8_conversion);
}

bool File_Prefs::u8_conversion() const
{
    return _u8_conversion;
}

void File_Prefs::cache(bool in)
{
    if (in == _cache)
    {
        return;
    }

    _cache = in;

    if (! _cache)
    {
        Cache::global()->del();
    }

    cache_signal.emit(_cache);
}

bool File_Prefs::cache() const
{
    return _cache;
}

void File_Prefs::cache_size(int in)
{
    if (in == _cache_size)
    {
        return;
    }

    //DJV_DEBUG("File_Prefs::cache_size");
    //DJV_DEBUG_PRINT("in = " << in);

    _cache_size = in;

    Cache::global()->max(_cache_size);
}

int File_Prefs::cache_size() const
{
    return _cache_size;
}

void File_Prefs::cache_type(Cache::CACHE in)
{
    if (in == _cache_type)
    {
        return;
    }

    //DJV_DEBUG("File_Prefs::cache_type");
    //DJV_DEBUG_PRINT("in = " << in);

    _cache_type = in;

    Cache::global()->type(_cache_type);
}

Cache::CACHE File_Prefs::cache_type() const
{
    return _cache_type;
}

void File_Prefs::cache_display(bool in)
{
    if (in == _cache_display)
    {
        return;
    }

    _cache_display = in;

    DJV_APP->window_update();
}

bool File_Prefs::cache_display() const
{
    return _cache_display;
}

//------------------------------------------------------------------------------
// Cache_Size_Widget
//------------------------------------------------------------------------------

namespace
{

class Cache_Size_Widget : public Widget
{
public:

    Cache_Size_Widget() :
        signal(this)
    {
        _widget = new Int_Edit;
        _widget->range(0, _widget->max());
        _widget->inc(100, 1000);
        Popup_Menu * menu = new Popup_Menu(Cache::label_size());

        size(FIXED, FIXED);
        Horizontal_Layout * layout = new Horizontal_Layout(this);
        layout->margin(0);
        layout->add(_widget);
        layout->add(menu);

        _widget->set(File_Prefs::global()->cache_size());

        _widget->signal.set(this, widget_callback);
        menu->signal.set(this, menu_callback);
    }

    void set(int in)
    {
        _widget->set(in);
    }

    Signal<int> signal;

private:

    DJV_CALLBACK(Cache_Size_Widget, widget_callback, int)
    {
        signal.emit(in);
    }
    DJV_CALLBACK(Cache_Size_Widget, menu_callback, int)
    {
        _widget->set(Cache::default_size()[in]);
    }

    Int_Edit * _widget;
};

} // namespace

//------------------------------------------------------------------------------
// File_Prefs_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_group = "General",
    label_seq_auto =
        "Automatically detect sequences when opening files",
    label_command_line_combine =
        "Combine command line arguments into a sequence",
    label_proxy_group = "Proxy Scale",
    label_proxy_text =
        "Proxy scaling reduces image resolution to allow more frames to be "
        "cached.",
    label_u8_conversion_group = "8-bit Conversion",
    label_u8_conversion_text =
        "Converting images to 8-bits can reduce their size allowing more "
        "frames to be cached.",
    label_u8_conversion = "Enable 8-bit conversion",
    label_cache_group = "Cache",
    label_cache = "Enable cache",
    label_cache_text =
        "The cache stores frames in memory. When the cache is disabled frames "
        "are streamed from disk.",
    label_cache_size = "Cache size (megabytes):",
    label_cache_type = "Cache type:",
    label_cache_display = "Display cached frames in timeline";

} // namespace

File_Prefs_Widget::File_Prefs_Widget()
{
    // Create options widgets.

    Group_Box * group = new Group_Box(label_group);

    Check_Button * seq_auto = new Check_Button(label_seq_auto);

    Check_Button * command_line_combine =
        new Check_Button(label_command_line_combine);

    // Create proxy scale widgets.

    Group_Box * proxy_group = new Group_Box(label_proxy_group);

    Multiline_Label * proxy_text = new Multiline_Label(label_proxy_text);

    Radio_Button_Group * proxy =
        new Radio_Button_Group(Pixel_Data_Info::label_proxy());

    // Create convert to 8-bits widgets.

    Group_Box * u8_conversion_group = new Group_Box(label_u8_conversion_group);

    Multiline_Label * u8_conversion_text =
        new Multiline_Label(label_u8_conversion_text);

    Check_Button * u8_conversion_widget = new Check_Button(label_u8_conversion);

    // Create memory cache widgets.

    Group_Box * cache_group = new Group_Box(label_cache_group);

    Multiline_Label * cache_text = new Multiline_Label(label_cache_text);

    Check_Button * cache = new Check_Button(label_cache);

    Cache_Size_Widget * cache_size = new Cache_Size_Widget;

    Label * cache_size_label = new Label(label_cache_size);

    Radio_Button_Group * cache_type =
        new Radio_Button_Group(label_cache_type, Cache::label_cache());

    Check_Button * cache_display = new Check_Button(label_cache_display);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(group);
    group->layout()->add(seq_auto);
    group->layout()->add(command_line_combine);

    layout->add(proxy_group);
    proxy_group->layout()->add(proxy_text);
    proxy_group->layout()->add(proxy);

    layout->add(u8_conversion_group);
    u8_conversion_group->layout()->add(u8_conversion_text);
    u8_conversion_group->layout()->add(u8_conversion_widget);

    layout->add(cache_group);
    cache_group->layout()->add(cache_text);
    cache_group->layout()->add(cache);
    Horizontal_Layout * layout_h = new Horizontal_Layout(cache_group->layout());
    layout_h->margin(0);
    layout_h->add(cache_size_label);
    layout_h->add(cache_size);
    cache_group->layout()->add(cache_type);
    cache_group->layout()->add(cache_display);

    layout->add_stretch();

    // Initialize.

    seq_auto->set(File_Prefs::global()->seq_auto());
    command_line_combine->set(File_Prefs::global()->command_line_combine());
    proxy->set(File_Prefs::global()->proxy());
    u8_conversion_widget->set(File_Prefs::global()->u8_conversion());
    cache->set(File_Prefs::global()->cache());
    cache_size->set(File_Prefs::global()->cache_size());
    cache_type->set(File_Prefs::global()->cache_type());
    cache_display->set(File_Prefs::global()->cache_display());

    // Callbacks.

    seq_auto->signal.set(this, seq_auto_callback);
    command_line_combine->signal.set(this, command_line_combine_callback);
    proxy->signal.set(this, proxy_callback);
    u8_conversion_widget->signal.set(this, u8_conversion_callback);
    cache->signal.set(this, cache_callback);
    cache_size->signal.set(this, cache_size_callback);
    cache_type->signal.set(this, cache_type_callback);
    cache_display->signal.set(this, cache_display_callback);
}

void File_Prefs_Widget::seq_auto_callback(bool in)
{
    File_Prefs::global()->seq_auto(in);
}

void File_Prefs_Widget::command_line_combine_callback(bool in)
{
    File_Prefs::global()->command_line_combine(in);
}

void File_Prefs_Widget::proxy_callback(int in)
{
    File_Prefs::global()->proxy(static_cast<Pixel_Data_Info::PROXY>(in));
}

void File_Prefs_Widget::u8_conversion_callback(bool in)
{
    File_Prefs::global()->u8_conversion(in);
}

void File_Prefs_Widget::cache_callback(bool in)
{
    File_Prefs::global()->cache(in);
}

void File_Prefs_Widget::cache_size_callback(int in)
{
    File_Prefs::global()->cache_size(in);
}

void File_Prefs_Widget::cache_type_callback(int in)
{
    File_Prefs::global()->cache_type(static_cast<Cache::CACHE>(in));
}

void File_Prefs_Widget::cache_display_callback(bool in)
{
    File_Prefs::global()->cache_display(in);
}

} // djv_view

