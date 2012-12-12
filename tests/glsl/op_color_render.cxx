//#define DJV_DEBUG

#include "op_color.h"

#include <djv_image/gl_data.h>
#include <djv_base/matrix.h>

namespace glsl {
namespace op_color {

namespace {

const String src =
"#version 110\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"\n"
"vec4 color_fnc(vec4 value, mat4 color)\n"
"{\n"
"  vec4 tmp;\n"
"  tmp[0] = value[0];\n"
"  tmp[1] = value[1];\n"
"  tmp[2] = value[2];\n"
"  tmp[3] = 1.0;\n"
"  tmp *= color;\n"
"  tmp[3] = value[3];\n"
"  return tmp;\n"
"}\n"
"\n"
"uniform sampler2DRect texture;\n"
"uniform mat4 color;\n"
"\n"
"void main(void)\n"
"{\n"
"  gl_FragColor = texture2DRect(texture, gl_TexCoord[0].st);\n"
"  gl_FragColor = color_fnc(gl_FragColor, color);\n"
"}\n";

}

void Op::render(const djv_image::Image & in) throw (Error)
{
  DJV_DEBUG("Op::render");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Initialize.
  
  begin();

  _texture.init(in);

  if (! _init)
  {
    _shader.init(src);
    
    _init = true;
  }

  // Render.
  
  _shader.bind();
  
  const double b = _value.brightness;
  const double c = _value.contrast;
  const double s = _value.saturation;
  
  const M4f value =
    djv_image::gl_data::brightness_matrix(b, b, b) *
    djv_image::gl_data::contrast_matrix(c, c, c) *
    djv_image::gl_data::saturation_matrix(s, s, s);
  glUniformMatrix4fv(glGetUniformLocation(_shader.program(), "color"), 1, false,
    matrix::convert<double, GLfloat>(matrix::transpose(value)).e);
  
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(_shader.program(), "texture"), 0);
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
