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

//! \file djv_file_browser_private.cpp

#include <djv_file_browser_private.h>

#include <djv_application.h>
#include <djv_cursor.h>
#include <djv_font.h>
#include <djv_icon.h>
#include <djv_style.h>

#include <djv_image.h>
#include <djv_user.h>
#include <djv_time.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// File_Browser_Item
//------------------------------------------------------------------------------

File_Browser_Item::File_Browser_Item() :
    _prev(0),
    _next(0),
    _odd(false),
    _image(0),
    _image_is_valid(false),
    _image_init(false)
{}

File_Browser_Item::File_Browser_Item(
    const File & in,
    File_Browser_Item * prev,
    File_Browser_Item * next,
    bool odd) :
    _value(in),
    _prev(prev),
    _next(next),
    _odd(odd),
    _image(0),
    _image_is_valid(Image_Load_Factory::global()->is_valid(_value)),
    _image_init(false)
{}

File_Browser_Item::~File_Browser_Item()
{
    delete _image;
}

namespace
{

V2i thumbnail(
    const V2i & in,
    int size,
    File_Browser::IMAGE image,
    Pixel_Data_Info::PROXY * proxy)
{
    const int image_size = Math::max(in.x, in.y);

    if (image_size <= 0)
    {
        return V2i();
    }

    int _proxy = 0;
    
    double proxy_scale = static_cast<double>(
        Pixel_Data::proxy_scale(Pixel_Data_Info::PROXY(_proxy)));

    if (File_Browser::IMAGE_LOW == image)
    {
        while (
            (image_size / proxy_scale) > size * 2 &&
            _proxy < Pixel_Data_Info::_PROXY_SIZE)
        {
            proxy_scale = static_cast<double>(
                Pixel_Data::proxy_scale(Pixel_Data_Info::PROXY(++_proxy)));
        }
    }
    
    if (proxy)
    {
        *proxy = Pixel_Data_Info::PROXY(_proxy);
    }

    const double scale = size / static_cast<double>(image_size / proxy_scale);
    
    return Vector_Util::ceil<double, int>(V2f(in) / proxy_scale * scale);
}

} // namespace

Fl_Image * File_Browser_Item::image(int size, File_Browser::IMAGE image) const
{
    if (_image_init)
    {
        return _image;
    }

    //DJV_DEBUG("File_Browser_Item::image");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("image = " << image);
    //DJV_DEBUG_PRINT("value = " << _value);

    try
    {
        File_Browser_Item * that = const_cast<File_Browser_Item *>(this);

        that->_image_init = true;

        if (_image_is_valid)
        {
            Cursor cursor(FL_CURSOR_WAIT);

            std::auto_ptr<Image_Load> plugin(
                Image_Load_Factory::global()->get(_value, &that->_image_info));

            //DJV_DEBUG_PRINT("info = " << _image_info);

            Pixel_Data_Info::PROXY proxy(Pixel_Data_Info::PROXY(0));
            const V2i _size = thumbnail(_image_info.size, size, image, &proxy);

            //DJV_DEBUG_PRINT("size = " << _size);
            //DJV_DEBUG_PRINT("proxy = " << proxy);

            Image image;
            
             plugin->load(image, Image_Io_Frame_Info(-1, 0, proxy));

            //DJV_DEBUG_PRINT("image = " << *image);

            that->_image = Icon_Factory::image_to_fltk(
                image,
                _size,
                image.color_profile);
        }
    }
    catch (Error in)
    {
        DJV_APP->error(in);
    }

    return _image;
}

//------------------------------------------------------------------------------
// File_Browser_Browser
//------------------------------------------------------------------------------

File_Browser_Browser::File_Browser_Browser() :
    Fl_Browser_(0, 0, 0, 0),
    signal(this),
    mouse_signal(this),
    action_signal(this),
    _size_text(0),
    _size_icon(0),
    _size_thumbnail(0),
    _margin(0),
    _image(File_Browser::IMAGE(0)),
    _handle_mouse(false)
{
    debug_path("File_Browser_Browser");

    //DJV_DEBUG("File_Browser_Browser::File_Browser_Browser");

    Fl_Browser_::end();

    Fl_Browser_::type(FL_HOLD_BROWSER);
    Fl_Browser_::has_scrollbar(VERTICAL_ALWAYS);

    Fl_Browser_::box(FL_FLAT_BOX);
    Fl_Browser_::scrollbar.slider(FL_BOX_THIN_UP);
    Fl_Browser_::hscrollbar.slider(FL_BOX_THIN_UP);

    for (uint i = 0; i < File_Browser::_SORT_SIZE; ++i)
    {
        _columns[i] = 0;
    }

    callback(widget_callback, this);
}

File_Browser_Browser::~File_Browser_Browser()
{}

void File_Browser_Browser::del()
{
    //DJV_DEBUG("File_Browser_Browser::del");

    Abstract_Widget::del();

    Fl::remove_idle(image_callback, this);

    item_del();
}

