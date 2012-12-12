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

//! \file djv_glsl_exposure.cpp

#include <djv_glsl_exposure.h>

#include <djv_float_edit_slider.h>
#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_row_layout.h>

#include <djv_gl_image.h>

namespace djv_glsl_test
{

Exposure_Op::Exposure_Op() :
    _init(false)
{
    // Widgets.

    Group_Box * value_group = new Group_Box("Value");
    Float_Edit_Slider * value = new Float_Edit_Slider(-10.0, 10.0);
    Group_Box * defog_group = new Group_Box("Defog");
    Float_Edit_Slider * defog = new Float_Edit_Slider(0.0, 0.01);
    Group_Box * knee_group = new Group_Box("Knee");
    Float_Edit_Slider * knee_low = new Float_Edit_Slider(-3.0, 3.0);
    Float_Edit_Slider * knee_high = new Float_Edit_Slider(3.5, 7.5);
    Group_Box * gamma_group = new Group_Box("Gamma");
    Float_Edit_Slider * gamma = new Float_Edit_Slider(0.1, 4.0);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(value_group);
    Vertical_Layout * layout_v = new Vertical_Layout(value_group);
    layout_v->add(value);
    layout->add(defog_group);
    layout_v = new Vertical_Layout(defog_group);
    layout_v->add(defog);
    layout->add(knee_group);
    layout_v = new Vertical_Layout(knee_group);
    layout_v->add(knee_low);
    layout_v->add(knee_high);
    layout->add(gamma_group);
    layout_v = new Vertical_Layout(gamma_group);
    layout_v->add(gamma);
    layout->add_stretch();

    // Initialize.

    value->set(_value.value);
    defog->set(_value.defog);
    knee_low->set(_value.knee_low);
    knee_high->set(_value.knee_high);
    gamma->set(_value.gamma);

    // Callbacks.

    value->signal.set(this, value_callback);
    defog->signal.set(this, defog_callback);
    knee_low->signal.set(this, knee_low_callback);
    knee_high->signal.set(this, knee_high_callback);
    gamma->signal.set(this, gamma_callback);
}

namespace
{

const String src =
    "struct Exposure { float v, d, k, f, g; };\n"
    "\n"
    "float knee(float value, float f)\n"
    "{\n"
    "  return log(value * f + 1.0) / f;\n"
    "}\n"
    "\n"
    "vec4 exposure_fnc(vec4 value, Exposure exposure)\n"
    "{\n"
    "  value[0] = max(0.0, value[0] - exposure.d) * exposure.v;\n"
    "  value[1] = max(0.0, value[1] - exposure.d) * exposure.v;\n"
    "  value[2] = max(0.0, value[2] - exposure.d) * exposure.v;\n"
    "\n"
    "  if (value[0] > exposure.k)"
    "    value[0] = exposure.k + knee(value[0] - exposure.k, exposure.f);\n"
    "  if (value[1] > exposure.k)"
    "    value[1] = exposure.k + knee(value[1] - exposure.k, exposure.f);\n"
    "  if (value[2] > exposure.k)"
    "    value[2] = exposure.k + knee(value[2] - exposure.k, exposure.f);\n"
    "\n"
    "  value[0] = pow(value[0], exposure.g) * 0.332;\n"
    "  value[1] = pow(value[1], exposure.g) * 0.332;\n"
    "  value[2] = pow(value[2], exposure.g) * 0.332;\n"
    "\n"
    "  return value;\n"
    "}\n"
    "\n"
    "uniform sampler2DRect texture;\n"
    "uniform Exposure exposure;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  gl_FragColor = texture2DRect(texture, gl_TexCoord[0].st);\n"
    "  gl_FragColor = exposure_fnc(gl_FragColor, exposure);\n"
    "}\n";

double knee(double x, double f)
{
    return Math::log(x * f + 1.0) / f;
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

        if (knee(x, f2) < y)
        {
            f1 = f2;
        }
        else
        {
            f0 = f2;
        }
    }

    return (f0 + f1) / 2.0;
}

} // namespace

void Exposure_Op::render(const Image & in) throw (Error)
{
    //DJV_DEBUG("Exposure_Op::render");
    //DJV_DEBUG_PRINT("in = " << in);

    // Initialize.

    begin();

    _texture.init(in);

    if (! _init)
    {
        _shader.init(src);

        _init = true;
    }

    // Render.

    _shader.bind();
    const GLhandleARB program = _shader.program();

    const float v = static_cast<float>(Math::pow(2.0, _value.value + 2.47393));
    const float d = static_cast<float>(_value.defog);
    const float k = static_cast<float>(Math::pow(2.0, _value.knee_low));
    const float f = static_cast<float>(
        knee_f(Math::pow(2.0, _value.knee_high) - k, Math::pow(2.0, 3.5) - k));
    const float g = static_cast<float>(1.0 / _value.gamma);
    //DJV_DEBUG_PRINT("v = " << v);
    //DJV_DEBUG_PRINT("d = " << d);
    //DJV_DEBUG_PRINT("k = " << k);
    //DJV_DEBUG_PRINT("f = " << f);
    //DJV_DEBUG_PRINT("g = " << g);
    glUniform1f(glGetUniformLocation(program, "exposure.v"), v);
    glUniform1f(glGetUniformLocation(program, "exposure.d"), d);
    glUniform1f(glGetUniformLocation(program, "exposure.k"), k);
    glUniform1f(glGetUniformLocation(program, "exposure.f"), f);
    glUniform1f(glGetUniformLocation(program, "exposure.g"), g);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(_shader.program(), "texture"), 0);
    _texture.bind();

    const Pixel_Data_Info & info = in.info();
    Gl_Util::ortho(info.size);
    glViewport(0, 0, info.size.x, info.size.y);
    glClear(GL_COLOR_BUFFER_BIT);
    Util::quad(info);

    end();
}

void Exposure_Op::value_callback(double in)
{
    signal.emit((_value.value = in));
}

void Exposure_Op::defog_callback(double in)
{
    signal.emit((_value.defog = in));
}

void Exposure_Op::knee_low_callback(double in)
{
    signal.emit((_value.knee_low = in));
}

void Exposure_Op::knee_high_callback(double in)
{
    signal.emit((_value.knee_high = in));
}

void Exposure_Op::gamma_callback(double in)
{
    signal.emit((_value.gamma = in));
}

} // djv_glsl_test
