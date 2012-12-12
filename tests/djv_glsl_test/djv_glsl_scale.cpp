//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_glsl_scale.cpp

#include <djv_glsl_scale.h>

#include <djv_choice_widget.h>
#include <djv_group_box.h>
#include <djv_int_edit_slider.h>
#include <djv_label.h>
#include <djv_row_layout.h>

namespace djv_glsl_test
{

const List<String> & Scale_Op::label_type()
{
    static const List<String> data = List<String>() <<
        "Default" <<
        "Custom";
    
    return data;
}

const List<String> & Scale_Op::label_filter_default()
{
    static const List<String> data = List<String>() <<
        "Nearest" <<
        "Linear";
    
    return data;
}

const List<String> & Scale_Op::label_filter_custom()
{
    static const List<String> data = List<String>() <<
        "Box" <<
        "Triangle" <<
        "Bell" <<
        "B-Spline" <<
        "Lanczos3" <<
        "Cubic" <<
        "Mitchell";
    
    return data;
}

Scale_Op::State_Default::State_Default() :
    _init(false)
{}

Scale_Op::State_Default::State_Default(const Value & in) :
    _filter_min(in.default_min),
    _filter_mag(in.default_mag),
    _init(true)
{}

bool Scale_Op::State_Default::operator != (const State_Default & in) const
{
    return
        _filter_min != in._filter_min ||
        _filter_mag != in._filter_mag ||
        ! _init;
}

Scale_Op::State_Custom::State_Custom() :
    _init(false)
{}

Scale_Op::State_Custom::State_Custom(const Value & in) :
    _size(in.size),
    _filter_min(in.custom_min),
    _filter_mag(in.custom_mag),
    _init(true)
{}

bool Scale_Op::State_Custom::operator != (const State_Custom & in) const
{
    return
        _size != in._size ||
        _filter_min != in._filter_min ||
        _filter_mag != in._filter_mag ||
        ! _init;
}

Scale_Op::Scale_Op()
{
    // Widgets.

    Group_Box * size_group = new Group_Box("Size");
    Int_Edit_Slider * width = new Int_Edit_Slider;
    Int_Edit_Slider * height = new Int_Edit_Slider;

    Group_Box * type_group = new Group_Box("Type");
    Choice_Widget * type = new Choice_Widget(label_type());

    Group_Box * filter_default_group = new Group_Box("Default Filter");
    Choice_Widget * filter_default_min =
        new Choice_Widget(label_filter_default());
    Choice_Widget * filter_default_mag =
        new Choice_Widget(label_filter_default());

    Group_Box * filter_custom_group = new Group_Box("Custom Filter");
    Choice_Widget * filter_custom_min =
        new Choice_Widget(label_filter_custom());
    Choice_Widget * filter_custom_mag =
        new Choice_Widget(label_filter_custom());

    // Layout.

    Layout_Item::group(List<Layout_Item *>() <<
        width <<
        height);

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(size_group);
    Vertical_Layout * layout_v = new Vertical_Layout(size_group);
    Horizontal_Layout * layout_h = new Horizontal_Layout(layout_v);
    layout_v->add(width);
    layout_v->add(height);

    layout->add(type_group);
    layout_v = new Vertical_Layout(type_group);
    layout_v->add(type);

    layout->add(filter_default_group);
    layout_v = new Vertical_Layout(filter_default_group);
    layout_h = new Horizontal_Layout(layout_v);
    layout_h->margin(0);
    layout_h->add(filter_default_min);
    layout_h->add(filter_default_mag);

    layout->add(filter_custom_group);
    layout_v = new Vertical_Layout(filter_custom_group);
    layout_h = new Horizontal_Layout(layout_v);
    layout_h->margin(0);
    layout_h->add(filter_custom_min);
    layout_h->add(filter_custom_mag);

    layout->add_stretch();

    // Initialize.

    width->range(1, 2048);
    height->range(1, 2048);
    width->set(_value.size.x);
    height->set(_value.size.y);
    type->set(_value.type);
    filter_default_min->set(_value.default_min);
    filter_default_mag->set(_value.default_mag);
    filter_custom_min->set(_value.custom_min);
    filter_custom_mag->set(_value.custom_mag);

    // Callbacks.

    width->signal.set(this, width_callback);
    height->signal.set(this, height_callback);
    type->signal.set(this, type_callback);
    filter_default_min->signal.set(this, default_min_callback);
    filter_default_mag->signal.set(this, default_mag_callback);
    filter_custom_min->signal.set(this, custom_min_callback);
    filter_custom_mag->signal.set(this, custom_mag_callback);
}

namespace
{

typedef double (Filter_Fnc)(const double t);

const double support_box = 0.5;

double filter_box(double t)
{
    if (t > -0.5 && t <= 0.5)
    {
        return 1.0;
    }

    return 0.0;
}

const double support_triangle = 1.0;

double filter_triangle(double t)
{
    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 1.0)
    {
        return 1.0 - t;
    }

    return 0.0;
}

const double support_bell = 1.5;

double filter_bell(double t)
{
    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 0.5)
    {
        return 0.75 - t * t;
    }

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
    if (t < 0.0)
    {
        t = -t;
    }

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
    x *= Math::pi;

