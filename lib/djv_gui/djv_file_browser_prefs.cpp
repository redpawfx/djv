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

//! \file djv_file_browser_prefs.cpp

#include <djv_file_browser_private.h>

#include <djv_application.h>
#include <djv_file.h>
#include <djv_row_layout.h>
#include <djv_shortcut_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
// File_Browser_Prefs
//------------------------------------------------------------------------------

File_Browser_Prefs::File_Browser_Prefs() :
    _navigate(File_Browser::NAVIGATE_SINGLE_CLICK)
{
    //DJV_DEBUG("File_Browser_Prefs::File_Browser_Prefs");

    Prefs prefs(Prefs::prefs_global(), "file_browser");

    Prefs::get_(&prefs, "recent", &_recent);
    Prefs::get_(&prefs, "bookmark", &_bookmark);
    Prefs::get_(&prefs, "navigate", &_navigate);

    _shortcut += Shortcut(label_shortcut()[UP], FL_COMMAND + '\\');
    _shortcut += Shortcut(label_shortcut()[PREV], FL_COMMAND + FL_BackSpace);
    _shortcut += Shortcut(label_shortcut()[CURRENT], FL_COMMAND + '.');
    _shortcut += Shortcut(label_shortcut()[HOME], FL_COMMAND + ',');
    _shortcut += Shortcut(label_shortcut()[DESKTOP], FL_COMMAND + 'k');
    _shortcut += Shortcut(label_shortcut()[ROOT], FL_COMMAND + '/');
    _shortcut += Shortcut(label_shortcut()[RELOAD], FL_COMMAND + 'r');
    _shortcut += Shortcut(label_shortcut()[SEQ_OFF], FL_COMMAND + 'q');
    _shortcut += Shortcut(label_shortcut()[SEQ_SPARSE], FL_COMMAND + 'a');
    _shortcut += Shortcut(label_shortcut()[SEQ_RANGE], FL_COMMAND + 'z');
    _shortcut += Shortcut(label_shortcut()[HIDDEN], FL_COMMAND + 'n');
    _shortcut += Shortcut(label_shortcut()[SORT_NAME], FL_COMMAND + '1');
    _shortcut += Shortcut(label_shortcut()[SORT_SIZE], FL_COMMAND + '2');
# if ! defined(DJV_WINDOWS)
    _shortcut += Shortcut(label_shortcut()[SORT_USER], FL_COMMAND + '3');
    _shortcut += Shortcut(label_shortcut()[SORT_TIME], FL_COMMAND + '4');
# else
    _shortcut += Shortcut(label_shortcut()[SORT_TIME], FL_COMMAND + '3');
# endif
    _shortcut += Shortcut(label_shortcut()[SORT_REVERSE], FL_COMMAND + '`');
    _shortcut += Shortcut(label_shortcut()[SORT_DIRECTORY], FL_COMMAND + 'd');
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_ADD], FL_COMMAND + 'b');
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_1], FL_F + 1);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_2], FL_F + 2);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_3], FL_F + 3);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_4], FL_F + 4);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_5], FL_F + 5);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_6], FL_F + 6);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_7], FL_F + 7);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_8], FL_F + 8);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_9], FL_F + 9);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_10], FL_F + 10);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_11], FL_F + 11);
    _shortcut += Shortcut(label_shortcut()[BOOKMARK_12], FL_F + 12);

    Prefs prefs_shortcut(prefs, "shortcut");
    String tmp;

    for (size_t i = 0; i < _shortcut.size(); ++i)
    {
        if (prefs_shortcut.get(_shortcut[i].name, &tmp))
        {
            Shortcut::serialize(&tmp, &_shortcut[i].value);
        }
    }
}

File_Browser_Prefs::~File_Browser_Prefs()
{
    //DJV_DEBUG("File_Browser_Prefs::~File_Browser_Prefs");

    Prefs prefs(Prefs::prefs_global(), "file_browser");

    Prefs::set_(&prefs, "recent", _recent);
    Prefs::set_(&prefs, "bookmark", _bookmark);
    Prefs::set_(&prefs, "navigate", _navigate);

    Prefs prefs_shortcut(prefs, "shortcut");

    for (size_t i = 0; i < _shortcut.size(); ++i)
    {
        prefs_shortcut.set(
            _shortcut[i].name,
            Shortcut::serialize(_shortcut[i].value));
    }
}

void File_Browser_Prefs::recent(const List<String> & in)
{
    _recent = in;
}

void File_Browser_Prefs::recent_add(const String & in)
{
    File_Util::recent(in, _recent);
}

const List<String> & File_Browser_Prefs::recent() const
{
    return _recent;
}

void File_Browser_Prefs::bookmark(const List<String> & in)
{
    _bookmark = in;
}

void File_Browser_Prefs::bookmark_add(const String & in)
{
    _bookmark += in;
}

const List<String> & File_Browser_Prefs::bookmark() const
{
    return _bookmark;
}

void File_Browser_Prefs::navigate(File_Browser::NAVIGATE in)
{
    if (in == _navigate)
    {
        return;
    }

    _navigate = in;
}

File_Browser::NAVIGATE File_Browser_Prefs::navigate() const
{
    return _navigate;
}

