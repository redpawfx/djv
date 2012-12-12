//#define DJV_DEBUG

#include "op_edge.h"

#include <djv_ui/layout_row.h>
#include <djv_image/gl.h>

namespace glsl {
namespace op_edge {

Op::Op() :
  _init(false)
{}

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

  if (! _init)
  {
    DJV_DEBUG_PRINT("init");
    
    Kernel kernel;
    kernel.init(9);
    
    _shader.init(string::Format(src).arg(kernel.src()));
    _shader.bind();
  
    const float value [] =
    {
      0.0,  1.0, 0.0,
      1.0, -4.0, 1.0,
      0.0,  1.0, 0.0
    };
    const float offset [] =
    {
      -1, -1,  0, -1,  1, -1,
      -1,  0,  0,  0,  1,  0,
      -1,  1,  0,  1,  1,  1
    };
    kernel.value(_shader.program(), value);
    kernel.offset(_shader.program(), offset);
    
    _init = true;
  }

  // Render.
  
  _shader.bind();
  
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
