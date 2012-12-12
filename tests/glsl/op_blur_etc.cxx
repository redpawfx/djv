#include "op_blur.h"

namespace glsl {
namespace op_blur {

State::State() :
  _init(false)
{}

State::State(const Value & in) :
  _value(in),
  _init(true)
{}

bool State::operator != (const State & in) const
{
  return
    _value.radius != in._value.radius ||
    _value.filter != in._value.filter ||
    ! _init;
}

const List<String> & label_filter()
{
  static const List<String> data = List<String>() <<
    "Box" <<
    "Gaussian";
  return data;
}

}
}