const List<String> & File_Browser_Prefs::label_shortcut()
{
    static const List<String> data = List<String>() <<
        "Up" <<
        "Previous" <<
        "Current" <<
        "Home" <<
        "Desktop" <<
        "Root" <<
        "Reload" <<
        "Sequence Off" <<
        "Sequence Sparse" <<
        "Sequence Range" <<
        "Hidden" <<
        "Sort Name" <<
        "Sort Size" <<
#       if ! defined(DJV_WINDOWS)
        "Sort User" <<
#       endif
        "Sort Time" <<
        "Sort Reverse" <<
        "Sort Directories" <<
        "Bookmark Add" <<
        "Bookmark 1" <<
        "Bookmark 2" <<
        "Bookmark 3" <<
        "Bookmark 4" <<
        "Bookmark 5" <<
        "Bookmark 6" <<
        "Bookmark 7" <<
        "Bookmark 8" <<
        "Bookmark 9" <<
        "Bookmark 10" <<
        "Bookmark 11" <<
        "Bookmark 12";

    DJV_ASSERT(data.size() == _SHORTCUT_SIZE);

    return data;
}

void File_Browser_Prefs::shortcut(const List<Shortcut> & in)
{
    if (in == _shortcut)
    {
        return;
    }

    _shortcut = in;

    DJV_APP->window_update();
}

const List<Shortcut> & File_Browser_Prefs::shortcut() const
{
    return _shortcut;
}

//------------------------------------------------------------------------------
// File_Browser_Prefs_Instance
//------------------------------------------------------------------------------

File_Browser_Prefs_Instance::File_Browser_Prefs_Instance(
    Prefs *        prefs,
    const String & prefs_name) :
    _prefs         (prefs),
    _prefs_name    (prefs_name),
    _image         (File_Browser::IMAGE_HIGH),
    _seq           (Seq::COMPRESS_RANGE),
    _type          (-1),
    _hidden        (false),
    _sort          (File_Browser::SORT_NAME),
    _sort_reverse  (false),
    _sort_directory(true),
    _pin           (false)
{
    //DJV_DEBUG("File_Browser_Prefs_Instance::File_Browser_Prefs_Instance");

    Prefs prefs_(_prefs, _prefs_name);

    Prefs::get_(&prefs_, "image", &_image);
    Prefs::get_(&prefs_, "seq", &_seq);
    Prefs::get_(&prefs_, "type", &_type);
    Prefs::get_(&prefs_, "hidden", &_hidden);
    Prefs::get_(&prefs_, "sort", &_sort);
    Prefs::get_(&prefs_, "sort_reverse", &_sort_reverse);
    Prefs::get_(&prefs_, "sort_directory", &_sort_directory);
    Prefs::get_(&prefs_, "pin", &_pin);
}

File_Browser_Prefs_Instance::~File_Browser_Prefs_Instance()
{
    //DJV_DEBUG("File_Browser_Prefs_Instance::~File_Browser_Prefs_Instance");

    Prefs prefs(_prefs, _prefs_name);

    Prefs::set_(&prefs, "image", _image);
    Prefs::set_(&prefs, "seq", _seq);
    Prefs::set_(&prefs, "type", _type);
    Prefs::set_(&prefs, "hidden", _hidden);
    Prefs::set_(&prefs, "sort", _sort);
    Prefs::set_(&prefs, "sort_reverse", _sort_reverse);
    Prefs::set_(&prefs, "sort_directory", _sort_directory);
    Prefs::set_(&prefs, "pin", _pin);
}

void File_Browser_Prefs_Instance::image(File_Browser::IMAGE in)
{
    _image = in;
}

File_Browser::IMAGE File_Browser_Prefs_Instance::image() const
{
    return _image;
}

void File_Browser_Prefs_Instance::seq(Seq::COMPRESS in)
{
    _seq = in;
}

Seq::COMPRESS File_Browser_Prefs_Instance::seq() const
{
    return _seq;
}

void File_Browser_Prefs_Instance::type(int in)
{
    _type = in;
}

int File_Browser_Prefs_Instance::type() const
{
    return _type;
}

void File_Browser_Prefs_Instance::hidden(bool in)
{
    _hidden = in;
}

bool File_Browser_Prefs_Instance::hidden() const
{
    return _hidden;
}

void File_Browser_Prefs_Instance::sort(File_Browser::SORT in)
{
    _sort = in;
}

File_Browser::SORT File_Browser_Prefs_Instance::sort() const
{
    return _sort;
}

void File_Browser_Prefs_Instance::sort_reverse(bool in)
{
    _sort_reverse = in;
}

bool File_Browser_Prefs_Instance::sort_reverse() const
{
    return _sort_reverse;
}

void File_Browser_Prefs_Instance::sort_directory(bool in)
{
    _sort_directory = in;
}

bool File_Browser_Prefs_Instance::sort_directory() const
{
    return _sort_directory;
}

void File_Browser_Prefs_Instance::pin(bool in)
{
    _pin = in;
}

bool File_Browser_Prefs_Instance::pin() const
{
    return _pin;
}

//------------------------------------------------------------------------------
// File_Browser_Prefs_Widget
//------------------------------------------------------------------------------

File_Browser_Prefs_Widget::File_Browser_Prefs_Widget()
{
    // Create widgets.

    Shortcut_Widget * shortcut_widget = new Shortcut_Widget;

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->spacing(Style::global()->spacing_large());

    layout->add(shortcut_widget);
    layout->stretch(shortcut_widget);

    // Initialize.

    shortcut_widget->set(File_Browser_Prefs::global()->shortcut());

    // Callbacks.

    shortcut_widget->signal.set(this, shortcut_callback);
}

void File_Browser_Prefs_Widget::shortcut_callback(const List<Shortcut> & in)
{
    File_Browser_Prefs::global()->shortcut(in);
}

} // djv

