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

//! \file djv_window.cpp

#include <djv_window.h>

#include <djv_application.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/x.H>

#if defined(DJV_WINDOWS)
//#include "winrc.h"
#elif defined(DJV_MACOS)
#elif defined(DJV_X11)
#include <X11/xpm.h>
#include "../../etc/X11/projector32x32.xpm"
#endif

namespace djv
{

//------------------------------------------------------------------------------
// X11
//------------------------------------------------------------------------------

#if defined(DJV_X11)

namespace
{

struct X11
{
    X11() :
        icon(0),
        mask(0)
    {
        fl_open_display();

        XpmCreatePixmapFromData(
            fl_display,
            DefaultRootWindow(fl_display),
            projector32x32_xpm,
            &icon,
            &mask,
            0);

        atom_net_active_window =
            XInternAtom(fl_display, "_NET_ACTIVE_WINDOW", False);
    }

    ~X11()
    {
        if (icon)
        {
            XFreePixmap(fl_display, icon);
        }

        if (mask)
        {
            XFreePixmap(fl_display, mask);
        }
    }

    Pixmap icon;
    Pixmap mask;

    Atom atom_net_active_window;
};

X11 * _x11 = 0;

} // namespace

#endif // DJV_X11

//------------------------------------------------------------------------------
// Window
//------------------------------------------------------------------------------

Window::Window(const String & title) :
    Fl_Double_Window(0, 0, 0, 0),
    close_signal(this),
    _title      (title),
    _fullscreen (false),
    _resizable  (true)
{
    debug_path("Window");

    //DJV_DEBUG("Window::Window");
    //DJV_DEBUG_PRINT("title = " << title);

    Fl_Double_Window::end();

#if defined(DJV_WINDOWS)

    /*HICON icon = LoadIcon(fl_display, MAKEINTRESOURCE(DJV_ICON));
    if (! icon)
    {
      //DJV_DEBUG_PRINT("error = " << error::last_error());
    }
    else
    {
      Fl_Double_Window::icon((char *)icon);
    }*/

#elif defined(DJV_X11)

    if (! _x11)
    {
        _x11 = new X11;
    }

    Fl_Double_Window::icon((char *)_x11->icon);

#endif

    Fl_Double_Window::label(_title.c_str());

    callback(close_callback, this);

    DJV_APP->window_add(this);
}

Window::~Window()
{
    //DJV_DEBUG("Window::~Window");
}

void Window::del()
{
    //DJV_DEBUG("Window::del");

    Abstract_Widget::del();

    DJV_APP->window_del(this);
}

void Window::title(const String & in)
{
    _title = in;

    Fl_Double_Window::label(_title.c_str());
}

const String & Window::title() const
{
    return _title;
}

void Window::fullscreen(bool in)
{
    if (in == _fullscreen)
    {
        return;
    }

    //DJV_DEBUG("Window::fullscreen");
    //DJV_DEBUG_PRINT("in = " << in);

    _fullscreen = in;

    if (_fullscreen)
    {
        _geom_tmp = Box2i(V2i(x(), y()), geom().size);

        Fl_Double_Window::fullscreen();
    }
    else
    {
        fullscreen_off(_geom_tmp.x, _geom_tmp.y, _geom_tmp.w, _geom_tmp.h);

        if (_resizable)
        {
            const V2i min = size_min();
            size_range(min.x, min.y, 0, 0);
        }
        else
        {
            size_range(_geom_tmp.w, _geom_tmp.h, _geom_tmp.w, _geom_tmp.h);
        }

        free_position();
    }
}

bool Window::fullscreen() const
{
    return _fullscreen;
}

int Window::handle(int in)
{
    //DJV_DEBUG("Window::handle");
    //DJV_DEBUG_PRINT("in = " << in);

    switch (Abstract_Widget::handle(in))
    {
        case -1:
            return 0;

        case 1:
            return 1;
    }

    switch (in)
    {
        case FL_FOCUS:
            DJV_APP->window_focus(this);
            break;
    }

    return Fl_Double_Window::handle(in);
}

namespace
{

//! \todo Don't let FLTK call our show() recursively when calling
//! show(argc, argv).

class Show
{
public:

    Show()
    {
        ++_ref;
    }

    ~Show()
    {
        --_ref;
    }

    bool is_recurse() const
    {
        return _ref > 1;
    }

private:

