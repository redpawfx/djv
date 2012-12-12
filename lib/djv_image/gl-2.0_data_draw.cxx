/*------------------------------------------------------------------------------
 lib/djv_image/gl-2.0_data_draw.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "gl-2.0_data_private.h"

namespace djv_image {
namespace gl_data {

namespace {

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

namespace {

void active_texture(GLenum in)
{
  DJV_DEBUG("active_texture");
  
  if (GLEW_VERSION_1_3)
  {
    DJV_DEBUG_GL(glActiveTexture(in));
  }
  else if (GLEW_ARB_multitexture)
  {
    DJV_DEBUG_GL(glActiveTextureARB(in));
  }
}

void uniform1i(GLhandleARB program, String name, int value)
{
  if (GLEW_VERSION_2_0)
  {
    glUniform1i(
      glGetUniformLocation(program, name.c_str()),
      static_cast<GLint>(value)
    );
  }
  else if (GLEW_ARB_shader_objects)
  {
    glUniform1iARB(
      glGetUniformLocationARB(program, name.c_str()),
      static_cast<GLint>(value)
    );
  }
}

void uniform1f(GLhandleARB program, String name, double value)
{
  if (GLEW_VERSION_2_0)
  {
    glUniform1f(
      glGetUniformLocation(program, name.c_str()),
      static_cast<GLfloat>(value)
    );
  }
  else if (GLEW_ARB_shader_objects)
  {
    glUniform1fARB(
      glGetUniformLocationARB(program, name.c_str()),
      static_cast<GLfloat>(value)
    );
  }
}

void uniformMatrix4f(
  GLhandleARB program,
  String name,
  const M4f & value
) {
  if (GLEW_VERSION_2_0)
  {
    glUniformMatrix4fv(
      glGetUniformLocation(program, name.c_str()),
      1,
      false,
      matrix::convert<double, GLfloat>(matrix::transpose(value)).e
    );
  }
  else if (GLEW_ARB_shader_objects)
  {
    glUniformMatrix4fvARB(
      glGetUniformLocationARB(program, name.c_str()),
      1,
      false,
      matrix::convert<double, GLfloat>(matrix::transpose(value)).e
    );
  }
}

}

//------------------------------------------------------------------------------
// Shader Source
//------------------------------------------------------------------------------

const String src_vertex =
"void main(void)\n"
"{\n"
"  gl_FrontColor = gl_Color;\n"
"  gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

const String src_fragment_header =
"#version 110\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"\n"
"struct Levels\n"
"{\n"
"  float in0, in1;\n"
"  float gamma;\n"
"  float out0, out1;\n"
"};\n"
"\n"
"struct Exposure\n"
"{\n"
"  float v, d, k, f, g;\n"
"};\n"
"\n"
"float knee(float value, float f)\n"
"{\n"
"  return log(value * f + 1.0) / f;\n"
"}\n"
"\n"
"vec4 lut(vec4 value, sampler1D lut)\n"
"{\n"
"  value[0] = texture1D(lut, value[0])[0];\n"
"  value[1] = texture1D(lut, value[1])[1];\n"
"  value[2] = texture1D(lut, value[2])[2];\n"
"  return value;\n"
"}\n"
"\n"
"vec4 gamma(vec4 value, float gamma)\n"
"{\n"
"  value[0] = pow(value[0], 1.0 / gamma);\n"
"  value[1] = pow(value[1], 1.0 / gamma);\n"
"  value[2] = pow(value[2], 1.0 / gamma);\n"
"  return value;\n"
"}\n"
"\n"
"vec4 display_profile_color(vec4 value, mat4 color)\n"
"{\n"
"  vec4 tmp;\n"
"  tmp[0] = value[0];\n"
"  tmp[1] = value[1];\n"
"  tmp[2] = value[2];\n"
"  tmp[3] = 1.0;\n"
"  tmp *= color;\n"
"  tmp[3] = value[3];\n"
"  return tmp;\n"
"}\n"
"\n"
"vec4 levels(vec4 value, Levels data)\n"
"{\n"
"  value[0] = pow(max(value[0] - data.in0, 0.0) / data.in1, data.gamma) *\n"
"    data.out1 + data.out0;\n"
"  value[1] = pow(max(value[1] - data.in0, 0.0) / data.in1, data.gamma) *\n"
"    data.out1 + data.out0;\n"
"  value[2] = pow(max(value[2] - data.in0, 0.0) / data.in1, data.gamma) *\n"
"   data.out1 + data.out0;\n"
"  return value;\n"
"}\n"
"\n"
"vec4 exposure(vec4 value, Exposure data)\n"
"{\n"
"  value[0] = max(0.0, value[0] - data.d) * data.v;\n"
"  value[1] = max(0.0, value[1] - data.d) * data.v;\n"
"  value[2] = max(0.0, value[2] - data.d) * data.v;\n"
"\n"
"  if (value[0] > data.k)\n"
"    value[0] = data.k + knee(value[0] - data.k, data.f);\n"
"  if (value[1] > data.k)\n"
"    value[1] = data.k + knee(value[1] - data.k, data.f);\n"
"  if (value[2] > data.k)\n"
"    value[2] = data.k + knee(value[2] - data.k, data.f);\n"
"\n"
"  value[0] *= 0.332;\n"
"  value[1] *= 0.332;\n"
"  value[2] *= 0.332;\n"
"  return value;\n"
"}\n"
"\n"
"vec4 soft_clip(vec4 value, float soft_clip)\n"
"{\n"
"  float tmp = 1.0 - soft_clip;\n"
"  if (value[0] > tmp)\n"
"    value[0] = tmp + (1.0 - exp(-(value[0] - tmp) / soft_clip)) * soft_clip;\n"
"  if (value[1] > tmp)\n"
"    value[1] = tmp + (1.0 - exp(-(value[1] - tmp) / soft_clip)) * soft_clip;\n"
"  if (value[2] > tmp)\n"
"    value[2] = tmp + (1.0 - exp(-(value[2] - tmp) / soft_clip)) * soft_clip;\n"
"  return value;\n"
"}\n"
"\n";

const String src_fragment_scale_x =
"int scale_size = %%;\n"
"\n"
"vec4 scale_x(sampler2DRect texture, sampler2DRect contrib)\n"
"{\n"
"  vec4 value = vec4(0.0);\n"
"  for (int i = 0; i < scale_size; ++i)\n"
"  {\n"
"    vec4 tmp = texture2DRect(\n"
"      contrib, vec2(gl_TexCoord[0].s, float(i)));\n"
"    vec2 position = vec2(tmp[0] + 0.5, gl_TexCoord[0].t);\n"
"    value += tmp[3] * %%;\n"
"  }\n"
"  return value;\n"
"}\n"
"\n";

const String src_fragment_scale_y =
"int scale_size = %%;\n"
"\n"
"vec4 scale_y(sampler2DRect texture, sampler2DRect contrib)\n"
"{\n"
"  vec4 value = vec4(0.0);\n"
"  for (int i = 0; i < scale_size; ++i)\n"
"  {\n"
"    vec4 tmp = texture2DRect(\n"
"      contrib, vec2(gl_TexCoord[0].t, float(i)));\n"
"    vec2 position = vec2(gl_TexCoord[0].s, tmp[0] + 0.5);\n"
"    value += tmp[3] * texture2DRect(texture, position);\n"
"  }\n"
"  return value;\n"
"}\n"
"\n";

const String src_fragment_main =
"void main(void)\n"
"{\n"
"  vec4 color;\n"
"%%"
"  gl_FragColor = color;\n"
"}\n";

//------------------------------------------------------------------------------
// src_fragment()
//
// Generate fragment shader source code.
//------------------------------------------------------------------------------

String src_fragment(
  color_profile::PROFILE color_profile,
  const Display_Profile & display_profile,
  CHANNEL channel,
  bool multipass_filter,
  int scale_size,
  bool scale_x,
  bool clamp
) {
  DJV_DEBUG("src_fragment");
  DJV_DEBUG_PRINT("color profile = " << color_profile);
  //DJV_DEBUG_PRINT("display_profile = " << display_profile);
  DJV_DEBUG_PRINT("channel = " << channel);
  DJV_DEBUG_PRINT("multipass filter = " << multipass_filter);
  DJV_DEBUG_PRINT("scale size = " << scale_size);
  DJV_DEBUG_PRINT("scale x = " << scale_x);
  DJV_DEBUG_PRINT("clamp = " << clamp);
  
  String header, main;
  
  // Initialize header.
  
  header = src_fragment_header;
  header += "uniform sampler2DRect in_texture;\n";
  
  // Color profile.

  String sample;
  
  switch (color_profile)
  {
    case color_profile::LUT:
      header +=
        "uniform sampler1D in_color_profile_lut;\n";
      sample =
        "lut(texture2DRect(in_texture, position), in_color_profile_lut)\n";
      break;

    case color_profile::GAMMA:
      header +=
        "uniform float in_color_profile_gamma;\n";
      sample =
        "gamma(texture2DRect(in_texture, position), in_color_profile_gamma)\n";
      break;
    
    case color_profile::EXPOSURE:
      header +=
        "uniform Exposure in_color_profile_exposure;\n";
      sample =
        "exposure(\n"
         "  texture2DRect(in_texture, position),\n"
         "  in_color_profile_exposure\n"
         ")\n";
      break;

    default:
      sample =
        "texture2DRect(in_texture, position)\n";
      break;
  }
  
  // Image filter.
  
  if (! multipass_filter)
  {
    main +=
      "vec2 position = gl_TexCoord[0].st;\n";
    main +=
      string::Format("color = %%;\n").arg(sample);
  }
  else
  {
    header +=
      "uniform sampler2DRect in_scale_contrib;\n";
    header +=
      string::Format(scale_x ? src_fragment_scale_x : src_fragment_scale_y).
        arg(scale_size).
        arg(sample);
    main += scale_x ?
      "color = scale_x(in_texture, in_scale_contrib);\n" :
      "color = scale_y(in_texture, in_scale_contrib);\n";
  }
  
  // Display profile.
  
  if (is_size_valid(display_profile.lut.size()))
  {
    header +=
      "uniform sampler1D in_display_profile_lut;\n";
    main +=
      "color = lut(color, in_display_profile_lut);\n";
  }
  
  if (display_profile.color != Display_Profile().color)
  {
    header +=
      "uniform mat4 in_display_profile_color;\n";
    main +=
      "color = display_profile_color(color, in_display_profile_color);\n";
  }
  
  if (display_profile.levels != Display_Profile().levels)
  {
    header +=
      "uniform Levels in_display_profile_levels;\n";
    main +=
      "color = levels(color, in_display_profile_levels);\n";
  }
  
  if (display_profile.soft_clip != Display_Profile().soft_clip)
  {
    header +=
      "uniform float in_display_profile_soft_clip;\n";
    main +=
      "color = soft_clip(color, in_display_profile_soft_clip);\n";
  }
  
  // Image channel.
  
  if (channel)
    main += string::Format("color = vec4(color[%%]);\n").arg(channel - 1);
  
  // Clamp pixel values.
  
  if (clamp)
    main += "color = clamp(color, vec4(0.0), vec4(1.0));\n";
  
  return
    header + "\n" +
    String(string::Format(src_fragment_main).arg(main));
}

//------------------------------------------------------------------------------
// color_profile_init()
//
// Initialize color profile.
//------------------------------------------------------------------------------

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

void color_profile_init(
  const Option & option,
  GLhandleARB program,
  Lut * color_profile
) {
  DJV_DEBUG("color_profile_init");
  
  switch (option.color_profile.type)
  {
    case color_profile::LUT:
    {
      active_texture(GL_TEXTURE2);
      uniform1i(program, "in_color_profile_lut", 2);
      color_profile->init(option.color_profile.lut);
    }
    break;
  
    case color_profile::GAMMA:
    {
      uniform1f(program, "in_color_profile_gamma", option.color_profile.gamma);
    }
    break;
  
    case color_profile::EXPOSURE:
    {
      struct Exposure
      {
        double v, d, k, f;
      } exposure;
  
      exposure.v =
        math::pow(2.0, option.color_profile.exposure.value + 2.47393);
      exposure.d = option.color_profile.exposure.defog;
      exposure.k = math::pow(2.0, option.color_profile.exposure.knee_low);
      exposure.f = knee_f(
        math::pow(2.0, option.color_profile.exposure.knee_high) - exposure.k,
        math::pow(2.0, 3.5) - exposure.k
      );
  
      DJV_DEBUG_PRINT("exposure");
      DJV_DEBUG_PRINT("  v = " << exposure.v);
      DJV_DEBUG_PRINT("  d = " << exposure.d);
      DJV_DEBUG_PRINT("  k = " << exposure.k);
      DJV_DEBUG_PRINT("  f = " << exposure.f);
  
      uniform1f(program, "in_color_profile_exposure.v", exposure.v);
      uniform1f(program, "in_color_profile_exposure.d", exposure.d);
      uniform1f(program, "in_color_profile_exposure.k", exposure.k);
      uniform1f(program, "in_color_profile_exposure.f", exposure.f);
    }
    break;
  }
}

//------------------------------------------------------------------------------
// display_profile_init()
//
// Initialize display profile.
//------------------------------------------------------------------------------

void display_profile_init(
  const Option & option,
  GLhandleARB program,
  Lut * display_profile
) {
  DJV_DEBUG("display_profile_init");

  // Color matrix.
  
  uniformMatrix4f(program, "in_display_profile_color",
    color_matrix(option.display_profile.color));
  
  // Levels in.

  uniform1f(program, "in_display_profile_levels.in0",
    option.display_profile.levels.in_low);
  
  uniform1f(program, "in_display_profile_levels.in1",
    option.display_profile.levels.in_high -
    option.display_profile.levels.in_low);
  
  // Gamma.
  
  uniform1f(program, "in_display_profile_levels.gamma",
    1.0 / option.display_profile.levels.gamma);
  
  // Levels out.
  
  uniform1f(program, "in_display_profile_levels.out0",
    option.display_profile.levels.out_low);
  
  uniform1f(program, "in_display_profile_levels.out1",
    option.display_profile.levels.out_high -
    option.display_profile.levels.out_low);
  
  // Soft-clip.
  
  uniform1f(program, "in_display_profile_soft_clip",
    option.display_profile.soft_clip);
  
  // Lookup table.
  
  if (is_size_valid(option.display_profile.lut.size()))
  {
    active_texture(GL_TEXTURE3);
    uniform1i(program, "in_display_profile_lut", 3);
    display_profile->init(option.display_profile.lut);
  }
}

}

//------------------------------------------------------------------------------
// Program::draw()
//------------------------------------------------------------------------------

void Program::draw(const data::Data & in, const Option & option, bool copy)
  throw (Error)
{  
  DJV_DEBUG("Program::draw");
  DJV_DEBUG_PRINT("in = " << in);

  const Info & info = in.info();
  
  const int proxy_scale = data::proxy_scale(info.proxy);
  
  const V2i scale = vector::ceil<double, int>(
    option.xform.scale * V2f(info.size * proxy_scale)
  );
  
  const V2i scale_tmp(scale.x, in.h());
  
  DJV_DEBUG_PRINT("scale = " << scale);
  
  // Initialize.
  //
  // XXX Use nearest filtering when not scaling.
  
  const FILTER filter =
    info.size == scale ? FILTER_NEAREST :
    (
      vector::area(scale) < vector::area(info.size) ?
        option.filter.min :
        option.filter.mag
    );
  
  DJV_DEBUG_PRINT("filter min = " << option.filter.min);
  DJV_DEBUG_PRINT("filter mag = " << option.filter.mag);
  DJV_DEBUG_PRINT("filter = " << filter);

  switch (filter)
  {
    case FILTER_NEAREST:
    case FILTER_LINEAR:
    {
      if (copy)
        _texture.init(
          in,
          filter_to_gl(filter),
          filter_to_gl(filter)
        );

      const State state(option);
      if (_state != state)
      {
        DJV_DEBUG_PRINT("init");
      
        // Initialize shader.
      
        _shader.init(
          src_vertex,
          src_fragment(
            option.color_profile.type,
            option.display_profile,
            option.channel,
            false,
            0,
            false,
            option.clamp
          )
        );
      
        // Initialize state.
      
        _state = state;
      }
    }
    break;
  
    case FILTER_BOX:
    case FILTER_TRIANGLE:
    case FILTER_BELL:
    case FILTER_BSPLINE:
    case FILTER_LANCZOS3:
    case FILTER_CUBIC:
    case FILTER_MITCHELL:
    {
      _texture.init(
        in.info(),
        GL_NEAREST,
        GL_NEAREST
      );

      const State_Two_Pass state(scale, proxy_scale, option);
      
      if (_state_two_pass != state)
      {
        DJV_DEBUG_PRINT("init two-pass");
      
        // Initialize offscreen buffer.

        _buffer = std::auto_ptr<gl_offscreen::Buffer>(
          gl_offscreen::global()->buffer_create()
        );
        
        data::Data contrib;
        scale_contrib(
          in.w(),
          scale.x,
          filter,
          &contrib
        );
      
        // Initialize horizontal pass.
      
        _scale_x_contrib.init(
          contrib,
          GL_NEAREST,
          GL_NEAREST
        );
      
        _scale_x_shader.init(
          src_vertex,
          src_fragment(
            option.color_profile.type,
            Display_Profile(),
            static_cast<CHANNEL>(0),
            true,
            contrib.h(),
            true,
            false
          )
        );
      
        // Initialize vertical pass.

        scale_contrib(
          in.h(),
          scale.y,
          filter,
          &contrib
        );
      
        _scale_y_contrib.init(
          contrib,
          GL_NEAREST,
          GL_NEAREST
        );
      
        _scale_y_shader.init(
          src_vertex,
          src_fragment(
            static_cast<color_profile::PROFILE>(0),
            option.display_profile,
            option.channel,
            true,
            contrib.h(),
            false,
            option.clamp
          )
        );
      
        // Initialize state.
        
        _state_two_pass = state;
      }
    }
    break;
  }
  
  // Render.
  
  const V2b mirror(
    info.mirror.x ? (! option.xform.mirror.x) : option.xform.mirror.x,
    info.mirror.y ? (! option.xform.mirror.y) : option.xform.mirror.y
  );
  
  DJV_DEBUG_PRINT("mirror = " << mirror);

  switch (filter)
  {
    case FILTER_NEAREST:
    case FILTER_LINEAR:
    {
      _shader.bind();
    
      // Initialize color and display profiles.

      color_profile_init(option, _shader.program(), &_lut_color_profile);
      display_profile_init(option, _shader.program(), &_lut_display_profile);
    
      // Draw.

      active_texture(GL_TEXTURE0);
    
      uniform1i(_shader.program(), "in_texture", 0);
      _texture.bind();

      DJV_DEBUG_GL(glPushMatrix());
      DJV_DEBUG_GL(
        glLoadMatrixd(matrix::matrix4(xform_matrix(option.xform)).e)
      );
    
      quad(info.size, mirror, proxy_scale);

      DJV_DEBUG_GL(glPopMatrix());
    
      _shader.unbind();
    }
    break;
  
    case FILTER_BOX:
    case FILTER_TRIANGLE:
    case FILTER_BELL:
    case FILTER_BSPLINE:
    case FILTER_LANCZOS3:
    case FILTER_CUBIC:
    case FILTER_MITCHELL:
    {
      // Horizontal pass.
  
      _buffer->init(Info(scale_tmp, in.pixel()));
      _buffer->bind();
  
      _scale_x_shader.bind();
    
      // Initialize color profile.
    
      color_profile_init(
        option,
        _scale_x_shader.program(),
        &_lut_color_profile
      );
    
      // Draw.
    
      active_texture(GL_TEXTURE0);

      uniform1i(_scale_x_shader.program(), "in_texture", 0);
      _texture.bind();
      _texture.copy(in);

      active_texture(GL_TEXTURE1);

      uniform1i(_scale_x_shader.program(), "in_scale_contrib", 1);
      _scale_x_contrib.bind();

      glPushAttrib(GL_TRANSFORM_BIT | GL_VIEWPORT_BIT);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
  
      gl::ortho(scale_tmp);
      glViewport(0, 0, scale_tmp.x, scale_tmp.y);
      quad(scale_tmp, mirror);

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
      glPopAttrib();

      _scale_x_shader.unbind();
    
      _buffer->unbind();

      // Vertical pass.
  
      _scale_y_shader.bind();
    
      // Iniitalize display profile.
    
      display_profile_init(
        option,
        _scale_y_shader.program(),
        &_lut_display_profile
      );

      // Draw.

      active_texture(GL_TEXTURE0);

      uniform1i(_scale_y_shader.program(), "in_texture", 0);
      _buffer->texture_bind();
  
      active_texture(GL_TEXTURE1);

      uniform1i(_scale_y_shader.program(), "in_scale_contrib", 1);
      _scale_y_contrib.bind();

      Xform xform = option.xform;
      xform.scale = V2f(1.0);
      const M3f mtx = xform_matrix(xform);
      DJV_DEBUG_GL(glPushMatrix());
      DJV_DEBUG_GL(glLoadMatrixd(matrix::matrix4(mtx).e));  
      quad(scale);
      DJV_DEBUG_GL(glPopMatrix());
    
      _scale_y_shader.unbind();
    }
    break;
  }
}

//------------------------------------------------------------------------------

}
}

