#include "widget.h"

#include <djv_ui/application.h>

namespace glsl {

Widget::Widget() :
  _op(0),
  _image(0)
{}

void Widget::set(Op * op, const djv_image::Image * image)
{
  _op = op;
  _image = image;
}

void Widget::draw()
{
  Gl::draw();
  
  glClearColor(1, 0, 0, 0);
  
  if (_op && _image)
  {
    try
    {
      _op->render(*_image);
    }
    catch (Error error)
    {
      djv_ui::application::global()->error(error);
    }
  }
}

}
