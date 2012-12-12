#ifndef OP_BLUR_H
#define OP_BLUR_H

#include "op.h"

namespace glsl {
namespace op_blur {

//------------------------------------------------------------------------------
// Options
//------------------------------------------------------------------------------

enum FILTER
{
  BOX,
  GAUSSIAN
};

struct Value
{
  Value() :
    radius(3),
    filter(GAUSSIAN)
  {}
  
  int radius;
  FILTER filter;
};

//------------------------------------------------------------------------------
// Render
//------------------------------------------------------------------------------

struct Render
{
  
  Offscreen offscreen;
  Shader shader;
  Kernel kernel;
  Texture texture_tmp;
};

class State
{
public:

  State();
  State(const Value &);
  
  bool operator != (const State &) const;

private:

  Value _value;
  bool _init;
};

//------------------------------------------------------------------------------
// Operation
//------------------------------------------------------------------------------

class Op : public glsl::Op
{
public:

  Op();
  
  String name() const { return "Blur"; }

  void render(const djv_image::Image &) throw (Error);
  
private:

  DJV_CALLBACK(Op, radius_callback, int);
  DJV_CALLBACK(Op, filter_callback, int);
  
  Value _value;
  Render _render;
  State _state;
};

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

const List<String> & label_filter();

//------------------------------------------------------------------------------

}
}

#endif
