#ifndef OP_COLOR_H
#define OP_COLOR_H

#include "op.h"

namespace glsl {
namespace op_color {

//------------------------------------------------------------------------------
// Options
//------------------------------------------------------------------------------

struct Value
{
  Value() :
    brightness(2.0),
    contrast(1.0),
    saturation(1.0)
  {}
  
  double brightness;
  double contrast;
  double saturation;
};

//------------------------------------------------------------------------------
// Operation
//------------------------------------------------------------------------------

class Op : public glsl::Op
{
public:

  Op();
  
  String name() const { return "Color"; }

  void render(const djv_image::Image &) throw (Error);
  
private:

  DJV_CALLBACK(Op, brightness_callback, double);
  DJV_CALLBACK(Op, contrast_callback, double);
  DJV_CALLBACK(Op, saturation_callback, double);
  
  Value _value;
  Shader _shader;
  bool _init;
};

//------------------------------------------------------------------------------

}
}

#endif
