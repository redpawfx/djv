//#define DJV_DEBUG

#include "op_levels.h"

#include <djv_image/gl.h>

namespace glsl {
namespace op_levels {

namespace {

const String src_levels =
"  gl_FragColor = levels_fnc(\n"
"    texture2DRect(texture, gl_TexCoord[0].st), levels);\n";

const String src_levels_soft_clip =
"  gl_FragColor = soft_clip_fnc(levels_fnc(\n"
"    texture2DRect(texture, gl_TexCoord[0].st), levels), soft_clip);\n";

const String src =
"#version 110\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"\n"
"struct Levels\n"
"{\n"
"  float input0, input1;\n"
"  float gamma;\n"
"  float output0, output1;\n"
"};\n"
"\n"
"vec4 levels_fnc(vec4 value, Levels levels)\n"
"{\n"
"  value[0] = "
"    pow(max(value[0] - levels.input0, 0.0) / levels.input1, levels.gamma) * "
"      levels.output1 + levels.output0;\n"
"  value[1] = "
"    pow(max(value[1] - levels.input0, 0.0) / levels.input1, levels.gamma) * "
"      levels.output1 + levels.output0;\n"
"  value[2] = "
"    pow(max(value[2] - levels.input0, 0.0) / levels.input1, levels.gamma) * "
"      levels.output1 + levels.output0;\n"
"  return value;\n"
"}\n"
"\n"
"vec4 soft_clip_fnc(vec4 value, float soft_clip)\n"
"{\n"
"  float tmp = 1.0 - soft_clip;\n"
"  if (value[0] > tmp)\n"
"    value[0] = tmp + (1.0 - exp(-(value[0] - tmp) / soft_clip)) * soft_clip;\n"
"  if (value[1] > tmp)\n"
"    value[1] = tmp + (1.0 - exp(-(value[1] - tmp) / soft_clip)) * soft_clip;\n"
"  if (value[2] > tmp)\n"
"    value[2] = tmp + (1.0 - exp(-(value[2] - tmp) / soft_clip)) * soft_clip;\n"
"  return value;\n"
"}\n"
"\n"
"uniform sampler2DRect texture;\n"
"uniform Levels levels;\n"
"uniform float soft_clip;\n"
"\n"
"void main(void)\n"
"{\n"
"  %%\n"
"\n"
"}\n";

}

void Op::render(const djv_image::Image & in) throw (Error)
{
  DJV_DEBUG("Op::render");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Initialize.
  
  begin();
  
  _texture.init(in);
  
  const State state(_value);
  if (_state != state)
  {
    DJV_DEBUG_PRINT("init");
  
    _render.shader.init(string::Format(src).
      arg(_value.soft_clip > 0.0 ? src_levels_soft_clip : src_levels)
      );

    _state = state;
  }
  
  // Render.
  
  _render.shader.bind();
  const GLhandleARB program = _render.shader.program();

  glUniform1f(glGetUniformLocation(program, "levels.input0"),
    static_cast<GLfloat>(_value.input0));
  glUniform1f(glGetUniformLocation(program, "levels.input1"),
    static_cast<GLfloat>(_value.input1 - _value.input0));
  glUniform1f(glGetUniformLocation(program, "levels.gamma"),
    static_cast<GLfloat>(1.0 / _value.gamma));
  glUniform1f(glGetUniformLocation(program, "levels.output0"),
    static_cast<GLfloat>(_value.output0));
  glUniform1f(glGetUniformLocation(program, "levels.output1"),
    static_cast<GLfloat>(_value.output1 - _value.output0));
  glUniform1f(glGetUniformLocation(program, "soft_clip"),
    static_cast<GLfloat>(_value.soft_clip));
  
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(program, "texture"), 0);
  _texture.bind();
  
  const djv_image::Info & info = in.info();
  djv_image::gl::ortho(info.size);
  glViewport(0, 0, info.size.x, info.size.y);
  glClear(GL_COLOR_BUFFER_BIT);
  quad(info);
  
  end();
}

}
}
