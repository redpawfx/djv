//#define DJV_DEBUG

#include "op_exposure.h"

#include <djv_image/gl.h>

namespace glsl {
namespace op_exposure {

namespace {

const String src =
"#version 110\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"\n"
"struct Exposure { float v, d, k, f, g; };\n"
"\n"
"float knee(float value, float f)\n"
"{\n"
"  return log(value * f + 1.0) / f;\n"
"}\n"
"\n"
"vec4 exposure_fnc(vec4 value, Exposure exposure)\n"
"{\n"
"  value[0] = max(0.0, value[0] - exposure.d) * exposure.v;\n"
"  value[1] = max(0.0, value[1] - exposure.d) * exposure.v;\n"
"  value[2] = max(0.0, value[2] - exposure.d) * exposure.v;\n"
"\n"
"  if (value[0] > exposure.k)"
"    value[0] = exposure.k + knee(value[0] - exposure.k, exposure.f);\n"
"  if (value[1] > exposure.k)"
"    value[1] = exposure.k + knee(value[1] - exposure.k, exposure.f);\n"
"  if (value[2] > exposure.k)"
"    value[2] = exposure.k + knee(value[2] - exposure.k, exposure.f);\n"
"\n"
"  value[0] = pow(value[0], exposure.g) * 0.332;\n"
"  value[1] = pow(value[1], exposure.g) * 0.332;\n"
"  value[2] = pow(value[2], exposure.g) * 0.332;\n"
"\n"
"  return value;\n"
"}\n"
"\n"
"uniform sampler2DRect texture;\n"
"uniform Exposure exposure;\n"
"\n"
"void main(void)\n"
"{\n"
"  gl_FragColor = texture2DRect(texture, gl_TexCoord[0].st);\n"
"  gl_FragColor = exposure_fnc(gl_FragColor, exposure);\n"
"}\n";

double knee(double x, double f)
{
  return math::log(x * f + 1.0) / f;
}

double knee_f(double x, double y)
{
  double f0 = 0.0, f1 = 1.0;

  while (knee(x, f1) > y)
  {
    f0 = f1;
    f1 = f1 * 2.0;
  }

  for (int i = 0; i < 30; ++i)
  {
    const double f2 = (f0 + f1) / 2.0;
    if (knee(x, f2) < y) f1 = f2;
    else f0 = f2;
  }

  return (f0 + f1) / 2.0;
}

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
  const GLhandleARB program = _shader.program();
  
  const float v = static_cast<float>(math::pow(2.0, _value.value + 2.47393));
  const float d = static_cast<float>(_value.defog);
  const float k = static_cast<float>(math::pow(2.0, _value.knee_low));
  const float f = static_cast<float>(
    knee_f(math::pow(2.0, _value.knee_high) - k, math::pow(2.0, 3.5) - k)
  );
  const float g = static_cast<float>(1.0 / _value.gamma);
  DJV_DEBUG_PRINT("v = " << v);
  DJV_DEBUG_PRINT("d = " << d);
  DJV_DEBUG_PRINT("k = " << k);
  DJV_DEBUG_PRINT("f = " << f);
  DJV_DEBUG_PRINT("g = " << g);
  glUniform1f(glGetUniformLocation(program, "exposure.v"), v);
  glUniform1f(glGetUniformLocation(program, "exposure.d"), d);
  glUniform1f(glGetUniformLocation(program, "exposure.k"), k);
  glUniform1f(glGetUniformLocation(program, "exposure.f"), f);
  glUniform1f(glGetUniformLocation(program, "exposure.g"), g);
  
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
