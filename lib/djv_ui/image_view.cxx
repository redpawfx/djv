/*------------------------------------------------------------------------------
 lib/djv_ui/image_view.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_view.h"

#include <djv_ui/application.h>
#include <djv_image/image.h>
#include <djv_base/matrix.h>

namespace djv_ui {
namespace image {

//------------------------------------------------------------------------------
// View
//------------------------------------------------------------------------------

View::View() :
  signal(this),
  _data(0),
  _zoom(1.0)
{}

View::~View()
{}

void View::set(const djv_image::data::Data * in)
{
  _data = in;
}

const djv_image::data::Data * View::get() const
{
  return _data;
}

const djv_image::gl_data::Data & View::gl_data() const
{
  return _gl;
}

void View::view(const V2i & in)
{
  if (in == _view)
    return;
  
  _view = in;
  
  signal.emit(true);
}

const V2i & View::view() const
{
  return _view;
}

void View::zoom(double in)
{
  if (in == _zoom)
    return;
  
  DJV_DEBUG("View::zoom");
  DJV_DEBUG_PRINT("in = " << in);
  
  _zoom = in;
  
  signal.emit(true);
}

void View::zoom(double in, const V2i & focus)
{
  if (in == _zoom)
    return;
  
  DJV_DEBUG("View::zoom");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("focus = " << focus);
  
  _view = focus + V2i(V2f(_view - focus) * (in / _zoom));
  _zoom = in;
  
  signal.emit(true);
}

double View::zoom() const
{
  return _zoom;
}

void View::view_zero()
{
  _view = V2i();
  _zoom = 1.0;
  
  signal.emit(true);
}

void View::view_center()
{
  view(V2f(geom().size) / 2.0 - bbox().size / 2.0);
}

void View::view_fit()
{
  if (! _data)
    return;
  
  DJV_DEBUG("View::view_fit");
  
  const Box2i & geom = this->geom();
  
  DJV_DEBUG_PRINT("geom = " << geom.size);
  
  _view.zero();
  _zoom = 1.0;
  const Box2f bbox = this->bbox();
  
  DJV_DEBUG_PRINT("bbox = " << bbox);
  
  _zoom = vector::is_size_valid(bbox.size) ?
    math::min(geom.w / bbox.size.x, geom.h / bbox.size.y) : 1.0;
  
  DJV_DEBUG_PRINT("zoom = " << _zoom);

  _view =
    V2f(geom.size) / 2.0 -
    bbox.size * _zoom / 2.0 -
    this->bbox().position;
  
  DJV_DEBUG_PRINT("view = " << _view);
  
  signal.emit(true);
}

void View::option(const djv_image::gl_data::Option & in)
{
  _option = in;
}

const djv_image::gl_data::Option & View::option() const
{
  return _option;
}

void View::draw()
{
  Gl::draw();

  if (! visible())
    return;
    
  DJV_DEBUG("View::draw");
  DJV_DEBUG_PRINT("valid = " << valid());
  DJV_DEBUG_PRINT("view = " << _view);
  DJV_DEBUG_PRINT("zoom = " << _zoom);

  const Box2i & geom = this->geom();
  
  //DJV_DEBUG_PRINT("geom = " << geom.size);
    
  //if (! valid())
  //{
    djv_image::gl::ortho(V2i(geom.w, geom.h));
    glViewport(0, 0, geom.w, geom.h);
  //}
  
  djv_image::Color background(djv_image::pixel::RGB_F32);
  djv_image::color::convert(_option.background, background);
  glClearColor(
    background.get_f32(0),
    background.get_f32(1),
    background.get_f32(2),
    0.0
  );
  //glClearColor(0, 1, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  
  if (! _data)
    return;
  
  DJV_DEBUG_PRINT("data = " << *_data);
  
  djv_image::gl_data::Option option = _option;
  option.xform.position += _view;
  option.xform.scale *= _zoom;
  
  try
  {
    _gl.draw(*_data, option);
  }
  catch (Error in)
  {
    application::global()->error(in);
  }
}

Box2f View::bbox() const
{
  if (! _data)
    return Box2f();
  
  const djv_image::data::Info & info = _data->info();
  
  djv_image::gl_data::Xform xform = _option.xform;
  xform.position += _view;
  xform.scale *= _zoom;
  
  return
    djv_image::gl_data::xform_matrix(xform) *
    Box2f(_data->info().size * djv_image::data::proxy_scale(info.proxy));
}

//------------------------------------------------------------------------------

}
}
