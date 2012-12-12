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

//! \file djv_glsl_blur.cpp

#include <djv_glsl_blur.h>

#include <djv_choice_widget.h>
#include <djv_group_box.h>
#include <djv_int_edit_slider.h>
#include <djv_label.h>
#include <djv_row_layout.h>

#include <djv_gl_image.h>

namespace djv_glsl_test
{

const List<String> & Blur_Op::label_filter()
{
    static const List<String> data = List<String>() <<
        "Box" <<
        "Gaussian";
    return data;
}

Blur_Op::State::State() :
    _init(false)
{}

Blur_Op::State::State(const Value & in) :
    _value(in),
    _init (true)
{}

bool Blur_Op::State::operator != (const State & in) const
{
    return
        _value.radius != in._value.radius ||
        _value.filter != in._value.filter ||
        ! _init;
}

Blur_Op::Blur_Op()
{
    // Widgets.

    Group_Box * radius_group = new Group_Box("Radius");
    Int_Edit_Slider * radius = new Int_Edit_Slider(0, 127);

    Group_Box * filter_group = new Group_Box("Filter");
    Choice_Widget * filter = new Choice_Widget(label_filter());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(radius_group);
    Vertical_Layout * layout_v = new Vertical_Layout(radius_group);
    layout_v->add(radius);
    layout->add(filter_group);
    layout_v = new Vertical_Layout(filter_group);
    layout_v->add(filter);
    layout->add_stretch();

    // Initialize.

    radius->set(_value.radius);
    filter->set(_value.filter);

    // Callbacks.

    radius->signal.set(this, radius_callback);
    filter->signal.set(this, filter_callback);
}

namespace
{

const String src =
    "%%"
    "\n"
    "uniform sampler2DRect texture;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  gl_FragColor = kernel(texture);\n"
    "}\n";

} // namespace

void Blur_Op::render(const Image & in) throw (Error)
{
    //DJV_DEBUG("Blur_Op::render");
    //DJV_DEBUG_PRINT("in = " << in);

    // Initialize.

    begin();

    _texture.init(in);
    _render.texture_tmp.init(Pixel_Data_Info(in.size(), in.pixel()));

    const int size = _value.radius * 2 + 1;

    const State state(_value);

    if (_state != state)
    {
        _render.offscreen.init();
        _render.kernel.init(size);
        _render.shader.init(String_Format(src).arg(_render.kernel.src()));
        _render.shader.bind();

        // Kernel weights.

        Memory_Buffer<float> value(size);
        float * p = value();

        switch (_value.filter)
        {
            case BOX:
                for (int i = 0; i < size; ++i)
                {
                    p[i] = 1;
                }

                break;

            case GAUSSIAN:
            {
                const double theta = size / 6.0;
                double x = -_value.radius;

                for (int i = 0; i < size; ++i, x += 1.0)
                {
                    p[i] = static_cast<float>(
                        Math::sqrt(1.0 / (Math::sqrt(2.0 * Math::pi * theta))) *
                        Math::exp(-(x * x) / (2 * theta * theta)));
                }
            }
            break;
        }

        float sum = 0.0;

        for (int i = 0; i < size; ++i)
        {
            sum += p[i];
        }

        for (int i = 0; i < size; ++i)
        {
            //DJV_DEBUG_PRINT("i = " << p[i]);
            p[i] = p[i] / sum;
        }

        _render.kernel.value(_render.shader.program(), value());

        _state = state;
    }

    // Horizontal.

    _render.offscreen.bind();
    _render.offscreen.set(_render.texture_tmp);

    _render.shader.bind();
    const GLuint progam = _render.shader.program();

    Memory_Buffer<float> offset(size * 2);
    offset.zero();
    float * p = offset();

    for (int i = -_value.radius; i <= _value.radius; ++i, p += 2)
    {
        p[0] = static_cast<float>(i);
    }

    _render.kernel.offset(progam, offset());

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(progam, "texture"), 0);
    _texture.bind();

    const Pixel_Data_Info & info = in.info();
    Gl_Util::ortho(info.size);
    glViewport(0, 0, info.size.x, info.size.y);
    Util::quad(info);

    _render.offscreen.unbind();

    // Vertical.

    offset.zero();
    p = offset();

    for (int i = -_value.radius; i <= _value.radius; ++i, p += 2)
    {
        p[1] = static_cast<float>(i);
    }

    _render.kernel.offset(progam, offset());

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(progam, "texture"), 0);
    _render.texture_tmp.bind();

    glClear(GL_COLOR_BUFFER_BIT);
    Util::quad(Pixel_Data_Info(info.size, info.pixel));

    end();
}

void Blur_Op::radius_callback(int in)
{
    signal.emit((_value.radius = in));
}

void Blur_Op::filter_callback(int in)
{
    signal.emit((_value.filter = FILTER(in)));
}

} // djv_glsl_test
