#ifndef TEST_IMAGE_PLAY2_WIDGET_H
#define TEST_IMAGE_PLAY2_WIDGET_H

#include "image.h"
#include <djv_ui/gl.h>

class Widget : public djv_ui::gl::Gl
{
public:

  Widget();

  void set(const Image *);
  
  void draw();

private:

  const Image * _image;
};

#endif

