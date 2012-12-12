/*------------------------------------------------------------------------------
 lib/djv_image/gl-2.0_data_private.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_GL_2_0_DATA_PRIVATE_H
#define DJV_IMAGE_GL_2_0_DATA_PRIVATE_H

#include <djv_image/gl_data.h>
#include <djv_image/gl_offscreen.h>

namespace djv_image {
namespace gl_data {

//------------------------------------------------------------------------------
// LUT
//------------------------------------------------------------------------------

class Lut
{
public:

  Lut();
  
  ~Lut();
  
  void init(const data::Info &) throw (Error);
  void init(const data::Data &) throw (Error);
  void copy(const data::Data &);
  void bind();
  
  const data::Info & info() const;
  GLuint id() const;

private:

  void del();
  
  data::Info _info;
  int _size;
  GLuint _id;
};

//------------------------------------------------------------------------------
// Texture
//------------------------------------------------------------------------------

class Texture
{
public:

  Texture();
  
  ~Texture();
  
  void init(const data::Info &, GLenum = GL_LINEAR, GLenum = GL_LINEAR)
    throw (Error);
  void init(const data::Data &, GLenum = GL_LINEAR, GLenum = GL_LINEAR)
    throw (Error);
  void copy(const data::Data &);
  void copy(const V2i &);
  void bind();
  
  const data::Info & info() const;
  GLenum min() const;
  GLenum mag() const;
  GLuint id() const;

private:

  void del();
  
  data::Info _info;
  GLenum _min, _mag;
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
  
  void init(const String & vertex, const String & fragment) throw (Error);
  void bind();
  static void unbind();
  
  GLhandleARB program() const;

private:

  void del();

  GLhandleARB _vertex_id;
  GLhandleARB _fragment_id;
  GLhandleARB _program;
};

//------------------------------------------------------------------------------
// Program
//------------------------------------------------------------------------------

class Program
{
public:

  ~Program();

  void draw(const data::Data &, const Option &, bool copy) throw (Error);

private:

  class State
  {
  public:
  
    State();
    State(const Option &);
    
    bool operator != (const State &) const;
    
  private:
  
    color_profile::PROFILE _color_profile;
    Display_Profile _display_profile;
    CHANNEL _channel;
    Filter _filter;
    bool _clamp;
    bool _init;

  } _state;

  class State_Two_Pass
  {
  public:
  
    State_Two_Pass();
    State_Two_Pass(const V2i &, int proxy_scale, const Option &);
    
    bool operator != (const State_Two_Pass &) const;
    
  private:
  
    V2i _size;
    int _proxy_scale;
    color_profile::PROFILE _color_profile;
    Display_Profile _display_profile;
    CHANNEL _channel;
    Filter _filter;
    bool _clamp;
    bool _init;

  } _state_two_pass;
  
  Lut _lut_color_profile;
  Lut _lut_display_profile;

  Texture _scale_x_contrib;
  Texture _scale_y_contrib;
  Shader _scale_x_shader;
  Shader _scale_y_shader;

  Texture _texture;

  Shader _shader;

  std::auto_ptr<gl_offscreen::Buffer> _buffer;
};

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

void scale_contrib(
  int input,
  int output,
  FILTER filter,
  data::Data *
);

void quad(
  const V2i & size,
  const V2b & mirror = V2b(),
  int proxy_scale = 1
);

//------------------------------------------------------------------------------

}
}

#endif
