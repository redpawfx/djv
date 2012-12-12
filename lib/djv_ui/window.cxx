/*------------------------------------------------------------------------------
 lib/djv_ui/window.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "window.h"

#include <djv_ui/application.h>
#include <djv_ui/global.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/x.H>

#if defined(DJV_WINDOWS)
//#include "winrc.h"
#elif defined(DJV_MACOS)
#elif defined(DJV_X11)
#include <X11/xpm.h>
#include "../../etc/X11/projector32x32.xpm"
#endif

namespace djv_ui {
namespace window {

//------------------------------------------------------------------------------
// X11
//------------------------------------------------------------------------------

#if defined(DJV_MACOS)
#elif defined(DJV_X11)

namespace {

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
      0
    );
    
    atom_net_active_window =
      XInternAtom(fl_display, "_NET_ACTIVE_WINDOW", False);
  }
  
  ~X11()
  {
    if (icon) XFreePixmap(fl_display, icon);
    if (mask) XFreePixmap(fl_display, mask);
  }
  
  Pixmap icon;
  Pixmap mask;
  Atom atom_net_active_window;
};

std::auto_ptr<X11> _x11;

}

#endif

//------------------------------------------------------------------------------
// Window
//------------------------------------------------------------------------------

Window::Window(const String & title) :
  Fl_Double_Window(0, 0, 0, 0),
  close_signal(this),
  _title(title),
  _fullscreen(false),
  _resizable(true)
{
  debug_path("Window");
  
  DJV_DEBUG("Window::Window");

  Fl_Double_Window::end();

#if defined(DJV_WINDOWS)
  
  /*HICON icon = LoadIcon(fl_display, MAKEINTRESOURCE(DJV_ICON));
  if (! icon)
  {
    DJV_DEBUG_PRINT("error = " << error::last_error());
  }
  else
  {
    Fl_Double_Window::icon((char *)icon);
  }*/

#elif defined(DJV_MACOS)
#elif defined(DJV_X11)

  Fl_Double_Window::icon((char *)_x11->icon);

#endif

  Fl_Double_Window::label(_title.c_str());

  callback(close_callback, this);

  djv_ui::global()->window_add(this);
}

Window::~Window()
{
  DJV_DEBUG("Window::~Window");
}

void Window::del()
{
  DJV_DEBUG("Window::del");

  Widget_Base::del();

  djv_ui::global()->window_del(this);
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
    return;

  DJV_DEBUG("Window::fullscreen");
  DJV_DEBUG_PRINT("in = " << in);

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
  
  switch (Widget_Base::handle(in))
  {
    case -1: return 0;
    case 1: return 1;
  }
  switch (in)
  {
    case FL_FOCUS: djv_ui::global()->window_focus(this); break;
  }
  
  return Fl_Double_Window::handle(in);
}

namespace {

// XXX Don't let FLTK call our show() recursively when calling show(argc, argv).

class Show
{
public:

  Show() { ++_ref; }
  
  ~Show() { --_ref; }
  
  bool is_recurse() const { return _ref > 1; }

private:

  static int _ref;
};

int Show::_ref = 0;

}

void Window::show()
{
  DJV_DEBUG("Window::show");
  
  // XXX Windows doesn't like show(argc, argv)?

#if defined (DJV_MACOS)
#elif defined (DJV_X11)

  Show _show;

  if (! _show.is_recurse())
  {
    // XXX Can't pass argc = 0.
    
    const String name = application::global()->command_line_name();

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

  Widget_Base::show();
  
#if defined(DJV_MACOS)
#elif defined(DJV_X11)

  // XXX Tell the window manager to raise the window.
  //
  // References:
  //
  // * Extended Window Manager Hints
  //   http://standards.freedesktop.org/wm-spec/1.3/ar01s03.html
  
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
    &xevent
  );

#endif
  
  // XXX
  
  style::global()->color_update();
}

void Window::draw()
{
  Fl_Double_Window::draw();
  
  Widget_Base::draw();
}

void Window::dirty()
{
  //DJV_DEBUG("Window::dirty");
  
  Widget_Base::dirty();
}

void Window::update()
{
  //DJV_DEBUG("Window::update");
  //DJV_DEBUG_PRINT("resizable = " << _resizable);

  dirty();
  
  const V2i min = size_min();
  const V2i size = clamp(geom().size, min, V2i(Fl::w(), Fl::h()));
  
  //DJV_DEBUG_PRINT("min = " << min);
  //DJV_DEBUG_PRINT("size = " << size);

  Fl_Double_Window::size(size.x, size.y);
  if (_resizable)
    size_range(min.x, min.y, 0, 0);
  else
    size_range(size.x, size.y, size.x, size.y);
  Widget_Base::geom(Box2i(size));
  
  redraw();
}

void Window::size(const V2i & in, bool resizable)
{
  //DJV_DEBUG("Window::size");
  //DJV_DEBUG_PRINT("in = " << in);
  //DJV_DEBUG_PRINT("resizable = " << resizable);

  dirty();
  
  const V2i min = math::max(size_min(), V2i(16, 16));
  const V2i size = clamp(in, min, V2i(Fl::w(), Fl::h()));
  
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
    size_range(min.x, min.y, 0, 0);
  else
    size_range(size.x, size.y, size.x, size.y);

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
    Widget_Base::geom(Box2i(in.size));
    //if (layout())
    //  layout()->debug();
  }
}

void Window::resize(int x, int y, int w, int h)
{
  geom(Box2i(x, y, w, h));
}

void Window::size(layout_item::SIZE x, layout_item::SIZE y)
{
  Widget_Base::size(x, y);
}

const Box2i & Window::geom() const
{
  return Widget_Base::geom();
}

int Window::visible() const
{
  return Fl_Double_Window::visible();
}

const layout_item::SIZE * Window::size() const
{
  return Widget_Base::size();
}

void Window::close_callback()
{
  hide();
  
  close_signal.emit(true);
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global()
{
  DJV_DEBUG("Global::Global");
  
#if defined(DJV_MACOS)
#elif defined(DJV_X11)

  _x11 = std::auto_ptr<X11>(new X11);

#endif
}

Global::~Global()
{
  DJV_DEBUG("Global::~Global");

#if defined(DJV_MACOS)
#elif defined(DJV_X11)

  _x11.reset();

#endif
}

//------------------------------------------------------------------------------

}
}
