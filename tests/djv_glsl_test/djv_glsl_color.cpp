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

//! \file djv_glsl_color.cpp

#include <djv_glsl_color.h>

#include <djv_float_edit_slider.h>
#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_row_layout.h>

#include <djv_gl_image.h>
#include <djv_matrix.h>

namespace djv_glsl_test
{

Color_Op::Color_Op() :
    _init(false)
{
    // Widgets.

    Group_Box * brightness_group = new Group_Box("Brightness");
    Float_Edit_Slider * brightness = new Float_Edit_Slider(0.0, 5.0);
    Group_Box * contrast_group = new Group_Box("Contrast");
    Float_Edit_Slider * contrast = new Float_Edit_Slider(0.0, 5.0);
    Group_Box * saturation_group = new Group_Box("Saturation");
    Float_Edit_Slider * saturation = new Float_Edit_Slider(0.0, 5.0);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(brightness_group);
    Vertical_Layout * layout_v = new Vertical_Layout(brightness_group);
    layout_v->add(brightness);
    layout->add(contrast_group);
    layout_v = new Vertical_Layout(contrast_group);
    layout_v->add(contrast);
    layout->add(saturation_group);
    layout_v = new Vertical_Layout(saturation_group);
    layout_v->add(saturation);
    layout->add_stretch();

    // Initialize.

    brightness->set(_value.brightness);
    contrast->set(_value.contrast);
    saturation->set(_value.saturation);

    // Callbacks.

    brightness->signal.set(this, brightness_callback);
    contrast->signal.set(this, contrast_callback);
    saturation->signal.set(this, saturation_callback);
}

namespace
{

const String src =
    "vec4 color_fnc(vec4 value, mat4 color)\n"
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
    "uniform sampler2DRect texture;\n"
    "uniform mat4 color;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  gl_FragColor = texture2DRect(texture, gl_TexCoord[0].st);\n"
    "  gl_FragColor = color_fnc(gl_FragColor, color);\n"
    "}\n";

} // namespace

void Color_Op::render(const Image & in) throw (Error)
{
    //DJV_DEBUG("Color_Op::render");
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

    const double b = _value.brightness;
    const double c = _value.contrast;
    const double s = _value.saturation;

    const M4f value =
        Gl_Image_Color::brightness_matrix(b, b, b) *
        Gl_Image_Color::contrast_matrix(c, c, c) *
        Gl_Image_Color::saturation_matrix(s, s, s);
    glUniformMatrix4fv(
        glGetUniformLocation(_shader.program(), "color"), 1, false,
        Matrix_Util::convert<double, GLfloat>(Matrix_Util::transpose(value)).e);

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

void Color_Op::brightness_callback(double in)
{
    signal.emit((_value.brightness = in));
}

void Color_Op::contrast_callback(double in)
{
    signal.emit((_value.contrast = in));
}

void Color_Op::saturation_callback(double in)
{
    signal.emit((_value.saturation = in));
}

} // djv_glsl_test
