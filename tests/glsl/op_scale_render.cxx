//#define DJV_DEBUG

#include "op_scale.h"

#include <djv_image/gl.h>

namespace glsl {
namespace op_scale {

namespace {

typedef double (Filter_Fnc)(const double t);

const double support_box = 0.5;

double filter_box(double t)
{
  if (t > -0.5 && t <= 0.5) return 1.0;
  return 0.0;
}

const double support_triangle = 1.0;

double filter_triangle(double t)
{
  if (t < 0.0) t = -t;
  if (t < 1.0) return 1.0 - t;
  return 0.0;
}

const double support_bell = 1.5;

double filter_bell(double t)
{
  if (t < 0.0) t = -t;
  if (t < 0.5) return 0.75 - t * t;
  if (t < 1.5)
  {
    t = t - 1.5;
    return 0.5 * t * t;
  }
  return 0.0;
}

const double support_bspline = 2.0;

double filter_bspline(double t)
{
  if (t < 0.0) t = -t;
  if (t < 1.0)
  {
    const double tt = t * t;
    return (0.5 * tt * t) - tt + 2.0 / 3.0;
  }
  else if (t < 2.0)
  {
    t = 2.0 - t;
    return (1.0 / 6.0) * (t * t * t);
  }
  return 0.0;
}

double sinc(double x)
{
  x *= math::pi;
  if (x != 0.0) return math::sin(x) / x;
  return 1.0;
}

const double support_lanczos3 = 3.0;

double filter_lanczos3(double t)
{
  if (t < 0.0) t = -t;
  if (t < 3.0) return sinc(t) * sinc(t / 3.0);
  return 0.0;
}

const double support_cubic = 1.0;

double filter_cubic(double t)
{
  if (t < 0.0) t = -t;
  if (t < 1.0) return (2.0 * t - 3.0) * t * t + 1.0;
  return 0.0;
}

const double support_mitchell = 2.0;

double filter_mitchell(double t)
{
  const double tt = t * t;
  static const double b = 1.0 / 3.0;
  static const double c = 1.0 / 3.0;
  if (t < 0.0) t = -t;
  if (t < 1.0)
  {
    t =
      ((12.0 - 9.0 * b - 6.0 * c) * (t * tt)) +
      ((-18.0 + 12.0 * b + 6.0 * c) * tt) +
      (6.0 - 2.0 * b);
    return t / 6.0;
  }
  else if (t < 2.0)
  {
    t =
      ((-1.0 * b - 6.0 * c) * (t * tt)) +
      ((6.0 * b + 30.0 * c) * tt) +
      ((-12.0 * b - 48.0 * c) * t) +
      (8.0 * b + 24.0 * c);
    return t / 6.0;
  }
  return 0.0;
}

Filter_Fnc * filter_fnc(FILTER_CUSTOM in)
{
  static Filter_Fnc * tmp [] =
  {
    filter_box,
    filter_triangle,
    filter_bell,
    filter_bspline,
    filter_lanczos3,
    filter_cubic,
    filter_mitchell
  };
  return tmp[in];
}

double filter_support(FILTER_CUSTOM in)
{
  static const double tmp [] =
  {
    support_box,
    support_triangle,
    support_bell,
    support_bspline,
    support_lanczos3,
    support_cubic,
    support_mitchell
  };
  return tmp[in];
}

void contrib_fnc(
  int input,
  int output,
  FILTER_CUSTOM filter_min,
  FILTER_CUSTOM filter_mag,
  djv_image::Data * out
) {
  //DJV_DEBUG("_contrib");
  //DJV_DEBUG_PRINT("scale = " << input << " " << output);
  //DJV_DEBUG_PRINT("filter = " << filter_min << " " << filter_mag);

  // Filter function.

  FILTER_CUSTOM filter =
    input < output ? filter_mag : filter_min;
  Filter_Fnc * fnc = filter_fnc(filter);
  const double support = filter_support(filter);
  //DJV_DEBUG_PRINT("support = " << support);

  // XXX Edges?

  //const double scale =
  //  static_cast<double>(output) / static_cast<double>(input);
  const double scale =
    (output > 1 ? (output - 1) : 1) /
    static_cast<double>(input > 1 ? (input - 1) : 1);
  //DJV_DEBUG_PRINT("scale = " << scale);

  const double radius = support * (scale >= 1.0) ? 1.0 : (1.0 / scale);
  //DJV_DEBUG_PRINT("radius = " << radius);

  // Initialize.

  const int width = math::ceil(radius * 2 + 1);
  //DJV_DEBUG_PRINT("width = " << width);
  
  out->set(djv_image::Info(
    V2i(output, width),
    djv_image::pixel::LA_F32));

  // Work.
  
  for (int i = 0; i < output; ++i)
  {
    const double center = i / scale;
    int left = math::ceil(center - radius);
    int right = math::floor(center + radius);
    //DJV_DEBUG_PRINT(i << " = " << left << " " << center << " " << right);
    
    float sum = 0.0;
    int pixel = 0;
    int j = 0;
    for (int k = left; j < width && k <= right; ++j, ++k)
    {
      djv_image::pixel::F32_T * p =
        reinterpret_cast<djv_image::pixel::F32_T *>(out->data(i, j));

      pixel = edge(k, input);
      
      const float x =
        static_cast<float>((center - k) * (scale < 1.0 ? scale : 1.0));
      const float w =
        static_cast<float>((scale < 1.0) ? ((*fnc)(x) * scale) : (*fnc)(x));
      //DJV_DEBUG_PRINT("w = " << w);
      
      p[0] = static_cast<float>(pixel);
      p[1] = w;
      
      sum += w;
    }
    for (; j < width; ++j)
    {
      djv_image::pixel::F32_T * p =
        reinterpret_cast<djv_image::pixel::F32_T *>(out->data(i, j));
      
      p[0] = static_cast<float>(pixel);
      p[1] = 0.0;
    }
    
    /*for (j = 0; j < width; ++j)
    {
      djv_image::pixel::F32_T * p =
        reinterpret_cast<djv_image::pixel::F32_T *>(out->data(i, j));

      DJV_DEBUG_PRINT(p[0] << " = " << p[1]);
    }*/
    //DJV_DEBUG_PRINT("sum = " << sum);

    for (j = 0; j < width; ++j)
    {
      djv_image::pixel::F32_T * p =
        reinterpret_cast<djv_image::pixel::F32_T *>(out->data(i, j));

      p[1] = p[1] / sum;
    }
  }
}

const String src_default =
"#version 110\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"\n"
"uniform sampler2DRect texture;\n"
"uniform vec2 scale_input;\n"
"uniform vec2 scale_output;\n"
"\n"
"void main(void)\n"
"{\n"
"  gl_FragColor = texture2DRect(\n"
"    texture,\n"
"    gl_TexCoord[0].st / scale_output * scale_input\n"
"    );\n"
"}\n";

const String src_x =
"const vec4 tmp = texture2DRect(\n"
"  contrib,\n"
"  vec2(gl_TexCoord[0].s, float(i))\n"
"  );\n"
"value += tmp[3] * texture2DRect(\n"
"  texture,\n"
"  vec2(tmp[0] + 0.5, gl_TexCoord[0].t)\n"
"  );\n";

const String src_y =
"const vec4 tmp = texture2DRect(\n"
"  contrib,\n"
"  vec2(gl_TexCoord[0].t, float(i))\n"
"  );\n"
"value += tmp[3] * texture2DRect(\n"
"  texture,\n"
"  vec2(gl_TexCoord[0].s, tmp[0] + 0.5)\n"
"  );\n";

const String src_custom =
"#version 110\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"\n"
"const int scale_width = %%;\n"
"\n"
"vec4 scale(sampler2DRect texture, sampler2DRect contrib)\n"
"{\n"
"  vec4 value = vec4(0.0);\n"
"  for (int i = 0; i < scale_width; ++i)\n"
"  {\n"
"%%"
"  }\n"
"  return value;\n"
"}\n"
"\n"
"uniform sampler2DRect texture;\n"
"uniform sampler2DRect contrib;\n"
"\n"
"void main(void)\n"
"{\n"
"  gl_FragColor = scale(texture, contrib);\n"
"}\n";

}

void Op::render(const djv_image::Image & in) throw (Error)
{
  DJV_DEBUG("Op::render");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("size = " << _value.size);
  DJV_DEBUG_PRINT("type = " << _value.type);
  
  // Initialize.
  
  djv_image::Info info = in.info();

  begin();
  
  if (DEFAULT == _value.type)
  {
    _texture.init(
      info,
      filter_to_gl(_value.default_min),
      filter_to_gl(_value.default_mag));
  
    const State_Default state(_value);
    if (_state_default != state)
    {
      DJV_DEBUG_PRINT("init");

      _render.shader.init(src_default);
      
      _state_default = state;
    }
  }
  else
  {
    _texture.init(info);

    _render.texture_tmp.init(
      djv_image::Info(V2i(_value.size.x, in.h()), info.pixel));
    
    const State_Custom state(_value);
    if (_state_custom != state)
    {
      DJV_DEBUG_PRINT("init");

      _render.offscreen.init();
    
      djv_image::Data contrib;
      contrib_fnc(
        in.w(),
        _value.size.x,
        _value.custom_min,
        _value.custom_mag,
        &contrib);
      _render.contrib_x.init(contrib);
      _render.shader_x.init(string::Format(src_custom).
        arg(contrib.h()).
        arg(src_x));

      contrib_fnc(
        in.h(),
        _value.size.y,
        _value.custom_min,
        _value.custom_mag,
        &contrib);
      _render.contrib_y.init(contrib);
      _render.shader_y.init(string::Format(src_custom).
        arg(contrib.h()).
        arg(src_y));

      _state_custom = state;
    }
  }
  
  // Render.
  
  if (DEFAULT == _value.type)
  {
    _render.shader.bind();
    const GLuint program = _render.shader.program();
    
    glUniform2f(glGetUniformLocation(program, "scale_input"),
      static_cast<GLfloat>(in.w()), static_cast<GLfloat>(in.h()));
    glUniform2f(glGetUniformLocation(program, "scale_output"),
      static_cast<GLfloat>(_value.size.x), static_cast<GLfloat>(_value.size.y));
    
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "texture"), 0);
    _texture.bind();
    _texture.copy(in);
  
