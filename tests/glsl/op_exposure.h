#ifndef OP_EXPOSURE_H
#define OP_EXPOSURE_H

#include "op.h"

namespace glsl {
namespace op_exposure {

//------------------------------------------------------------------------------
// Options
//------------------------------------------------------------------------------

struct Value
{
  Value() :
    value(0.0),
    defog(0.0),
    knee_low(0.0),
    knee_high(5.0),
    gamma(2.2)
  {}
  
  double value;
  double defog;
  double knee_low, knee_high;
  double gamma;
};

//------------------------------------------------------------------------------
// Operation
//------------------------------------------------------------------------------

class Op : public glsl::Op
{
public:

  Op();
  
  String name() const { return "Exposure"; }

  void render(const djv_image::Image &) throw (Error);
  
private:

  DJV_CALLBACK(Op, value_callback, double);
  DJV_CALLBACK(Op, defog_callback, double);
  DJV_CALLBACK(Op, knee_low_callback, double);
  DJV_CALLBACK(Op, knee_high_callback, double);
  DJV_CALLBACK(Op, gamma_callback, double);
  
  Value _value;
  Shader _shader;
  bool _init;
};

//------------------------------------------------------------------------------

}
}

#endif