void File_Browser_Browser::set(const List<File> & in, File_Browser::IMAGE image)
{
    //DJV_DEBUG("File_Browser_Browser::set");
    //DJV_DEBUG_PRINT("in = " << in.size());
    //DJV_DEBUG_PRINT("image = " << image);

    item_del();

    const size_t size = in.size();

    _items.resize(size);

    for (size_t i = 0; i < size; ++i)
    {
        _items[i] = File_Browser_Item(
            in[i],
            i > 0 ? &_items[i - 1] : 0,
            i < size - 1 ? &_items[i + 1] : 0,
            i % 2 != 0);
    }

    _image = image;

    Fl_Browser_::deselect();
    Fl_Browser_::new_list();
}

File File_Browser_Browser::get() const
{
    const File_Browser_Item * item = reinterpret_cast<File_Browser_Item *>(
        Fl_Browser_::selection());

    return item ? item->get() : File();
}

void File_Browser_Browser::item_del()
{
    //DJV_DEBUG("File_Browser_Browser::item_del");

    _items.clear();
    _image_work.clear();
}

void File_Browser_Browser::columns(const List<int> & in)
{
    //DJV_DEBUG("File_Browser_Browser::columns");
    //DJV_DEBUG_PRINT("in = " << in);

    for (uint i = 0; i < File_Browser::_SORT_SIZE; ++i)
    {
        _columns[i] = i < in.size() ? in[i] : 0;
    }

    redraw();
}

int File_Browser_Browser::handle(int in)
{
    switch (in)
    {
        case FL_PUSH:
        case FL_RELEASE:
        case FL_DRAG:

            _handle_mouse = true;

            break;

        case FL_KEYBOARD:

            _handle_mouse = false;

            if (! Fl::event_state(FL_SHIFT | FL_CTRL | FL_ALT))
                switch (Fl::event_key())
                {
                    case FL_Enter:
                    case FL_KP_Enter:
                    {
                        File_Browser_Item * item =
                            reinterpret_cast<File_Browser_Item *>(
                                Fl_Browser_::selection());

                        if (item)
                        {
                            action_signal.emit(item->get());
                        }

                        return 1;
                    }
                }

            break;
    }

    return Fl_Browser_::handle(in);
}

void File_Browser_Browser::show()
{
    Fl_Browser_::show();

    Abstract_Widget::show();
}

void File_Browser_Browser::draw()
{
    Fl_Browser_::draw();

    Abstract_Widget::draw();
}

int File_Browser_Browser::item_height(void * in) const
{
    File_Browser_Item * item = static_cast<File_Browser_Item *>(in);

    int height = _size_icon;

    if (_image && item)
    {
        if (item->image_is_valid())
        {
            height = Math::max(
                Math::max(_size_thumbnail, _size_text * 3),
                height);
        }
    }

    return Math::max(_size_text, height) + _margin * 2;
}

int File_Browser_Browser::item_width(void *) const
{
    return 0;
}

