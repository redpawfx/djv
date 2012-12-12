#ifndef WIDGET_H
#define WIDGET_H

#include "op.h"
#include <djv_ui/gl.h>

namespace glsl {
using namespace djv_base;

class Widget : public djv_ui::gl::Gl
{
public:

  Widget();

  void set(Op *, const djv_image::Image *);
  
  void draw();

private:

  Op * _op;
  const djv_image::Image * _image;
};

}

#endif