    static int _ref;
};

int Show::_ref = 0;

}

void Window::show()
{
    //DJV_DEBUG("Window::show");

    //! \todo Windows doesn't like show(argc, argv)?

#if defined (DJV_X11)

    Show _show;

    if (! _show.is_recurse())
    {
        //! \todo Can't pass argc = 0.

        static const String name = DJV_APP->command_line_name();

        static const char * argv [] =
        {
            name.c_str()
        };

        Fl_Window::show(1, (char **)argv);
    }
    else
    {
        Fl_Double_Window::show();
    }

#else

    Fl_Double_Window::show();

#endif

    Abstract_Widget::show();

#if defined(DJV_X11)

    //! \todo Tell the window manager to raise the window.
    //!
    //! References:
    //!
    //! * Extended Window Manager Hints
    //!   http://standards.freedesktop.org/wm-spec/1.3/ar01s03.html

    ::Window root = RootWindow(fl_display, fl_screen);

    XEvent xevent;
    xevent.xclient.type = ClientMessage;
    xevent.xclient.window = fl_xid(this);
    xevent.xclient.message_type = _x11->atom_net_active_window;
    xevent.xclient.format = 32;
    xevent.xclient.data.l[0] = 1;
    xevent.xclient.data.l[1] = CurrentTime;
    xevent.xclient.data.l[0] = root;

    XSendEvent(
        fl_display,
        root,
        False,
        SubstructureRedirectMask,
        &xevent);

#endif

    //! \todo Is this still necessary?

    Style::global()->color_update();
}

void Window::draw()
{
    Fl_Double_Window::draw();

    Abstract_Widget::draw();
}

void Window::dirty()
{
    //DJV_DEBUG("Window::dirty");

    Abstract_Widget::dirty();
}

void Window::update()
{
    //DJV_DEBUG("Window::update");
    //DJV_DEBUG_PRINT("resizable = " << _resizable);

    dirty();

    const V2i min = size_min();
    
    const V2i size =
        Vector_Util::clamp(geom().size, min, V2i(Fl::w(), Fl::h()));

    //DJV_DEBUG_PRINT("min = " << min);
    //DJV_DEBUG_PRINT("size = " << size);

    Fl_Double_Window::size(size.x, size.y);

    if (_resizable)
    {
        size_range(min.x, min.y, 0, 0);
    }
    else
    {
        size_range(size.x, size.y, size.x, size.y);
    }

    Abstract_Widget::geom(Box2i(size));

    redraw();
}

void Window::size(const V2i & in, bool resizable)
{
    //DJV_DEBUG("Window::size");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("resizable = " << resizable);

    dirty();

    const V2i min = Math::max(size_min(), V2i(16, 16));
    const V2i size = Vector_Util::clamp(in, min, V2i(Fl::w(), Fl::h()));

    //DJV_DEBUG_PRINT("min = " << min);
    //DJV_DEBUG_PRINT("size = " << size);

    if (_fullscreen)
    {
        fullscreen_off(_geom_tmp.x, _geom_tmp.y, size.x, size.y);
        _fullscreen = false;
    }
    else
    {
        Fl_Double_Window::size(size.x, size.y);
    }

    _resizable = resizable;

    if (_resizable)
    {
        size_range(min.x, min.y, 0, 0);
    }
    else
    {
        size_range(size.x, size.y, size.x, size.y);
    }

    free_position();
}

void Window::geom(const Box2i & in)
{
    //DJV_DEBUG("Window::geom");
    //DJV_DEBUG_PRINT("in = " << in);

    const bool dirty = in.size != geom().size;

    //DJV_DEBUG_PRINT("dirty = " << dirty);

    Fl_Double_Window::resize(in.x, in.y, in.w, in.h);

    if (dirty)
    {
        Abstract_Widget::geom(Box2i(in.size));
        //if (layout())
        //  layout()->debug();
    }
}

void Window::resize(int x, int y, int w, int h)
{
    geom(Box2i(x, y, w, h));
}

void Window::size(SIZE x, SIZE y)
{
    Abstract_Widget::size(x, y);
}

const Box2i & Window::geom() const
{
    return Abstract_Widget::geom();
}

int Window::visible() const
{
    return Fl_Double_Window::visible();
}

const Layout_Item::SIZE * Window::size() const
{
    return Abstract_Widget::size();
}

void Window::close_callback()
{
    hide();

    close_signal.emit(true);
}

} // djv
