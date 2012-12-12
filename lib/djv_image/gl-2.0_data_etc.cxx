/*------------------------------------------------------------------------------
 lib/djv_image/gl-2.0_data_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "gl-2.0_data_private.h"

namespace djv_image {
namespace gl_data {

//------------------------------------------------------------------------------
// Errors
//------------------------------------------------------------------------------

namespace {

const String
  error_name = "OpenGL 2.0",
  error_format = "%%\n(%%:%%)",
  error_texture = "Cannot create texture",
  error_shader_create = "Cannot create shader",
  error_shader_compile = "Cannot compile shader:\n%%",
  error_shader_program = "Cannot create shader program",
  error_shader_link = "Cannot link shader:\n%%";

#if defined(DJV_WINDOWS)
#undef ERROR // XXX
#endif

#define ERROR(in) \
  \
  Error( \
    error_name, \
    string::Format(error_format). \
      arg(in). \
      arg(__FILE__). \
      arg(__LINE__) \
  )

}

//------------------------------------------------------------------------------
// Lut
//------------------------------------------------------------------------------

Lut::Lut() :
  _size(0),
  _id(0)
{}

Lut::~Lut()
{
  del();
}

void Lut::init(const data::Info & in) throw (Error)
{
  if (in == _info)
    return;
  
  DJV_DEBUG("Lut::init");
  DJV_DEBUG_PRINT("in = " << in);
  
  _info = in;
  _size = math::to_pow2(_info.size.x);
  
  DJV_DEBUG_PRINT("size = " << _size);

  del();

  DJV_DEBUG_GL(glGenTextures(1, &_id));
  if (! _id)
    throw ERROR(error_texture);

  DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_1D, _id));
  DJV_DEBUG_GL(
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
  );
  DJV_DEBUG_GL(
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
  );
  DJV_DEBUG_GL(
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
  );
  
  DJV_DEBUG_GL(
    glTexImage1D(
      GL_TEXTURE_1D, 0, GL_RGBA,
      _size, 0,
      gl::format(_info.pixel, _info.bgr),
      gl::type(_info.pixel),
      0
    )
  );
}

void Lut::init(const data::Data & in) throw (Error)
{
  DJV_DEBUG("Lut::init");
  DJV_DEBUG_PRINT("in = " << in);
  
  init(in.info());
  bind();
  copy(in);
}

void Lut::del()
{
  if (_id)
  {
    glDeleteTextures(1, &_id);
    _id = 0;
  }
}

void Lut::copy(const data::Data & in)
{
  DJV_DEBUG("Lut::copy");
  DJV_DEBUG_PRINT("in = " << in);
  
  const data::Info & info = in.info();

  gl_data::state_unpack(in.info());
  
  DJV_DEBUG_GL(
    glTexSubImage1D(
      GL_TEXTURE_1D,
      0,
      0,
      info.size.x,
      gl::format(info.pixel, info.bgr),
      gl::type(info.pixel),
      in.data()
    )
  );
}

void Lut::bind()
{
  DJV_DEBUG("Lut::bind");
  
  DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_1D, _id));
}

const data::Info & Lut::info() const
{
  return _info;
}

GLuint Lut::id() const
{
  return _id;
}

//------------------------------------------------------------------------------
// Texture
//------------------------------------------------------------------------------

Texture::Texture() :
  _min(GL_LINEAR), _mag(GL_LINEAR),
  _id(0)
{}

Texture::~Texture()
{
  del();
}

void Texture::init(const data::Info & in, GLenum min, GLenum mag)
  throw (Error)
{
  if (in == _info && min == _min && mag == _mag)
    return;
  
  DJV_DEBUG("Texture::init");
  DJV_DEBUG_PRINT("in = " << in);
  
  _info = in;
  _min = min;
  _mag = mag;

  del();

  DJV_DEBUG_GL(glGenTextures(1, &_id));
  if (! _id)
    throw ERROR(error_texture);

  const GLenum target = gl::texture();
  DJV_DEBUG_GL(glBindTexture(target, _id));
  DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, _min));
  DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, _mag));
  
  // XXX
  
  GLenum format = GL_RGBA;
  if (
#if defined (DJV_OPENEXR)
    pixel::F16 == pixel::type(_info.pixel) ||
#endif
    pixel::F32 == pixel::type(_info.pixel)
  ) {
    if (GLEW_NV_float_buffer)
      format = GL_FLOAT_RGBA_NV;
    else if(GLEW_ARB_texture_float)
      format = GL_RGBA32F_ARB;
  }
  
  DJV_DEBUG_GL(
    glTexImage2D(
      target,
      0,
      format,
      _info.size.x,
      _info.size.y,
      0,
      gl::format(_info.pixel, _info.bgr),
      gl::type(_info.pixel),
      0
    )
  );
}

void Texture::init(const data::Data & in, GLenum min, GLenum mag)
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

void Texture::copy(const data::Data & in)
{
  DJV_DEBUG("Texture::copy");
  DJV_DEBUG_PRINT("in = " << in);
  
  const data::Info & info = in.info();

  gl_data::state_unpack(in.info());
  
  DJV_DEBUG_GL(
    glTexSubImage2D(
      gl::texture(),
      0,
      0,
      0,
      info.size.x,
      info.size.y,
      gl::format(info.pixel, info.bgr),
      gl::type(info.pixel),
      in.data()
    )
  );
}

void Texture::copy(const V2i & in)
{
  DJV_DEBUG("Texture::copy");
  DJV_DEBUG_PRINT("in = " << in);

  DJV_DEBUG_GL(
    glCopyTexSubImage2D(
      gl::texture(),
      0,
      0, 0, 0, 0, in.x, in.y
    )
  );
}

void Texture::bind()
{
  DJV_DEBUG("Texture::bind");
  
  DJV_DEBUG_GL(glBindTexture(gl::texture(), _id));
}

const data::Info & Texture::info() const
{
  return _info;
}

GLenum Texture::min() const
{
  return _min;
}

GLenum Texture::mag() const
{
  return _mag;
}

GLuint Texture::id() const
{
  return _id;
}

//------------------------------------------------------------------------------
// Shader
//------------------------------------------------------------------------------

Shader::Shader() :
  _vertex_id(0),
  _fragment_id(0),
  _program(0)
{}

Shader::~Shader()
{
  del();
}

namespace {

GLhandleARB shader_create(GLenum type) throw (Error)
{
  DJV_DEBUG("shader_create");
  
  GLhandleARB r = 0;
  
  DJV_DEBUG_GL(r = glCreateShaderObjectARB(type));
  if (! r)
    throw ERROR(error_shader_create);

  return r;
}

void shader_compile(GLhandleARB id, const String & src) throw (Error)
{
  DJV_DEBUG("shader_compile");
  DJV_DEBUG_PRINT("src = " << src);

  GLint error = 0;

  const char * src_list [] = { src.c_str() };
  DJV_DEBUG_GL(glShaderSourceARB(id, 1, src_list, 0));
  DJV_DEBUG_GL(glCompileShaderARB(id));

  char log [string::cstring_size] = "";
  glGetInfoLogARB(id, string::cstring_size, 0, log);
  
  DJV_DEBUG_PRINT("log = " << log);
  
  glGetObjectParameterivARB(id, GL_OBJECT_COMPILE_STATUS_ARB, &error);
  if (! error)
    throw ERROR(string::Format(error_shader_compile).arg(log));
}

}

void Shader::init(const String & vertex, const String & fragment) throw (Error)
{
  DJV_DEBUG("Shader::init");
  
  GLint error = 0;
  
  del();
  
  _vertex_id = shader_create(GL_VERTEX_SHADER_ARB);
  _fragment_id = shader_create(GL_FRAGMENT_SHADER_ARB);

  shader_compile(_vertex_id, vertex);
  shader_compile(_fragment_id, fragment);

  DJV_DEBUG_GL(_program = glCreateProgramObjectARB());
  if (! _program)
    throw ERROR(error_shader_program);

  DJV_DEBUG_GL(glAttachObjectARB(_program, _vertex_id));
  DJV_DEBUG_GL(glAttachObjectARB(_program, _fragment_id));
  DJV_DEBUG_GL(glLinkProgramARB(_program));

  char log [string::cstring_size] = "";
  glGetInfoLogARB(_program, string::cstring_size, 0, log);
  
  DJV_DEBUG_PRINT("log = " << log);
  
  glGetObjectParameterivARB(_program, GL_OBJECT_LINK_STATUS_ARB, &error);
  if (! error)
    throw ERROR(string::Format(error_shader_link).arg(log));
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
  if (_vertex_id)
  {
    glDeleteObjectARB(_vertex_id);
    _vertex_id = 0;
  }
  if (_fragment_id)
  {
    glDeleteObjectARB(_fragment_id);
    _fragment_id = 0;
  }
  if (_program)
  {
    glDeleteObjectARB(_program);
    _program = 0;
  }
}

GLhandleARB Shader::program() const
{
  return _program;
}

//------------------------------------------------------------------------------
// Program
//------------------------------------------------------------------------------

Program::~Program()
{}

Program::State::State() :
  _color_profile(static_cast<color_profile::PROFILE>(0)),
  _channel(static_cast<CHANNEL>(0)),
  _clamp(false),
  _init(false)
{}

Program::State::State(const Option & in) :
  _color_profile(in.color_profile.type),
  _channel(in.channel),
  _display_profile(in.display_profile),
  _filter(in.filter),
  _clamp(in.clamp),
  _init(true)
{}

bool Program::State::operator != (const State & in) const
{
  DJV_DEBUG("Program::State::operator !=");
  DJV_DEBUG_PRINT("color profile = " << (in._color_profile != _color_profile));
  DJV_DEBUG_PRINT("channel = " << (in._channel != _channel));
  DJV_DEBUG_PRINT("display profile:");
  DJV_DEBUG_PRINT("  lut = " <<
    (in._display_profile.lut.info() != _display_profile.lut.info()));
  DJV_DEBUG_PRINT("  color = " <<
    (in._display_profile.color != _display_profile.color));
  DJV_DEBUG_PRINT("  levels = " <<
    (in._display_profile.levels != _display_profile.levels));
  DJV_DEBUG_PRINT("  soft clip = " <<
    (in._display_profile.soft_clip != _display_profile.soft_clip));
  DJV_DEBUG_PRINT("filter = " << (in._filter != _filter));
  DJV_DEBUG_PRINT("clamp = " << (in._clamp != _clamp));
  DJV_DEBUG_PRINT("init = " << (! _init));

  return
    in._color_profile != _color_profile ||
    in._channel != _channel ||
    in._display_profile.lut.info() != _display_profile.lut.info() ||
    in._display_profile.color != _display_profile.color ||
    in._display_profile.levels != _display_profile.levels ||
    in._display_profile.soft_clip != _display_profile.soft_clip ||
    in._filter != _filter ||
    in._clamp != _clamp ||
    ! _init;
}

Program::State_Two_Pass::State_Two_Pass() :
  _init(false)
{}

Program::State_Two_Pass::State_Two_Pass(
  const V2i & size,
  int proxy_scale,
  const Option & option
) :
  _size(size),
  _proxy_scale(proxy_scale),
  _color_profile(option.color_profile.type),
  _channel(option.channel),
  _display_profile(option.display_profile),
  _filter(option.filter),
  _clamp(option.clamp),
  _init(true)
{}
    
bool Program::State_Two_Pass::operator != (const State_Two_Pass & in) const
{
  return
    in._size != _size ||
    in._proxy_scale != _proxy_scale ||
    in._color_profile != _color_profile ||
    in._channel != _channel ||
    in._display_profile.lut.info() != _display_profile.lut.info() ||
    in._display_profile.color != _display_profile.color ||
    in._display_profile.levels != _display_profile.levels ||
    in._display_profile.soft_clip != _display_profile.soft_clip ||
    in._filter != _filter ||
    in._clamp != _clamp ||
    ! _init;
}

//------------------------------------------------------------------------------
// quad()
//------------------------------------------------------------------------------

void quad(
  const V2i & size,
  const V2b & mirror,
  int proxy_scale
) {
  double u [] = { 0, 0 }, v [] = { 0, 0 };
  
  u[! mirror.x] = size.x;
  v[! mirror.y] = size.y;
  
  if (GL_TEXTURE_2D == gl::texture())
  {
    u[! mirror.x] /= static_cast<double>(size.x);
    v[! mirror.y] /= static_cast<double>(size.y);
  }
  
  const V2f uv [] =
  {
    V2f(u[0], v[0]),
    V2f(u[0], v[1]),
    V2f(u[1], v[1]),
    V2f(u[1], v[0])
  };
    
  glBegin(GL_QUADS);
  
  gl::draw_box(size * proxy_scale, uv);
  
  glEnd();
}

//------------------------------------------------------------------------------

}
}

