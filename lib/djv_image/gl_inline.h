/*------------------------------------------------------------------------------
 include/djv_image/gl_inline.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include <djv_image/color.h>
#include <djv_base/box.h>

namespace djv_image {
namespace gl {

//------------------------------------------------------------------------------
// color()
//------------------------------------------------------------------------------

inline void color(const Color & in)
{
  Color tmp(djv_image::pixel::RGB_F32);
  
  djv_image::color::convert(in, tmp);
  
  glColor3f(
    tmp.get_f32(0),
    tmp.get_f32(1),
    tmp.get_f32(2)
  );
}

//------------------------------------------------------------------------------
// draw_box()
//------------------------------------------------------------------------------

inline void draw_box(const Box2i & in)
{
  glVertex2i(in.x, in.y);
  glVertex2i(in.x, in.y + in.h);
  glVertex2i(in.x + in.w, in.y + in.h);
  glVertex2i(in.x + in.w, in.y);
}

inline void draw_box(const Box2f & in)
{
  glVertex2d(in.x, in.y);
  glVertex2d(in.x, in.y + in.h);
  glVertex2d(in.x + in.w, in.y + in.h);
  glVertex2d(in.x + in.w, in.y);
}

inline void draw_box(const Box2i & in, const V2f uv [4])
{
  glTexCoord2d(uv[0].x, uv[0].y);
  glVertex2i(in.x, in.y);
  glTexCoord2d(uv[1].x, uv[1].y);
  glVertex2i(in.x, in.y + in.h);
  glTexCoord2d(uv[2].x, uv[2].y);
  glVertex2i(in.x + in.w, in.y + in.h);
  glTexCoord2d(uv[3].x, uv[3].y);
  glVertex2i(in.x + in.w, in.y);
}

inline void draw_box(const Box2f & in, const V2f uv [4])
{
  glTexCoord2d(uv[0].x, uv[0].y);
  glVertex2d(in.x, in.y);
  glTexCoord2d(uv[1].x, uv[1].y);
  glVertex2d(in.x, in.y + in.h);
  glTexCoord2d(uv[2].x, uv[2].y);
  glVertex2d(in.x + in.w, in.y + in.h);
  glTexCoord2d(uv[3].x, uv[3].y);
  glVertex2d(in.x + in.w, in.y);
}

//------------------------------------------------------------------------------

}
}

