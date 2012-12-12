//#define DJV_DEBUG

#include "widget.h"

#include <djv_base/application.h>

Widget::Widget() :
  _image(0)
{}

void Widget::set(const Image * in)
{
  _image = in;
}

void Widget::draw()
{
  Gl::draw();
  
  DJV_DEBUG("Widget::draw");
  
  const Box2i & geom = this->geom();
  
  DJV_DEBUG_PRINT("geom = " << geom);
  
  djv_image::gl::ortho(V2i(geom.w, geom.h));
  DJV_DEBUG_GL(glViewport(0, 0, geom.w, geom.h));
  
  DJV_DEBUG_GL(glClearColor(0.0, 0.0, 1.0, 0.0));
  DJV_DEBUG_GL(glClear(GL_COLOR_BUFFER_BIT));
  
  if (_image)
  {
    const djv_image::Info & info = _image->info();
    
    DJV_DEBUG_PRINT("info = " << info);
    
    try
    {
      _image->bind();
    }
    catch (Error error)
    {
      djv_base::application::global()->error(error);
    }

    DJV_DEBUG_GL(glPushMatrix());
    
    double u [] = { 0, 0 }, v [] = { 0, 0 };
  
    u[! info.mirror.x] = info.size.x;
    v[! info.mirror.y] = info.size.y;
  
    if (GL_TEXTURE_2D == djv_image::gl::texture())
    {
      u[! info.mirror.x] /= static_cast<double>(info.size.x);
      v[! info.mirror.y] /= static_cast<double>(info.size.y);
    }
  
    const V2f uv [] =
    {
      V2f(u[0], v[0]),
      V2f(u[0], v[1]),
      V2f(u[1], v[1]),
      V2f(u[1], v[0])
    };
    
    DJV_DEBUG_GL(glBegin(GL_QUADS));
    
    djv_image::gl::draw_box(info.size, uv);
    
    DJV_DEBUG_GL(glEnd());
  
    DJV_DEBUG_GL(glPopMatrix());
  }
}

