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

//! \file djv_file_browser_private.h

#ifndef DJV_FILE_BROWSER_PRIVATE_H
#define DJV_FILE_BROWSER_PRIVATE_H

#include <djv_check_button.h>
#include <djv_choice_widget.h>
#include <djv_columns.h>
#include <djv_file_browser.h>
#include <djv_file_browser_prefs.h>
#include <djv_menu.h>
#include <djv_prefs.h>
#include <djv_style.h>
#include <djv_text_edit.h>
#include <djv_tool_button.h>

#include <djv_file.h>
#include <djv_image_io.h>

#include <FL/Fl_Browser_.H>

#include <list>

namespace djv
{

//------------------------------------------------------------------------------
// File_Browser_Item
//------------------------------------------------------------------------------

class File_Browser_Item
{
public:

    File_Browser_Item();

    File_Browser_Item(
        const File &,
        File_Browser_Item * prev,
        File_Browser_Item * next,
        bool odd);

    ~File_Browser_Item();

    const File & get() const
    {
        return _value;
    }

    File_Browser_Item * prev() const
    {
        return _prev;
    }

    File_Browser_Item * next() const
    {
        return _next;
    }

    bool odd() const
    {
        return _odd;
    }

    Fl_Image * image(int size, File_Browser::IMAGE) const;

    const Image_Io_Info & image_info() const
    {
        return _image_info;
    }

    bool image_is_valid() const
    {
        return _image_is_valid;
    }

    bool image_init() const
    {
        return _image_init;
    }

private:

    File                _value;
    File_Browser_Item * _prev;
    File_Browser_Item * _next;
    bool                _odd;
    Fl_Image *          _image;
    Image_Io_Info       _image_info;
    bool                _image_is_valid;
    bool                _image_init;
};

//------------------------------------------------------------------------------
// File_Browser_Browser
//------------------------------------------------------------------------------

class File_Browser_Browser : public Fl_Browser_, public Abstract_Widget
{
public:

    File_Browser_Browser();

    virtual ~File_Browser_Browser();

    void del();

    void set(const List<File> &, File_Browser::IMAGE image);

    File get() const;

    Signal<const File &> signal;

    Signal<const File &> mouse_signal;

    Signal<const File &> action_signal;

    void columns(const List<int> &);

    virtual int handle(int);

    virtual void show();

    virtual void draw();

    inline void * item_first() const;

    inline void * item_next(void *) const;

    inline void * item_prev(void *) const;

    int item_height(void *) const;

    int item_width(void *) const;

    void item_draw(void *, int, int, int, int) const;

    virtual const Fl_Widget * fltk_widget() const;

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

    virtual void resize(int, int, int, int);

    virtual int visible() const;

private:

    void item_del();

    DJV_FL_WIDGET_CALLBACK(File_Browser_Browser, widget_callback);
    DJV_FL_CALLBACK(File_Browser_Browser, image_callback);

    List<File_Browser_Item> _items;

    int _columns [File_Browser::_SORT_SIZE];
    int _size_text;
    int _size_icon;
    int _size_thumbnail;
    int _margin;

    File_Browser::IMAGE _image;
    std::list<File_Browser_Item *> _image_work;

    bool _handle_mouse;
};

inline void * File_Browser_Browser::item_first() const
{
    return _items.size() ? (void *)&_items.front() : 0;
}

inline void * File_Browser_Browser::item_next(void * in) const
{
    return in ? static_cast<File_Browser_Item *>(in)->next() : 0;
}

inline void * File_Browser_Browser::item_prev(void * in) const
{
    return in ? static_cast<File_Browser_Item *>(in)->prev() : 0;
}

} // djv

#endif // DJV_FILE_BROWSER_PRIVATE_H

