//#define DJV_DEBUG

#include "util.h"

#include <djv_image/gl_data.h>
#include <djv_base/box.h>

namespace glsl {

namespace {

String fbo_error(GLenum in)
{
  switch (in)
  {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT";
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      return "GL_FRAMEBUFFER_UNSUPPORTED_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT";
  }
  return "";
}

void fbo_check()
{
  DJV_DEBUG("fbo_check");

  DJV_DEBUG_PRINT("error = " <<
    fbo_error(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)));
}

}

Offscreen::Offscreen() :
  _id(0)
{}

Offscreen::~Offscreen()
{
  if (_id) glDeleteFramebuffersEXT(1, &_id);
}

void Offscreen::init() throw (Error)
{
  if (_id) return;
  
  DJV_DEBUG("Offscreen::init");

  DJV_DEBUG_GL(glGenFramebuffersEXT(1, &_id));
  if (! _id)
    throw Error("Offscreen create.");
}

void Offscreen::bind()
{
  DJV_DEBUG("Offscreen::bind");
  
  DJV_DEBUG_GL(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _id));
}

void Offscreen::unbind()
{
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void Offscreen::set(const Texture & in)
{
  DJV_DEBUG("Offscreen::set");

  fbo_check();
  
  DJV_DEBUG_GL(glFramebufferTexture2DEXT(
    GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
    in.target(), in.id(), 0));
}
  
Shader::Shader() :
  _id(0),
  _program(0)
{}

Shader::~Shader()
{
  del();
}

void Shader::init(const String & in) throw (Error)
{
  DJV_DEBUG("Shader::init");
  DJV_DEBUG_PRINT("in = " << in);
  
  GLint error = 0;

  del();
  
  DJV_DEBUG_GL(_id = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB));
  if (! _id)
    throw Error("Shader create.");
  
  const char * src_list [] = { in.c_str() };
  DJV_DEBUG_GL(glShaderSourceARB(_id, 1, src_list, 0));
  DJV_DEBUG_GL(glCompileShaderARB(_id));
  char log [string::cstring_size] = "";
  glGetInfoLogARB(_id, string::cstring_size, 0, log);
  DJV_DEBUG_PRINT("log = " << log);
  glGetObjectParameterivARB(_id, GL_OBJECT_COMPILE_STATUS_ARB, &error);
  if (! error)
    throw Error(String(string::Format("Shader compile: %%").arg(log)));

  DJV_DEBUG_GL(_program = glCreateProgramObjectARB());
  DJV_DEBUG_GL(glAttachObjectARB(_program, _id));
  DJV_DEBUG_GL(glLinkProgramARB(_program));
  glGetInfoLogARB(_program, string::cstring_size, 0, log);
  DJV_DEBUG_PRINT("log = " << log);
  glGetObjectParameterivARB(_id, GL_OBJECT_LINK_STATUS_ARB, &error);
  if (! error)
    throw Error(String(string::Format("Shader compile: %%").arg(log)));
}

void Shader::bind()
{
  DJV_DEBUG("Shader::bind");
  
  DJV_DEBUG_GL(glUseProgramObjectARB(_program));
}

void Shader::unbind()
{
  glUseProgramObjectARB(0);
}

void Shader::del()
{
  if (_id)
  {
    glDeleteObjectARB(_id);
    _id = 0;
  }
  if (_program)
  {
    glDeleteObjectARB(_program);
    _program = 0;
  }
}

GLhandleARB Shader::program() const { return _program; }

Texture::Texture() :
  _target(djv_image::gl::texture()),
  _min(GL_LINEAR),
  _mag(GL_LINEAR),
  _id(0)
{}

Texture::~Texture()
{
  del();
}

void Texture::init(const djv_image::Info & in, GLenum min, GLenum mag)
  throw (Error)
{
  if (in == _info && min == _min && mag == _mag) return;
  
  DJV_DEBUG("Texture::init");
  DJV_DEBUG_PRINT("in = " << in);
  
  _info = in;
  _min = min;
  _mag = mag;

  del();

  DJV_DEBUG_GL(glGenTextures(1, &_id));
  if (! _id)
    throw Error("Texture create.");
  
  DJV_DEBUG_GL(glBindTexture(_target, _id));
  DJV_DEBUG_GL(glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_CLAMP));
  DJV_DEBUG_GL(glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_CLAMP));
  DJV_DEBUG_GL(glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, _mag));
  DJV_DEBUG_GL(glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, _min));
  
  GLenum format = GL_RGBA;
  if (
    djv_image::pixel::F16 == djv_image::pixel::type(_info.pixel) ||
    djv_image::pixel::F32 == djv_image::pixel::type(_info.pixel)
  ) {
    if (GLEW_NV_float_buffer)
      format = GL_FLOAT_RGBA_NV;
    else if(GLEW_ARB_texture_float)
      format = GL_RGBA32F_ARB;
  }
  
  DJV_DEBUG_GL(glTexImage2D(
    _target, 0, format,
    _info.size.x, _info.size.y, 0,
    djv_image::gl::format(_info.pixel, _info.bgr),
    djv_image::gl::type(_info.pixel),
    0));
}

