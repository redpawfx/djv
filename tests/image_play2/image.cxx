//#define DJV_DEBUG

#include "image.h"

#include <djv_image/gl_data.h>

Image::Image() :
  _id(0)
{}

Image::~Image()
{
  del();
}

const djv_image::Info & Image::info() const
{
  return _info;
}

void Image::del()
{
  if (_id)
  {
    glDeleteTextures(1, &_id);
    _id = 0;
  }
}

Image_Ppm::Image_Ppm() :
  _init(false),
  _load(false)
{}

void Image_Ppm::load(const String & in) throw (Error)
{
  DJV_DEBUG("Image_Ppm::load");
  DJV_DEBUG_PRINT("in = " << in);
  
  _io.open(in, file_io::READ);
  _io.read_ahead();
  
  char magic [] = { 0, 0, 0 };
  _io.get(magic, 2);
  
  DJV_DEBUG_PRINT("magic = " << magic);
  
  if (magic[0] != 'P')
    throw Error("ppm magic");
  if (magic[1] != '6')
    throw Error("ppm type");

  char tmp [string::cstring_size] = "";

  V2i size;
  file_io::word(_io, tmp, string::cstring_size);
  size.x = string::string_to_int<int>(tmp, string::cstring_size);
  file_io::word(_io, tmp, string::cstring_size);
  size.y = string::string_to_int<int>(tmp, string::cstring_size);
  
  int max_value = 0;
  file_io::word(_io, tmp, string::cstring_size);
  max_value = string::string_to_int<int>(tmp, string::cstring_size);
  if (max_value >= 256)
    throw Error("ppm max value");
  
  djv_image::Info info(size, djv_image::pixel::RGB_U8);
  info.mirror.y = true;
  info.endian = memory::MSB;
  
  DJV_DEBUG_PRINT("info = " << info.size);
  
  if (info != _info)
  {
    _init = true;
   
    _info = info;
  }
  
  _load = true;
}

void Image_Ppm::bind() const throw (Error)
{
  Image_Ppm * that = const_cast<Image_Ppm *>(this);
  
  DJV_DEBUG("Image_Ppm::bind");
  DJV_DEBUG_PRINT("info = " << _info.size);
  
  const GLenum target = djv_image::gl::texture();
  const GLenum format = djv_image::gl::format(_info.pixel, _info.bgr);
  const GLenum type = djv_image::gl::type(_info.pixel);
  
  if (_init)
  {
    DJV_DEBUG_PRINT("init");
    
    that->del();
    
    DJV_DEBUG_GL(glGenTextures(1, &that->_id));
    if (! _id)
      throw Error("texture");
    
    DJV_DEBUG_GL(glEnable(target));
    DJV_DEBUG_GL(glBindTexture(target, _id));
    
    DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    DJV_DEBUG_GL(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  
    DJV_DEBUG_GL(
      glTexImage2D(
        target,
        0,
        GL_RGBA,
        _info.size.x,
        _info.size.y,
        0,
        format,
        type,
        0
      )
    );
    
    that->_init = false;
  }
  
  DJV_DEBUG_GL(glEnable(target));
  DJV_DEBUG_GL(glBindTexture(target, _id));
  
  if (_load)
  {
    DJV_DEBUG_PRINT("load");
    
    djv_image::gl_data::state_unpack(_info);
    
    const uint8_t * p = _io.mmap_p();
    
    DJV_DEBUG_GL(
      glTexSubImage2D(
        target,
        0,
        0,
        0,
        _info.size.x,
        _info.size.y,
        format,
        type,
        p
      )
    );
    
    that->_load = false;
  }
}

