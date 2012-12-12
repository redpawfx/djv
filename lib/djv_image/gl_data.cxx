/*------------------------------------------------------------------------------
 lib/djv_image/gl_data.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "gl_data.h"

#include "gl-1.2_data_private.h"
#include "gl-2.0_data_private.h"

namespace djv_image {
namespace gl_data {

//------------------------------------------------------------------------------
// Filter
//------------------------------------------------------------------------------

Filter::Filter() :
  min(global()->filter().min),
  mag(global()->filter().mag)
{}

Filter::Filter(FILTER min, FILTER mag) :
  min(min),
  mag(mag)
{}

//------------------------------------------------------------------------------
// Data
//------------------------------------------------------------------------------

struct Data::_Data
{
  Fixed fixed;
  Program program;
};

Data::Data() :
  _p(new _Data)
{
  DJV_DEBUG("Data::Data");
}

Data::~Data()
{
  DJV_DEBUG("Data::~Data");
}

void Data::draw(
  const data::Data & in,
  const Option & option,
  bool copy
) const throw (Error) {

  DJV_DEBUG("Data::draw");
  DJV_DEBUG_PRINT("in = " << in);
  
  switch (gl::global()->pipeline())
  {
    case gl::OPENGL_1_2: _p->fixed.draw(in, option); break;
    case gl::OPENGL_2_0: _p->program.draw(in, option, copy); break;
  }
}

void Data::read(data::Data * out)
{
  read(out, Box2i(out->size()));
}

void Data::read(data::Data * out, const Box2i & area)
{
  DJV_DEBUG("Data::read");
  DJV_DEBUG_PRINT("out = " << *out);
  DJV_DEBUG_PRINT("area = " << area);
  
  const Info & info = out->info();
  
  DJV_DEBUG_GL(glPushAttrib(
    GL_CURRENT_BIT |
    GL_ENABLE_BIT |
    GL_PIXEL_MODE_BIT));
  DJV_DEBUG_GL(glDisable(GL_DITHER));
  
  // XXX
  
  if (GLEW_ARB_color_buffer_float)
    DJV_DEBUG_GL(glClampColorARB(GL_CLAMP_READ_COLOR_ARB, GL_FALSE));
  
  // XXX OpenGL sums color components for luminance?
  
  switch (pixel::format(info.pixel))
  {
    case pixel::L:
    case pixel::LA:
      //DJV_DEBUG_GL(glPixelTransferf(GL_GREEN_SCALE, 0.0));
      //DJV_DEBUG_GL(glPixelTransferf(GL_BLUE_SCALE, 0.0));
      DJV_DEBUG_GL(glPixelTransferf(GL_RED_SCALE,
        static_cast<GLfloat>(1.0 / 3.0)));
      DJV_DEBUG_GL(glPixelTransferf(GL_GREEN_SCALE,
        static_cast<GLfloat>(1.0 / 3.0)));
      DJV_DEBUG_GL(glPixelTransferf(GL_BLUE_SCALE,
        static_cast<GLfloat>(1.0 / 3.0)));
      break;

    default: break;
  }
  
  state_pack(info, area.position);

  DJV_DEBUG_GL(glReadPixels(
    0, 0, area.w, area.h,
    gl::format(info.pixel, info.bgr),
    gl::type(info.pixel),
    out->data()));

  //state_reset();

  DJV_DEBUG_GL(glPopAttrib());
}

//------------------------------------------------------------------------------

}
}