    if (x != 0.0)
    {
        return Math::sin(x) / x;
    }

    return 1.0;
}

const double support_lanczos3 = 3.0;

double filter_lanczos3(double t)
{
    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 3.0)
    {
        return sinc(t) * sinc(t / 3.0);
    }

    return 0.0;
}

const double support_cubic = 1.0;

double filter_cubic(double t)
{
    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 1.0)
    {
        return (2.0 * t - 3.0) * t * t + 1.0;
    }

    return 0.0;
}

const double support_mitchell = 2.0;

double filter_mitchell(double t)
{
    const double tt = t * t;
    static const double b = 1.0 / 3.0;
    static const double c = 1.0 / 3.0;

    if (t < 0.0)
    {
        t = -t;
    }

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

Filter_Fnc * filter_fnc(Scale_Op::FILTER_CUSTOM in)
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

double filter_support(Scale_Op::FILTER_CUSTOM in)
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
    int                     input,
    int                     output,
    Scale_Op::FILTER_CUSTOM filter_min,
    Scale_Op::FILTER_CUSTOM filter_mag,
    Pixel_Data *            out)
{
    //DJV_DEBUG("_contrib");
    //DJV_DEBUG_PRINT("scale = " << input << " " << output);
    //DJV_DEBUG_PRINT("filter = " << filter_min << " " << filter_mag);

    // Filter function.

    Scale_Op::FILTER_CUSTOM filter =
        input < output ? filter_mag : filter_min;
    Filter_Fnc * fnc = filter_fnc(filter);
    const double support = filter_support(filter);
    
    //DJV_DEBUG_PRINT("support = " << support);

    //! \todo Edges?

    //const double scale =
    //  static_cast<double>(output) / static_cast<double>(input);
    
    const double scale =
        (output > 1 ? (output - 1) : 1) /
        static_cast<double>(input > 1 ? (input - 1) : 1);
    
    //DJV_DEBUG_PRINT("scale = " << scale);

    const double radius = support * ((scale >= 1.0) ? 1.0 : (1.0 / scale));
    
    //DJV_DEBUG_PRINT("radius = " << radius);

    // Initialize.

    const int width = Math::ceil(radius * 2 + 1);
    
    //DJV_DEBUG_PRINT("width = " << width);

    out->set(Pixel_Data_Info(V2i(output, width), Pixel::LA_F32));

    // Work.

    for (int i = 0; i < output; ++i)
    {
        const double center = i / scale;
        
        int left  = Math::ceil (center - radius);
        int right = Math::floor(center + radius);
        
        //DJV_DEBUG_PRINT(i << " = " << left << " " << center << " " << right);

        float sum   = 0.0;
        int   pixel = 0;
        int   j     = 0;

        for (int k = left; j < width && k <= right; ++j, ++k)
        {
            Pixel::F32_T * p =
                reinterpret_cast<Pixel::F32_T *>(out->data(i, j));

            pixel = Scale_Op::edge(k, input);

            const float x = static_cast<float>(
                (center - k) * (scale < 1.0 ? scale : 1.0));
            
            const float w = static_cast<float>(
                (scale < 1.0) ? ((*fnc)(x) * scale) : (*fnc)(x));
            
            //DJV_DEBUG_PRINT("w = " << w);

            p[0] = static_cast<float>(pixel);
            p[1] = w;

            sum += w;
        }

        for (; j < width; ++j)
        {
            Pixel::F32_T * p =
                reinterpret_cast<Pixel::F32_T *>(out->data(i, j));

            p[0] = static_cast<float>(pixel);
            p[1] = 0.0;
        }

        /*for (j = 0; j < width; ++j)
        {
          Pixel::F32_T * p =
            reinterpret_cast<Pixel::F32_T *>(out->data(i, j));

          //DJV_DEBUG_PRINT(p[0] << " = " << p[1]);
        }*/
        
        //DJV_DEBUG_PRINT("sum = " << sum);

        for (j = 0; j < width; ++j)
        {
            Pixel::F32_T * p =
                reinterpret_cast<Pixel::F32_T *>(out->data(i, j));

            p[1] = p[1] / sum;
        }
    }
}

const String src_default =
    "uniform sampler2DRect texture;\n"
    "uniform vec2 scale_input;\n"
    "uniform vec2 scale_output;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    gl_FragColor = texture2DRect(\n"
    "        texture,\n"
    "        gl_TexCoord[0].st / scale_output * scale_input);\n"
    "}\n";

const String src_x =
    "    tmp = texture2DRect(\n"
    "        contrib,\n"
    "        vec2(gl_TexCoord[0].s, float(i)));\n"
    "    value += tmp[3] * texture2DRect(\n"
    "        texture,\n"
    "        vec2(tmp[0] + 0.5, gl_TexCoord[0].t));\n";

const String src_y =
    "    tmp = texture2DRect(\n"
    "        contrib,\n"
    "        vec2(gl_TexCoord[0].t, float(i)));\n"
    "    value += tmp[3] * texture2DRect(\n"
    "        texture,\n"
    "        vec2(gl_TexCoord[0].s, tmp[0] + 0.5));\n";