void File_Browser_Browser::item_draw(
    void * in,
    int x,
    int y,
    int w,
    int h) const
{
    File_Browser_Item * item = static_cast<File_Browser_Item *>(in);

    if (! item)
    {
        return;
    }

    //DJV_DEBUG("File_Browser_Browser::item_draw");
    //DJV_DEBUG_PRINT("in = " << item->get());

    fl_font(Style::global()->font(), Style::global()->font_size());

    const File & file  = item->get();
    const int margin   = Style::global()->margin_widget();
    const int text_off = fl_height() - fl_descent();

    // Background.

    if (item != selection())
    {
        const Fl_Color colors [] =
        {
            FL_BACKGROUND_COLOR,
            FL_BACKGROUND2_COLOR
        };

        fl_color(colors[item->odd()]);
        fl_rectf(x, y, w, h);
    }

    // Name, icon, and thumbnail image.

    fl_color(in == selection() ? FL_BLACK : FL_FOREGROUND_COLOR);

    fl_push_clip(x, y, _columns[0], h);

    int _x = x + margin;

    Fl_Image * image = 0;

    if (_image)
    {
        if (! item->image_init())
        {
            //DJV_DEBUG_PRINT("image init");

            File_Browser_Browser * that =
                const_cast<File_Browser_Browser *>(this);

            that->_image_work.push_back(item);

            Fl::remove_idle(image_callback, that);
            Fl::add_idle(image_callback, that);
        }
        else
        {
            //DJV_DEBUG_PRINT("image draw");

            image = item->image(_size_thumbnail, _image);

            if (image)
            {
                image->draw(_x, y + h / 2 - image->h() / 2);
            }
        }
    }

    if (! image)
    {
        Fl_Image * icon = 0;

        switch (file.type())
        {
            case File::SEQ:
                icon = Icon_Factory::get_("file_seq");
                break;

            case File::DIRECTORY:
                icon = Icon_Factory::get_("directory");
                break;

            default:
                icon = Icon_Factory::get_("file");
                break;
        }

        if (icon)
        {
            icon->draw(_x, y + h / 2 - icon->h() / 2);
        }
    }

    _x += (
        _image ?
        Math::max(
            Style::global()->size_thumbnail(),
            Style::global()->size_icon()) :
        Style::global()->size_icon()) + margin;

    int _y = 0;
    
    if (image)
    {
        const Image_Io_Info & info = item->image_info();

        _y = y + (h / 2 - fl_height() * 3 / 2);

        String tmp = file.get(-1, false);
        fl_draw(tmp.c_str(), _x, _y + text_off);

        tmp = String_Format("%%x%%:%% %%").
              arg(info.size.x).
              arg(info.size.y).
              arg(Vector_Util::aspect(info.size), 0, -2).
              arg(String_Util::label(info.pixel));

        fl_draw(tmp.c_str(), _x, _y + fl_height() + text_off);

        tmp = String_Format("%%@%%").
            arg(Time::units_to_string(
                static_cast<int>(info.seq.list.size()), info.seq.speed)).
            arg(Speed::speed_to_float(info.seq.speed));

        fl_draw(tmp.c_str(), _x, _y + fl_height() * 2 + text_off);
    }
    else
    {
        _y = y + (h / 2 - fl_height() / 2);

        fl_draw(file.get(-1, false).c_str(), _x, _y + text_off);
    }

    fl_pop_clip();

    // Information.

    int column = 0;

    _x = x + _columns[column];
    _y = y + (h / 2 - fl_height() / 2);
    ++column;

    fl_push_clip(_x, y, _columns[column], h);
    String tmp = File_Util::label_type()[file.type()];
    fl_draw(
        tmp.c_str(),
        _x + _columns[column] - margin - Font::string_width(tmp),
        _y + text_off);
    fl_pop_clip();
    _x += _columns[column];
    ++column;

    fl_push_clip(_x, y, _columns[column], h);
    tmp = File_Util::label_size(file.size());
    fl_draw(
        tmp.c_str(),
        _x + _columns[column] - margin - Font::string_width(tmp),
        _y + text_off);
    fl_pop_clip();
    _x += _columns[column];
    ++column;

#if ! defined(DJV_WINDOWS)

    fl_push_clip(_x, y, _columns[column], h);
    tmp = User::uid_to_string(file.user());
    fl_draw(
        tmp.c_str(),
        _x + _columns[column] - margin - Font::string_width(tmp),
        _y + text_off);
    fl_pop_clip();
    _x += _columns[column];
    ++column;

#endif // DJV_WINDOWS

    fl_push_clip(_x, y, _columns[column], h);
    tmp = Time::time_to_string(file.time());
    fl_draw(
        tmp.c_str(),
        _x +
        _columns[column] - margin -
        (Font::string_width(tmp) + Fl_Browser_::scrollbar.w()),
        _y + text_off);
    fl_pop_clip();
    ++column;

    // Layout.

    _x = x + _columns[0];

    for (uint i = 1; i < File_Browser::_SORT_SIZE; ++i)
    {
        fl_color(FL_COLOR_BORDER);
        fl_line(_x - 1, y, _x - 1, y + h - 1);

        _x += _columns[i];
    }
}

const Fl_Widget * File_Browser_Browser::fltk_widget() const
{
    return this;
}

void File_Browser_Browser::dirty()
{
    Abstract_Widget::dirty();

    labelfont(Style::global()->font());
    labelsize(Style::global()->font_size());

    fl_font(Style::global()->font(), Style::global()->font_size());
    _size_text = fl_height();
    _size_icon = Style::global()->size_icon();
    _size_thumbnail = Style::global()->size_thumbnail();
    _margin = Style::global()->margin_widget();
}

void File_Browser_Browser::geom(const Box2i & in)
{
    Fl_Browser_::resize(in.x, in.y, in.w, in.h);

    Abstract_Widget::geom(in);
}

const Box2i & File_Browser_Browser::geom() const
{
    return Abstract_Widget::geom();
}

void File_Browser_Browser::resize(int x, int y, int w, int h)
{
    geom(Box2i(x, y, w, h));
}

int File_Browser_Browser::visible() const
{
    return Fl_Browser_::visible();
}

void File_Browser_Browser::widget_callback()
{
    //DJV_DEBUG("File_Browser_Browser::widget_callback");

    if (const File_Browser_Item * item =
                reinterpret_cast<const File_Browser_Item *>(selection()))
    {
        const File & value = item->get();

        //DJV_DEBUG_PRINT("value = " << value);

        if (Fl::event_clicks())
        {
            Fl::event_clicks(0);

            action_signal.emit(value);
        }
        else
        {
            if (_handle_mouse)
            {
                mouse_signal.emit(value);
            }
            else
            {
                signal.emit(value);
            }
        }
    }
}

void File_Browser_Browser::image_callback()
{
    //DJV_DEBUG("File_Browser_Browser::image_callback");
    //DJV_DEBUG_PRINT("image = " << _image_work.size());
    //DJV_DEBUG_PRINT("visible = " << visible_r());

    if (_image_work.size())
    {
        File_Browser_Item * item = _image_work.front();
        _image_work.pop_front();

        if (displayed(item))
        {
            item->image(_size_thumbnail, _image);
            redraw_line(item);
        }
    }

    if (! _image_work.size())
    {
        Fl::remove_idle(image_callback, this);
    }

    if (! visible_r())
    {
        Fl::remove_idle(image_callback, this);
        _image_work.clear();
    }
}

} // djv

