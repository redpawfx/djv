#ifndef OP_SCALE_H
#define OP_SCALE_H

#include "op.h"

namespace glsl {
namespace op_scale {

//------------------------------------------------------------------------------
// Options
//------------------------------------------------------------------------------

enum TYPE
{
  DEFAULT,
  CUSTOM
};

enum FILTER_DEFAULT
{
  NEAREST,
  LINEAR
};

enum FILTER_CUSTOM
{
  BOX,
  TRIANGLE,
  BELL,
  BSPLINE,
  LANCZOS3,
  CUBIC,
  MITCHELL
};
  
struct Value
{
  Value() :
    size(500, 500),
    type(CUSTOM),
    default_min(LINEAR),
    default_mag(LINEAR),
    custom_min(LANCZOS3),
    custom_mag(MITCHELL)
  {}

  V2i size;
  TYPE type;
  FILTER_DEFAULT default_min, default_mag;
  FILTER_CUSTOM custom_min, custom_mag;
};

//------------------------------------------------------------------------------
// Render
//------------------------------------------------------------------------------

struct Render
{
  Offscreen offscreen;
  Texture texture_tmp;
  Texture contrib_x, contrib_y;
  Shader shader, shader_x, shader_y;
};

class State_Default
{
public:

  State_Default();
  State_Default(const Value &);
  
  bool operator != (const State_Default &) const;

private:

  FILTER_DEFAULT _filter_min, _filter_mag;
  bool _init;
};

class State_Custom
{
public:

  State_Custom();
  State_Custom(const Value &);
  
  bool operator != (const State_Custom &) const;

private:

  V2i _size;
  FILTER_CUSTOM _filter_min, _filter_mag;
  bool _init;
};

//------------------------------------------------------------------------------
// Operation
//------------------------------------------------------------------------------

class Op : public glsl::Op
{
public:

  Op();
  
  String name() const { return "Scale"; }

  void render(const djv_image::Image &) throw (Error);
  
private:

  DJV_CALLBACK(Op, width_callback, int);
  DJV_CALLBACK(Op, height_callback, int);
  DJV_CALLBACK(Op, type_callback, int);
  DJV_CALLBACK(Op, default_min_callback, int);
  DJV_CALLBACK(Op, default_mag_callback, int);
  DJV_CALLBACK(Op, custom_min_callback, int);
  DJV_CALLBACK(Op, custom_mag_callback, int);
  
  Value _value;
  Render _render;
  State_Default _state_default;
  State_Custom _state_custom;
};

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

int edge(int, int size);
GLuint filter_to_gl(FILTER_DEFAULT);

const List<String> & label_type();
const List<String> & label_filter_default();
const List<String> & label_filter_custom();

//------------------------------------------------------------------------------

}
}

#endif
