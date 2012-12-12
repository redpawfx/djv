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

//! \file djv_glsl_levels.cpp

#include <djv_glsl_levels.h>

#include <djv_float_edit_slider.h>
#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_row_layout.h>

#include <djv_gl_image.h>

namespace djv_glsl_test
{

Levels_Op::State::State() :
    _init(false)
{}

Levels_Op::State::State(const Value & in) :
    _soft_clip(in.soft_clip),
    _init(true)
{}

bool Levels_Op::State::operator != (const State & in) const
{
    return
        _soft_clip != in._soft_clip ||
        ! _init;
}

Levels_Op::Levels_Op()
{
    // Widgets.

    Group_Box * input_group = new Group_Box("Input");
    Float_Edit_Slider * input0 = new Float_Edit_Slider(0.0, 1.0);
    Float_Edit_Slider * input1 = new Float_Edit_Slider(0.0, 1.0);
    Group_Box * gamma_group = new Group_Box("Gamma");
    Float_Edit_Slider * gamma = new Float_Edit_Slider(0.0, 4.0);
    Group_Box * output_group = new Group_Box("Output");
    Float_Edit_Slider * output0 = new Float_Edit_Slider(0.0, 1.0);
    Float_Edit_Slider * output1 = new Float_Edit_Slider(0.0, 1.0);
    Group_Box * soft_clip_group = new Group_Box("Soft Clip");
    Float_Edit_Slider * soft_clip = new Float_Edit_Slider(0.0, 1.0);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(input_group);
    Vertical_Layout * layout_v = new Vertical_Layout(input_group);
    layout_v->add(input0);
    layout_v->add(input1);
    layout->add(gamma_group);
    layout_v = new Vertical_Layout(gamma_group);
    layout_v->add(gamma);
    layout->add(output_group);
    layout_v = new Vertical_Layout(output_group);
    layout_v->add(output0);
    layout_v->add(output1);
    layout->add(soft_clip_group);
    layout_v = new Vertical_Layout(soft_clip_group);
    layout_v->add(soft_clip);
    layout->add_stretch();

    // Initialize.

    input0->set(_value.input0);
    input1->set(_value.input1);
    gamma->set(_value.gamma);
    output0->set(_value.output0);
    output1->set(_value.output1);
    soft_clip->set(_value.soft_clip);

    // Callbacks.

    input0->signal.set(this, input0_callback);
    input1->signal.set(this, input1_callback);
    gamma->signal.set(this, gamma_callback);
    output0->signal.set(this, output0_callback);
    output1->signal.set(this, output1_callback);
    soft_clip->signal.set(this, soft_clip_callback);
}

namespace
{

const String src_levels =
    "    gl_FragColor = levels_fnc(\n"
    "        texture2DRect(texture, gl_TexCoord[0].st), levels);\n";

const String src_levels_soft_clip =
    "    gl_FragColor = soft_clip_fnc(levels_fnc(\n"
    "        texture2DRect(texture, gl_TexCoord[0].st), levels), soft_clip);\n";

const String src =
    "struct Levels\n"
    "{\n"
    "    float input0, input1;\n"
    "    float gamma;\n"
    "    float output0, output1;\n"
    "};\n"
    "\n"
    "vec4 levels_fnc(vec4 value, Levels levels)\n"
    "{\n"
    "    value[0] = "
    "        pow(max(value[0] - levels.input0, 0.0) / levels.input1, levels.gamma) * "
    "            levels.output1 + levels.output0;\n"
    "    value[1] = "
    "        pow(max(value[1] - levels.input0, 0.0) / levels.input1, levels.gamma) * "
    "            levels.output1 + levels.output0;\n"
    "    value[2] = "
    "        pow(max(value[2] - levels.input0, 0.0) / levels.input1, levels.gamma) * "
    "            levels.output1 + levels.output0;\n"
    "    return value;\n"
    "}\n"
    "\n"
    "vec4 soft_clip_fnc(vec4 value, float soft_clip)\n"
    "{\n"
    "    float tmp = 1.0 - soft_clip;\n"
    "    if (value[0] > tmp)\n"
    "        value[0] = tmp + (1.0 - exp(-(value[0] - tmp) / soft_clip)) * soft_clip;\n"
    "    if (value[1] > tmp)\n"
    "        value[1] = tmp + (1.0 - exp(-(value[1] - tmp) / soft_clip)) * soft_clip;\n"
    "    if (value[2] > tmp)\n"
    "        value[2] = tmp + (1.0 - exp(-(value[2] - tmp) / soft_clip)) * soft_clip;\n"
    "    return value;\n"
    "}\n"
    "\n"
    "uniform sampler2DRect texture;\n"
    "uniform Levels levels;\n"
    "uniform float soft_clip;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    %%\n"
    "\n"
    "}\n";

} // namespace

void Levels_Op::render(const Image & in) throw (Error)
{
    //DJV_DEBUG("Levels_Op::render");
    //DJV_DEBUG_PRINT("in = " << in);

    // Initialize.

    begin();

    _texture.init(in);

    const State state(_value);

    if (_state != state)
    {
        //DJV_DEBUG_PRINT("init");

        _render.shader.init(String_Format(src).
            arg(_value.soft_clip > 0.0 ? src_levels_soft_clip : src_levels));

        _state = state;
    }

    // Render.

    _render.shader.bind();
    const GLhandleARB program = _render.shader.program();

    glUniform1f(glGetUniformLocation(program, "levels.input0"),
        static_cast<GLfloat>(_value.input0));
    glUniform1f(glGetUniformLocation(program, "levels.input1"),
        static_cast<GLfloat>(_value.input1 - _value.input0));
    glUniform1f(glGetUniformLocation(program, "levels.gamma"),
        static_cast<GLfloat>(1.0 / _value.gamma));
    glUniform1f(glGetUniformLocation(program, "levels.output0"),
        static_cast<GLfloat>(_value.output0));
    glUniform1f(glGetUniformLocation(program, "levels.output1"),
        static_cast<GLfloat>(_value.output1 - _value.output0));
    glUniform1f(glGetUniformLocation(program, "soft_clip"),
        static_cast<GLfloat>(_value.soft_clip));

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "texture"), 0);
    _texture.bind();

    const Pixel_Data_Info & info = in.info();
    Gl_Util::ortho(info.size);
    glViewport(0, 0, info.size.x, info.size.y);
    glClear(GL_COLOR_BUFFER_BIT);
    Util::quad(info);

    end();
}

void Levels_Op::input0_callback(double in)
{
    signal.emit((_value.input0 = in));
}

void Levels_Op::input1_callback(double in)
{
    signal.emit((_value.input1 = in));
}

void Levels_Op::gamma_callback(double in)
{
    signal.emit((_value.gamma = in));
}

void Levels_Op::output0_callback(double in)
{
    signal.emit((_value.output0 = in));
}

void Levels_Op::output1_callback(double in)
{
    signal.emit((_value.output1 = in));
}

void Levels_Op::soft_clip_callback(double in)
{
    signal.emit((_value.soft_clip = in));
}

} // djv_glsl_test
