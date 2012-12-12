//#define DJV_DEBUG

#include "op.h"

#include <djv_image/gl.h>

namespace glsl {

Op::Op() :
  signal(this)
{}

Op::~Op()
{}

void Op::begin()
{
  DJV_DEBUG("Op::end");

  DJV_DEBUG_GL(glPushAttrib(
    GL_VIEWPORT_BIT |
    GL_CURRENT_BIT |
    GL_ENABLE_BIT |
    GL_TRANSFORM_BIT |
    GL_PIXEL_MODE_BIT |
    GL_TEXTURE_BIT
    ));
}

void Op::end()
{
  DJV_DEBUG("Op::end");

  glPopAttrib();
  Shader::unbind();
}

Op_Null::Op_Null() :
  _init(false)
{}

namespace {

const String src =
"#version 110\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"\n"
"uniform sampler2DRect texture;\n"
"\n"
"void main(void)\n"
"{\n"
"  gl_FragColor = texture2DRect(texture, gl_TexCoord[0].st);\n"
"}\n";

}

void Op_Null::render(const djv_image::Image & in) throw (Error)
{
  DJV_DEBUG("Op_Null::render");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Initialize.
  
  begin();

  _texture.init(in);

  if (! _init)
  {
    DJV_DEBUG_PRINT("init");
    
    _shader.init(src);
  }
  
  // Render.
  
  _shader.bind();
  
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(_shader.program(), "texture"), 0);
  _texture.bind();
  
  const djv_image::Info & info = in.info();
  djv_image::gl::ortho(info.size);
  glViewport(0, 0, info.size.x, info.size.y);
  quad(info);
  
  end();
}

}
