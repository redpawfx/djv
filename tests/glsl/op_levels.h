#ifndef OP_LEVELS_H
#define OP_LEVELS_H

#include "op.h"

namespace glsl {
namespace op_levels {

//------------------------------------------------------------------------------
// Options
//------------------------------------------------------------------------------
  
struct Value
{
  Value() :
    input0(0.0),
    input1(1.0),
    gamma(1.0),
    output0(0.0),
    output1(1.0),
    soft_clip(0.0)
  {}

  double input0, input1;
  double gamma;
  double output0, output1;
  double soft_clip;
};

//------------------------------------------------------------------------------
// Render
//------------------------------------------------------------------------------

struct Render
{
  Shader shader;
};

class State
{
public:

  State();
  State(const Value &);

  bool operator != (const State &) const;

private:

  double _soft_clip;
  bool _init;
};

//------------------------------------------------------------------------------
// Operation
//------------------------------------------------------------------------------

class Op : public glsl::Op
{
public:

  Op();
  
  String name() const { return "Levels"; }

  void render(const djv_image::Image &) throw (Error);
  
private:

  DJV_CALLBACK(Op, input0_callback, double);
  DJV_CALLBACK(Op, input1_callback, double);
  DJV_CALLBACK(Op, gamma_callback, double);
  DJV_CALLBACK(Op, output0_callback, double);
  DJV_CALLBACK(Op, output1_callback, double);
  DJV_CALLBACK(Op, soft_clip_callback, double);
  
  Value _value;
  Render _render;
  State _state;
};

//------------------------------------------------------------------------------

}
}

#endif
