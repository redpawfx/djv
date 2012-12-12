#ifndef OP_H
#define OP_H

#include "util.h"
#include <djv_ui/widget.h>

namespace glsl {

class Op : public djv_ui::widget::Widget
{
public:

  Op();
  
  virtual ~Op();
  
  virtual String name() const = 0;

  virtual void render(const djv_image::Image &) throw (Error) = 0;
  
  djv_ui::callback::Signal<bool> signal;

protected:

  void begin();
  void end();
  
  Texture _texture;
};

class Op_Null : public Op
{
public:
  
  Op_Null();
  
  String name() const { return "Null"; }

  void render(const djv_image::Image &) throw (Error);

private:

  Shader _shader;
  bool _init;

};

}

#endif
