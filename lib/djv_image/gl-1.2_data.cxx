/*------------------------------------------------------------------------------
 lib/djv_image/gl-1.2_data.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "gl-1.2_data_private.h"

namespace djv_image {
namespace gl_data {

//------------------------------------------------------------------------------
// color_table_info()
//------------------------------------------------------------------------------

namespace {

Info color_table_info(GLenum target, const Info & in)
{
  DJV_DEBUG("color_table_info");
  DJV_DEBUG_PRINT("in = " << in);

  Info out(in);

  const GLint format = gl::format(out.pixel, out.bgr);
  const GLint type = gl::type(out.pixel);

  GLint size = 0;
  do
  {
    const int pow2 = math::to_pow2(out.size.x);
    
    if (GLEW_ARB_imaging)
    {
      glColorTable(target, GL_RGBA, pow2, format, type, 0);
      glGetColorTableParameteriv(target, GL_COLOR_TABLE_WIDTH, &size);
    }
    else if (GLEW_SGI_color_table)
    {
      glColorTableSGI(target, GL_RGBA, pow2, format, type, 0);
      glGetColorTableParameterivSGI(target, GL_COLOR_TABLE_WIDTH_SGI, &size);
    }
    
    DJV_DEBUG_PRINT("size = " << static_cast<int>(size));
    
    if (! size)
      out.size.x = out.size.x >> 2;
  }
  while (! size && out.size.x);

  return out;
}

}

//------------------------------------------------------------------------------
// Fixed
//------------------------------------------------------------------------------

void Fixed::draw(const data::Data & in, const Option & option)
  throw (Error)
{
  DJV_DEBUG("Fixed::draw");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Initialize state.
  
  DJV_DEBUG_GL(
    glPushAttrib(
      GL_CURRENT_BIT |
      GL_ENABLE_BIT |
      GL_TRANSFORM_BIT |
      GL_PIXEL_MODE_BIT |
      GL_TEXTURE_BIT
    )
  );

  DJV_DEBUG_GL(
    glDisable(GL_DITHER)
  );

  // Color profile LUT.
  
  if (
    color_profile::LUT == option.color_profile.type &&
    vector::is_size_valid(option.color_profile.lut.size())
  ) {
  
    // LUT information.
    
    GLint max = 0;
    DJV_DEBUG_GL(glGetIntegerv(GL_MAX_PIXEL_MAP_TABLE, &max));
    
    DJV_DEBUG_PRINT("pixel map max = " << static_cast<int>(max));
    
    const int size = math::min(
      math::to_pow2(option.color_profile.lut.size().x),
      static_cast<int>(max)
    );
    
    DJV_DEBUG_PRINT("color profile lut = " << size);
    
    // Convert LUT.
    
    data::Data tmp(Info(V2i(size, 1), pixel::RGBA_F32));
    
    Option _option;
    _option.xform.scale =
      V2f(option.color_profile.lut.size()) /
      V2f(tmp.size());
    
    gl_data::copy(option.color_profile.lut, &tmp, _option);
    
    // Download LUT.

    memory::Buffer<GLfloat> data(size * 4);
    const pixel::F32_T * in_p = reinterpret_cast<pixel::F32_T *>(tmp.data());
    GLfloat * out_p = data();
    for (int i = 0; i < size; ++i, in_p += 4, out_p += 1)
    {
      DJV_DEBUG_PRINT("lut[" << i << "] = " << in_p[0]);
      out_p[size * 0] = in_p[0];
      out_p[size * 1] = in_p[1];
      out_p[size * 2] = in_p[2];
      out_p[size * 3] = in_p[3];
    }

    DJV_DEBUG_GL(glPixelTransferi(GL_MAP_COLOR, GL_TRUE));
    DJV_DEBUG_GL(glPixelMapfv(GL_PIXEL_MAP_R_TO_R, size, data() + size * 0));
    DJV_DEBUG_GL(glPixelMapfv(GL_PIXEL_MAP_G_TO_G, size, data() + size * 1));
    DJV_DEBUG_GL(glPixelMapfv(GL_PIXEL_MAP_B_TO_B, size, data() + size * 2));
    DJV_DEBUG_GL(glPixelMapfv(GL_PIXEL_MAP_A_TO_A, size, data() + size * 3));
  }
  
  // Image channel.
  
  if (option.channel > 0)
  {
    DJV_DEBUG_PRINT("channel = " << option.channel);
    
    bool mask [] =
    {
      GL_FALSE,
      GL_FALSE,
      GL_FALSE,
      GL_FALSE
    };
    
    mask[static_cast<int>(option.channel) - 1] = GL_TRUE;
    
    DJV_DEBUG_PRINT("mask = " << mask[0] << " " << mask[1] << " " << mask[2] <<
      " " << mask[3]);
    
    DJV_DEBUG_GL(glColorMask(mask[0], mask[1], mask[2], mask[3]));
  }

  // Display profile LUT.
  
  if (
    (GLEW_ARB_imaging || GLEW_SGI_color_table) &&
    vector::is_size_valid(option.display_profile.lut.size())
  ) {

    GLenum target = 0;
    if (GLEW_ARB_imaging)
      target = GL_PROXY_COLOR_TABLE;
    else if (GLEW_SGI_color_table)
      target = GL_PROXY_COLOR_TABLE_SGI;
  
    // LUT information.
    
    const Info info = color_table_info(
      target,
      Info(V2i(option.display_profile.lut.size().x, 1), pixel::RGBA_U8)
    );
    
    DJV_DEBUG_PRINT("display profile lut = " << info);
    
    // Convert LUT.
    
    data::Data tmp(info);
    
    Option _option;
    _option.xform.scale =
      V2f(option.display_profile.lut.size()) / V2f(tmp.size());
    
    gl_data::copy(option.display_profile.lut, &tmp, _option);

    // Download LUT.
    
    state_unpack(info);
    
    if (GLEW_ARB_imaging)
    {
      DJV_DEBUG_GL(
        glColorTable(
          GL_COLOR_TABLE,
          GL_RGBA,
          info.size.x,
          GL_RGBA,
          GL_UNSIGNED_BYTE,
          tmp.data()
        )
      );
    }
    else if (GLEW_SGI_color_table)
    {
      DJV_DEBUG_GL(
        glColorTableSGI(
          GL_COLOR_TABLE_SGI,
          GL_RGBA,
          info.size.x,
          GL_RGBA,
          GL_UNSIGNED_BYTE,
          tmp.data()
        )
      );
    }
    
    DJV_DEBUG_GL(glEnable(target));
  }
  
  // Display profile color matrix.
  
  if (GLEW_ARB_imaging || GL_SGI_color_matrix)
  {
    DJV_DEBUG_PRINT("color matrix");

    DJV_DEBUG_GL(glMatrixMode(GL_COLOR));
    DJV_DEBUG_GL(glLoadMatrixd(color_matrix(option.display_profile.color).e));
    DJV_DEBUG_GL(glMatrixMode(GL_MODELVIEW));
  }

  // Display profile levels.
  
  if (
    (GLEW_ARB_imaging || GLEW_SGI_color_table) &&
    (
      option.display_profile.levels != Display_Profile().levels ||
      option.display_profile.soft_clip != Display_Profile().soft_clip
    )
  ) {

    GLenum target = 0;
    if (GLEW_ARB_imaging)
      target = GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE;
    else if (GLEW_SGI_color_table)
      target = GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI;

    // Create LUT.
    
    const data::Data lut = color_lut(
      option.display_profile.levels,
      option.display_profile.soft_clip
    );
    
    const Info info = color_table_info(
      target, Info(V2i(lut.size().x, 1), pixel::RGBA_U8)
    );
    
    DJV_DEBUG_PRINT("color lut = " << info);
    
    // Convert LUT.

    data::Data tmp(info);
    
    Option _option;
    _option.xform.scale = V2f(lut.size()) / V2f(tmp.size());
    
    gl_data::copy(lut, &tmp, _option);

    // Download LUT.
    
    state_unpack(info);
    
    if (GLEW_ARB_imaging)
    {
      DJV_DEBUG_GL(
        glColorTable(
          GL_POST_COLOR_MATRIX_COLOR_TABLE,
          GL_RGBA,
          info.size.x,
          GL_RGBA,
          GL_UNSIGNED_BYTE,
          tmp.data()
        )
      );
    }
    else if (GLEW_SGI_color_table)
    {
      DJV_DEBUG_GL(
        glColorTableSGI(
          GL_POST_COLOR_MATRIX_COLOR_TABLE,
          GL_RGBA,
          info.size.x,
          GL_RGBA,
          GL_UNSIGNED_BYTE,
          tmp.data()
        )
      );
    }
    
    DJV_DEBUG_GL(glEnable(target));
  }
  
  // Draw.
  
  const Info & info = in.info();
  
  const V2b mirror(
    info.mirror.x ? (! option.xform.mirror.x) : option.xform.mirror.x,
    info.mirror.y ? (! option.xform.mirror.y) : option.xform.mirror.y
  );
  
  const int proxy_scale = data::proxy_scale(info.proxy);
  
  V2f scale = option.xform.scale * static_cast<double>(proxy_scale);
  V2f position = option.xform.position;
  if (mirror.x)
  {
    position.x += info.size.x * scale.x;
    scale.x = -scale.x;
  }
  if (mirror.y)
  {
    position.y += info.size.y * scale.y;
    scale.y = -scale.y;
  }
    
  state_unpack(info);
  
  DJV_DEBUG_GL(glRasterPos2i(0, 0));
  
  DJV_DEBUG_GL(
    glBitmap(
      0, 0,
      static_cast<GLfloat>(0.0),
      static_cast<GLfloat>(0.0),
      static_cast<GLfloat>(position.x),
      static_cast<GLfloat>(position.y),
      0
    )
  );
  
  DJV_DEBUG_GL(
    glPixelZoom(
      static_cast<GLfloat>(scale.x),
      static_cast<GLfloat>(scale.y)
    )
  );
  
  DJV_DEBUG_GL(
    glDrawPixels(
      info.size.x,
      info.size.y,
      gl::format(info.pixel, info.bgr),
      gl::type(info.pixel),
      in.data()
    )
  );
  
  state_reset();

  DJV_DEBUG_GL(glPopAttrib());
}

//------------------------------------------------------------------------------

}
}

