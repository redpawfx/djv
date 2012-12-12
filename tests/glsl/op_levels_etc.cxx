#include "op_levels.h"

namespace glsl {
namespace op_levels {

State::State() :
  _init(false)
{}

State::State(const Value & in) :
  _soft_clip(in.soft_clip),
  _init(true)
{}

bool State::operator != (const State & in) const
{
  return
    _soft_clip != in._soft_clip ||
    ! _init;
}

}
}