    info = in.info();
    info.size = _value.size;
    djv_image::gl::ortho(_value.size);
    glViewport(0, 0, _value.size.x, _value.size.y);
    glClear(GL_COLOR_BUFFER_BIT);
    quad(info);
  }
  else
  {
    // Horizontal.

    _render.offscreen.bind();
    _render.offscreen.set(_render.texture_tmp);
    
    _render.shader_x.bind();
  
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(_render.shader_x.program(), "texture"), 0);
    _texture.bind();
    _texture.copy(in);

    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(_render.shader_x.program(), "contrib"), 1);
    _render.contrib_x.bind();
    
    info = in.info();
    info.size.x = _value.size.x;
    djv_image::gl::ortho(info.size);
    glViewport(0, 0, info.size.x, info.size.y);
    glClear(GL_COLOR_BUFFER_BIT);
    quad(info);
    
    _render.offscreen.unbind();
  
    // Vertical.

    _render.shader_y.bind();
    
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(_render.shader_y.program(), "texture"), 0);
    _render.texture_tmp.bind();
  
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(_render.shader_y.program(), "contrib"), 1);
    _render.contrib_y.bind();
    
    djv_image::gl::ortho(_value.size);
    glViewport(0, 0, _value.size.x, _value.size.y);
    glClear(GL_COLOR_BUFFER_BIT);
    quad(djv_image::Info(_value.size, in.pixel()));
  }

  end();
}

}
}