const String src_custom =
    "const int scale_width = %%;\n"
    "\n"
    "vec4 scale(sampler2DRect texture, sampler2DRect contrib)\n"
    "{\n"
    "    vec4 value = vec4(0.0);\n"
    "    vec4 tmp;\n"
    "    for (int i = 0; i < scale_width; ++i)\n"
    "    {\n"
    "%%"
    "    }\n"
    "    return value;\n"
    "}\n"
    "\n"
    "uniform sampler2DRect texture;\n"
    "uniform sampler2DRect contrib;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    gl_FragColor = scale(texture, contrib);\n"
    "}\n";

} // namespace

void Scale_Op::render(const Image & in) throw (Error)
{
    //DJV_DEBUG("Scale_Op::render");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("size = " << _value.size);
    //DJV_DEBUG_PRINT("type = " << _value.type);

    // Initialize.

    Pixel_Data_Info info = in.info();

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
            //DJV_DEBUG_PRINT("init");

            _render.shader.init(src_default);

            _state_default = state;
        }
    }
    else
    {
        _texture.init(info);

        _render.texture_tmp.init(
            Pixel_Data_Info(V2i(_value.size.x, in.h()), info.pixel));

        const State_Custom state(_value);

        if (_state_custom != state)
        {
            //DJV_DEBUG_PRINT("init");

            _render.offscreen.init();

            Pixel_Data contrib;
            contrib_fnc(
                in.w(),
                _value.size.x,
                _value.custom_min,
                _value.custom_mag,
                &contrib);
            _render.contrib_x.init(contrib);
            _render.shader_x.init(String_Format(src_custom).
                                  arg(contrib.h()).
                                  arg(src_x));

            contrib_fnc(
                in.h(),
                _value.size.y,
                _value.custom_min,
                _value.custom_mag,
                &contrib);
            _render.contrib_y.init(contrib);
            _render.shader_y.init(String_Format(src_custom).
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
            static_cast<GLfloat>(_value.size.x),
            static_cast<GLfloat>(_value.size.y));

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(program, "texture"), 0);
        _texture.bind();
        _texture.copy(in);

        info = in.info();
        info.size = _value.size;
        Gl_Util::ortho(_value.size);
        glViewport(0, 0, _value.size.x, _value.size.y);
        glClear(GL_COLOR_BUFFER_BIT);
        Util::quad(info);
    }
    else
    {
        // Horizontal.

        _render.offscreen.bind();
        _render.offscreen.set(_render.texture_tmp);

        _render.shader_x.bind();

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(
            glGetUniformLocation(_render.shader_x.program(), "texture"), 0);
        _texture.bind();
        _texture.copy(in);

        glActiveTexture(GL_TEXTURE1);
        glUniform1i(
            glGetUniformLocation(_render.shader_x.program(), "contrib"), 1);
        _render.contrib_x.bind();

        info = in.info();
        info.size.x = _value.size.x;
        Gl_Util::ortho(info.size);
        glViewport(0, 0, info.size.x, info.size.y);
        glClear(GL_COLOR_BUFFER_BIT);
        Util::quad(info);

        _render.offscreen.unbind();

        // Vertical.

        _render.shader_y.bind();

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(
            glGetUniformLocation(_render.shader_y.program(), "texture"), 0);
        _render.texture_tmp.bind();

        glActiveTexture(GL_TEXTURE1);
        glUniform1i(
            glGetUniformLocation(_render.shader_y.program(), "contrib"), 1);
        _render.contrib_y.bind();

        Gl_Util::ortho(_value.size);
        glViewport(0, 0, _value.size.x, _value.size.y);
        glClear(GL_COLOR_BUFFER_BIT);
        Util::quad(Pixel_Data_Info(_value.size, in.pixel()));
    }

    end();
}

int Scale_Op::edge(int in, int size)
{
    return Math::clamp(in, 0, size - 1);
}

GLuint Scale_Op::filter_to_gl(FILTER_DEFAULT in)
{
    switch (in)
    {
        case NEAREST:
            return GL_NEAREST;

        case LINEAR:
            return GL_LINEAR;
    }

    return 0;
}

void Scale_Op::width_callback(int in)
{
    signal.emit((_value.size.x = in));
}

void Scale_Op::height_callback(int in)
{
    signal.emit((_value.size.y = in));
}

void Scale_Op::type_callback(int in)
{
    signal.emit((_value.type = TYPE(in)));
}

void Scale_Op::default_min_callback(int in)
{
    signal.emit((_value.default_min = FILTER_DEFAULT(in)));
}

void Scale_Op::default_mag_callback(int in)
{
    signal.emit((_value.default_mag = FILTER_DEFAULT(in)));
}

void Scale_Op::custom_min_callback(int in)
{
    signal.emit((_value.custom_min = FILTER_CUSTOM(in)));
}

void Scale_Op::custom_mag_callback(int in)
{
    signal.emit((_value.custom_mag = FILTER_CUSTOM(in)));
}

} // djv_glsl_test
