//#define DJV_DEBUG

#include "op_blur.h"

#include <djv_image/gl.h>

namespace glsl {
namespace op_blur {

namespace {

const String src =
"#version 110\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"\n"
"%%"
"\n"
"uniform sampler2DRect texture;\n"
"\n"
"void main(void)\n"
"{\n"
"  gl_FragColor = kernel(texture);\n"
"}\n";

}

void Op::render(const djv_image::Image & in) throw (Error)
{
  DJV_DEBUG("Op::render");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Initialize.
  
  begin();
  
  _texture.init(in);
  _render.texture_tmp.init(djv_image::Info(in.size(), in.pixel()));
  
  const int size = _value.radius * 2 + 1;

  const State state(_value);
  if (_state != state)
  {
    _render.offscreen.init();
    _render.kernel.init(size);
    _render.shader.init(string::Format(src).arg(_render.kernel.src()));
    _render.shader.bind();

    // Kernel weights.
    
    memory::Buffer<float> value(size);
    float * p = value();
    switch (_value.filter)
    {
      case BOX:
        for (int i = 0; i < size; ++i)
          p[i] = 1;
        break;
      
      case GAUSSIAN: {
        const double theta = size / 6.0;
        double x = -_value.radius;
        for (int i = 0; i < size; ++i, x += 1.0)
          p[i] = static_cast<float>(
            math::sqrt(1.0 / (math::sqrt(2.0 * math::pi * theta))) *
            math::exp(-(x * x) / (2 * theta * theta))
          );
      } break;
    }
    
    float sum = 0.0;
    for (int i = 0; i < size; ++i)
      sum += p[i];
    for (int i = 0; i < size; ++i)
    {
      DJV_DEBUG_PRINT("i = " << p[i]);
      p[i] = p[i] / sum;
    }
    
    _render.kernel.value(_render.shader.program(), value());
    
    _state = state;
  }
  
  // Horizontal.
    
  _render.offscreen.bind();
  _render.offscreen.set(_render.texture_tmp);
  
  _render.shader.bind();
  const GLuint progam = _render.shader.program();
  
  memory::Buffer<float> offset(size * 2);
  offset.zero();
  float * p = offset();
  for (int i = -_value.radius; i <= _value.radius; ++i, p += 2)
    p[0] = static_cast<float>(i);
  _render.kernel.offset(progam, offset());
  
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(progam, "texture"), 0);
  _texture.bind();
  
  const djv_image::Info & info = in.info();
  djv_image::gl::ortho(info.size);
  glViewport(0, 0, info.size.x, info.size.y);
  quad(info);
  
  _render.offscreen.unbind();

  // Vertical.
  
  offset.zero();
  p = offset();
  for (int i = -_value.radius; i <= _value.radius; ++i, p += 2)
    p[1] = static_cast<float>(i);
  _render.kernel.offset(progam, offset());
  
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(progam, "texture"), 0);
  _render.texture_tmp.bind();
  
  glClear(GL_COLOR_BUFFER_BIT);
  quad(djv_image::Info(info.size, info.pixel));
  
  end();
}

}
}
