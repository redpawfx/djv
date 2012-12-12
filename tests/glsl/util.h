#ifndef UTIL_H
#define UTIL_H

#include "base.h"

namespace glsl {

//------------------------------------------------------------------------------
// Offscreen Rendering
//------------------------------------------------------------------------------

class Texture;

class Offscreen
{
public:

  Offscreen();
  
  ~Offscreen();
  
  void init() throw (Error);
  void bind();
  static void unbind();
  
  void set(const Texture &);

private:

  GLuint _id;
};

//------------------------------------------------------------------------------
// Shader
//------------------------------------------------------------------------------

class Shader
{
public:

  Shader();
  
  ~Shader();
  
  void init(const String &) throw (Error);
  void bind();
  static void unbind();
  
  GLhandleARB program() const;

private:

  void del();

  GLhandleARB _id, _program;
};

//------------------------------------------------------------------------------
// Texture
//------------------------------------------------------------------------------

class Texture
{
public:

  Texture();
  
  ~Texture();
  
  void init(
    const djv_image::Info &,
    GLenum min = GL_LINEAR,
    GLenum mag = GL_LINEAR
  ) throw (Error);
  
  void init(
    const djv_image::Data &,
    GLenum min = GL_LINEAR,
    GLenum mag = GL_LINEAR
  ) throw (Error);
  
  void copy(const djv_image::Data &);
  void copy(const djv_image::Data &, const Box2i &);
  void copy(const V2i &);
  
  void bind();
  
  const djv_image::Info & info() const;
  GLenum target() const;
  GLenum min() const;
  GLenum mag() const;
  GLuint id() const;

private:

  void del();
  
  djv_image::Info _info;
  GLenum _target;
  GLenum _min, _mag;
  GLuint _id;
};

//------------------------------------------------------------------------------
// Filter Kernel
//------------------------------------------------------------------------------

class Kernel
{
public:

  Kernel();
  
  void init(int size);
  
  const String & src() const;
  
  //void size(int);
  void value(GLhandleARB, const float *);
  void offset(GLhandleARB, const float *);

private:

  int _size;
  String _src;
};

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

void quad(const djv_image::Info &);

//------------------------------------------------------------------------------

}

#endif
