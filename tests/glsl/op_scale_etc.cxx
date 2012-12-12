#include "op_scale.h"

namespace glsl {
namespace op_scale {

State_Default::State_Default() :
  _init(false)
{}

State_Default::State_Default(const Value & in) :
  _filter_min(in.default_min),
  _filter_mag(in.default_mag),
  _init(true)
{}

bool State_Default::operator != (const State_Default & in) const
{
  return
    _filter_min != in._filter_min ||
    _filter_mag != in._filter_mag ||
    ! _init;
}

State_Custom::State_Custom() :
  _init(false)
{}

State_Custom::State_Custom(const Value & in) :
  _size(in.size),
  _filter_min(in.custom_min),
  _filter_mag(in.custom_mag),
  _init(true)
{}

bool State_Custom::operator != (const State_Custom & in) const
{
  return
    _size != in._size ||
    _filter_min != in._filter_min ||
    _filter_mag != in._filter_mag ||
    ! _init;
}

int edge(int in, int size)
{
  return math::clamp(in, 0, size - 1);
}

GLuint filter_to_gl(FILTER_DEFAULT in)
{
  switch (in)
  {
    case NEAREST: return GL_NEAREST;
    case LINEAR: return GL_LINEAR;
  }
  return 0;
}

const List<String> & label_type()
{
  static const List<String> data = List<String>() <<
    "Default" <<
    "Custom";
  return data;
}

const List<String> & label_filter_default()
{
  static const List<String> data = List<String>() <<
    "Nearest" <<
    "Linear";
  return data;
}

const List<String> & label_filter_custom()
{
  static const List<String> data = List<String>() <<
    "Box" <<
    "Triangle" <<
    "Bell" <<
    "B-Spline" <<
    "Lanczos3" <<
    "Cubic" <<
    "Mitchell";
  return data;
}

}
}