void Texture::init(const djv_image::Data & in, GLenum min, GLenum mag)
  throw (Error)
{
  DJV_DEBUG("Texture::init");
  DJV_DEBUG_PRINT("in = " << in);
  
  init(in.info(), min, mag);
  bind();
  copy(in);
}

void Texture::del()
{
  if (_id)
  {
    glDeleteTextures(1, &_id);
    _id = 0;
  }
}

void Texture::copy(const djv_image::Data & in)
{
  DJV_DEBUG("Texture::copy");
  DJV_DEBUG_PRINT("in = " << in);
  
  const djv_image::Info & info = in.info();

  djv_image::gl_data::state_unpack(in.info());
  
  DJV_DEBUG_GL(glTexSubImage2D(
    _target, 0,
    0, 0, info.size.x, info.size.y,
    djv_image::gl::format(info.pixel, info.bgr),
    djv_image::gl::type(info.pixel),
    in.data()));
}

void Texture::copy(const djv_image::Data & in, const Box2i & area)
{
  DJV_DEBUG("Texture::copy");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("area = " << area);
  
  const djv_image::Info & info = in.info();
  
  V2i position = area.position;
  if (info.mirror.x)
    position.x = info.size.x - area.position.x - area.size.x;
  if (info.mirror.y)
    position.y = info.size.y - area.position.y - area.size.y;

  djv_image::gl_data::state_unpack(in.info(), position);
  
  DJV_DEBUG_GL(glTexSubImage2D(
    _target, 0,
    0, 0, area.size.x, area.size.y,
    djv_image::gl::format(info.pixel, info.bgr),
    djv_image::gl::type(info.pixel),
    in.data()));
}

void Texture::copy(const V2i & in)
{
  DJV_DEBUG("Texture::copy");
  DJV_DEBUG_PRINT("in = " << in);

  DJV_DEBUG_GL(glCopyTexSubImage2D(
    _target, 0,
    0, 0, 0, 0, in.x, in.y));
}

void Texture::bind()
{
  DJV_DEBUG("Texture::bind");
  
  DJV_DEBUG_GL(glBindTexture(_target, _id));
}

const djv_image::Info & Texture::info() const { return _info; }
GLenum Texture::target() const { return _target; }
GLenum Texture::min() const { return _min; }
GLenum Texture::mag() const { return _mag; }
GLuint Texture::id() const { return _id; }

Kernel::Kernel() :
  _size(0)
{}

namespace {

const String kernel_src =
"const int kernel_max = 256;\n"
"const int kernel_size = %%;\n"
"uniform float kernel_value [kernel_max];\n"
"uniform vec2 kernel_offset [kernel_max];\n"
"\n"
"vec4 kernel(sampler2DRect texture)\n"
"{\n"
"  vec4 tmp = vec4(0.0);\n"
"  int i;\n"
"  for (i = 0; i < kernel_size; ++i)\n"
"  {\n"
"    tmp += "
"      kernel_value[i] * "
"      texture2DRect(texture, gl_TexCoord[0].st + kernel_offset[i]);\n"
"  }\n"
"  return tmp;\n"
"}\n";

}

void Kernel::init(int size)
{
  _size = size;
  
  _src = string::Format(kernel_src).arg(_size);
}

const String & Kernel::src() const { return _src; }

//void Kernel::size(int in)

void Kernel::value(GLhandleARB program, const float * in)
{
  glUniform1fv(glGetUniformLocation(program, "kernel_value"), _size, in);
}

void Kernel::offset(GLhandleARB program, const float * in)
{
  glUniform2fv(glGetUniformLocation(program, "kernel_offset"), _size, in);
}

void quad(const djv_image::Info & in)
{
  double u [] = { 0, 0 }, v [] = { 0, 0 };
  u[! in.mirror.x] = in.size.x;
  v[! in.mirror.y] = in.size.y;
  const V2f uv [] =
  {
    V2f(u[0], v[0]),
    V2f(u[0], v[1]),
    V2f(u[1], v[1]),
    V2f(u[1], v[0])
  };
      
  glBegin(GL_QUADS);
  djv_image::gl::draw_box(Box2i(in.size), uv);
  glEnd();
}

}
