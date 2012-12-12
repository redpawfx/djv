#ifndef OP_SHARPEN_H
#define OP_SHARPEN_H

#include "op.h"

namespace glsl {
namespace op_sharpen {

//------------------------------------------------------------------------------
// Options
//------------------------------------------------------------------------------

struct Value
{
  Value() :
    value(1.0)
  {}
  
  double value;
};

//------------------------------------------------------------------------------
// Operation
//------------------------------------------------------------------------------

class Op : public glsl::Op
{
public:

  Op();
  
  String name() const { return "Sharpen"; }

  void render(const djv_image::Image &) throw (Error);
  
private:

  DJV_CALLBACK(Op, value_callback, double);
  
  Value _value;
  Shader _shader;
  bool _init;
};

//------------------------------------------------------------------------------

}
}

#endif
