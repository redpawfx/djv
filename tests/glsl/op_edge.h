#ifndef OP_EDGE_H
#define OP_EDGE_H

#include "op.h"

namespace glsl {
namespace op_edge {

//------------------------------------------------------------------------------
// Operation
//------------------------------------------------------------------------------

class Op : public glsl::Op
{
public:

  Op();
  
  String name() const { return "Edge"; }

  void render(const djv_image::Image &) throw (Error);

private:

  Shader _shader;
  bool _init;
};

//------------------------------------------------------------------------------

}
}

#endif
