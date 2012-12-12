/*-----------------------------------------------------------------------------
 bin/djv_view/tool_magnify.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "tool_magnify.h"

#include "application.h"
#include "view_widget.h"
#include <djv_ui/choice.h>
#include <djv_ui/image_view.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/prefs.h>
#include <djv_ui/push_button.h>
#include <djv_ui/style.h>
#include <djv_image/gl_offscreen.h>
#include <FL/fl_draw.H>

namespace djv_bin {
namespace view {
namespace tool_magnify {

//------------------------------------------------------------------------------
// View
//------------------------------------------------------------------------------

namespace {

class View : public djv_ui::Frame
{
public:

  View();
  
  void del();
  
  void set(const view::Widget *, const V2i & pick, double zoom);
  
  void draw();
  
  void dirty();
  void geom(const Box2i &);
  
private:

  djv_image::Data _data;
  std::auto_ptr<djv_image::gl_offscreen::Buffer> _buffer;
};

View::View()
{
  style(djv_ui::frame::STYLE_BORDER);
  color(FL_BLACK);
}

void View::del()
{
  Frame::del();
  
  _buffer.reset();
}

void View::set(const view::Widget * in, const V2i & pick, double zoom)
{
  DJV_DEBUG("View::set");
  DJV_DEBUG_PRINT("pick = " << pick);
  DJV_DEBUG_PRINT("zoom = " << zoom);
  
  if (const djv_image::Data * data = in->get())
  {
    try
    {
      if (! _buffer.get())
        _buffer = std::auto_ptr<djv_image::gl_offscreen::Buffer>(
          djv_image::gl_offscreen::global()->buffer_create()
        );
      
      _buffer->init(_data.info());

      djv_image::gl_data::Option option = in->option();
      option.xform.position -= pick - ((_data.size() - 1) / 2);
      option.xform.scale *= zoom;

      djv_image::gl_data::copy(
        *data,
        &_data,
        option,
        &in->gl_data(),
        _buffer.get(),
        false
      );
    }
    catch (Error in)
    {
      application::global()->error(in);
    }
  }
  else
  {
    _data.zero();
  }
  
  redraw();
}

void View::draw()
{
  const Box2i & geom = frame_geom();
  
  const int w = _data.w();
  const int h = _data.h();
  const int c = _data.channels();
  
  const size_t bytes_scanline = _data.bytes_scanline();
  const size_t bytes_data = _data.bytes_data();

  fl_draw_image(
    _data.data() + bytes_data - bytes_scanline,
    geom.x,
    geom.y,
    w,
    h,
    c,
    -static_cast<int>(bytes_scanline)
  );
  
  Frame::draw();
}

void View::dirty()
{
  Frame::dirty();
  
  size_hint(_data.size());
}

void View::geom(const Box2i & in)
{
  Frame::geom(in);
  
  _data.set(djv_image::Info(frame_geom().size, djv_image::pixel::RGB_U8));
  _data.zero();
}

}

//------------------------------------------------------------------------------
// Dialog::_Dialog
//------------------------------------------------------------------------------

struct Dialog::_Dialog
{
  //djv_ui::image::View * widget;
  View * widget;
  djv_ui::Choice * zoom;
  djv_ui::Push_Button * close;
  //djv_ui::Frame * frame;
};

//------------------------------------------------------------------------------
// Dialog
//------------------------------------------------------------------------------

Dialog::Dialog() :
  Base(dialog_title),
  _view(0),
  _zoom(ZOOM_200),
  _p(new _Dialog)
{
  using namespace djv_ui;
  
  // Create widgets.

  _p->widget =
    //new djv_ui::image::View;
    new View;
  
  _p->zoom =
    new Choice(label_zoom());
  
  _p->close =
    new Push_Button(djv_ui::dialog::label_close);
  
  //_p->frame =
  //  new Frame;
  //_p->frame->style(frame::STYLE_BORDER);
  //_p->frame->color(FL_BLACK);

  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  
  //layout->add(_p->frame);
  //layout->stretch(_p->frame);
  //Layout_V * layout_v = new Layout_V(_p->frame);
  //layout_v->margin(0);
  //layout_v->add(_p->widget);
  layout->add(_p->widget);
  layout->stretch(_p->widget);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(_p->zoom);
  layout_h->add_spacer(-1, true);
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Preferences.
  
  prefs::Prefs prefs(prefs::prefs(), "tool_magnify");
  prefs::get(&prefs, "zoom", &_zoom);

  // Initialize.
  
  widget_update();

  size(max(size_hint(), V2i(300, 300)));
  
  // Callbacks.

  _p->zoom->signal.set(this, zoom_callback);
  _p->close->signal.set(this, close_callback);
}

Dialog::~Dialog()
{}

void Dialog::del()
{
  using namespace djv_ui;
  
  Base::del();

  prefs::Prefs prefs(prefs::prefs(), "tool_magnify");
  prefs::set(&prefs, "zoom", _zoom);
}

void Dialog::set(const view::Widget * in, const V2i & pick)
{
  _view = in;
  _pick = pick;
  
  widget_update();
}

void Dialog::pick(const view::Widget * in, const V2i & pick)
{
  if (! shown() || ! visible())
    return;
  
  set(in, pick);
}

void Dialog::update(const view::Widget * in)
{
  if (in != _view || ! shown() || ! visible())
    return;
  
  widget_update();
}

void Dialog::del(const view::Widget * in)
{
  if (in != _view)
    return;
  
  _view = 0;
  
  //_p->widget->set(0);
  //_p->widget->redraw();
}

void Dialog::show()
{
  _p->close->take_focus();

  djv_ui::dialog::Base::show();
}

void Dialog::dirty()
{
  Base::dirty();
}

void Dialog::zoom_callback(int in)
{
  _zoom = static_cast<ZOOM>(in);
  
  widget_update();
}

void Dialog::close_callback(bool) { hide(); }

void Dialog::widget_update()
{
  if (! _view)
    return;

  DJV_DEBUG("Dialog::widget_update");
  
  callbacks(false);

  const double zoom = math::pow(2, _zoom);
  
  DJV_DEBUG_PRINT("zoom = " << zoom);
  
  /*_p->widget->set(_view->get());
  _p->widget->view(
    -vector::floor<double, int>(V2f(_pick) * zoom) +
    _p->widget->geom().size / 2);
  _p->widget->zoom(_view->zoom() * zoom);
  _p->widget->option(_view->option());
  _p->widget->redraw();*/
  
  _p->widget->set(
    _view,
    vector::floor<double, int>(V2f(_pick) * zoom),
    _view->zoom() * zoom
  );
  
  _p->zoom->set(_zoom);

  callbacks(true);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(ZOOM, label_zoom())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_zoom()
{
  static const List<String> data = List<String>() <<
    "100%" <<
    "200%" <<
    "400%" <<
    "800%" <<
    "1600%";
  
  DJV_ASSERT(data.size() == _ZOOM_SIZE);
  
  return data;
}

extern const String
  dialog_title = "Magnify";

//------------------------------------------------------------------------------

}
}
}

