/*------------------------------------------------------------------------------
 lib/djv_image/gl_data_copy.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "gl_data.h"

#include <djv_image/gl_offscreen.h>

namespace djv_image {
namespace gl_data {

//------------------------------------------------------------------------------
// copy()
//------------------------------------------------------------------------------

void copy(
  const data::Data & in,
  data::Data * out,
  const Option & option,
  const Data * data,
  gl_offscreen::Buffer * buffer,
  bool copy
) throw (Error) {

  DJV_ASSERT(out);
  
  DJV_DEBUG("copy");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("out = " << *out);
  DJV_DEBUG_PRINT("position = " << option.xform.position);
  DJV_DEBUG_PRINT("clamp = " << option.clamp);

  const V2i & size = out->info().size;

  std::auto_ptr<gl_offscreen::Buffer> _buffer;
  if (! buffer)
  {
    DJV_DEBUG_PRINT("bufffer create");
    
    _buffer = std::auto_ptr<gl_offscreen::Buffer>(
      gl_offscreen::global()->buffer_create());
    buffer = _buffer.get();
    buffer->init(Info(size, out->pixel()));
  }
  buffer->bind();

  gl::ortho(size);
  DJV_DEBUG_GL(glViewport(0, 0, size.x, size.y));
  
  //DJV_DEBUG_GL(glClearColor(0, 1, 0, 0));
  color::Color background(pixel::RGB_F32);
  color::convert(option.background, background);
  DJV_DEBUG_GL(glClearColor(
    background.get_f32(0),
    background.get_f32(1),
    background.get_f32(2),
    0.0));
  DJV_DEBUG_GL(glClear(GL_COLOR_BUFFER_BIT));
  
  Option _option = option;
  
  if (out->info().mirror.x)
    _option.xform.mirror.x = ! _option.xform.mirror.x;
  if (out->info().mirror.y)
    _option.xform.mirror.y = ! _option.xform.mirror.y;
  
  const pixel::TYPE type = pixel::type(out->info().pixel);

  _option.clamp = ! (
#if defined (DJV_OPENEXR)
    pixel::F16 == type ||
#endif
    pixel::F32 == type
  );

  Data _data;
  const Data * data_p = data ? data : &_data;
  data_p->draw(in, _option, copy);
  data_p->read(out);

  buffer->unbind();
}

//------------------------------------------------------------------------------

}
}

