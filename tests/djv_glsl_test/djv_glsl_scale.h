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

//! \file djv_glsl_scale.h

#ifndef DJV_GLSL_SCALE_H
#define DJV_GLSL_SCALE_H

#include <djv_glsl_op.h>

namespace djv_glsl_test
{
using namespace djv;

class Scale_Op : public Op
{
public:

    enum TYPE
    {
        DEFAULT,
        CUSTOM
    };

    static const List<String> & label_type();

    enum FILTER_DEFAULT
    {
        NEAREST,
        LINEAR
    };

    static const List<String> & label_filter_default();

    enum FILTER_CUSTOM
    {
        BOX,
        TRIANGLE,
        BELL,
        BSPLINE,
        LANCZOS3,
        CUBIC,
        MITCHELL
    };

    static const List<String> & label_filter_custom();

    struct Value
    {
        Value() :
            size       (500, 500),
            type       (CUSTOM),
            default_min(LINEAR),
            default_mag(LINEAR),
            custom_min (LANCZOS3),
            custom_mag (MITCHELL)
        {}

        V2i            size;
        TYPE           type;
        FILTER_DEFAULT default_min;
        FILTER_DEFAULT default_mag;
        FILTER_CUSTOM  custom_min;
        FILTER_CUSTOM  custom_mag;
    };

    struct Render
    {
        Offscreen offscreen;
        Texture   texture_tmp;
        Texture   contrib_x;
        Texture   contrib_y;
        Shader    shader;
        Shader    shader_x;
        Shader    shader_y;
    };

    class State_Default
    {
    public:

        State_Default();

        State_Default(const Value &);

        bool operator != (const State_Default &) const;

    private:

        FILTER_DEFAULT _filter_min;
        FILTER_DEFAULT _filter_mag;
        bool           _init;
    };

    class State_Custom
    {
    public:

        State_Custom();

        State_Custom(const Value &);

        bool operator != (const State_Custom &) const;

    private:

        V2i           _size;
        FILTER_CUSTOM _filter_min;
        FILTER_CUSTOM _filter_mag;
        bool          _init;
    };

    Scale_Op();

    String name() const
    {
        return "Scale";
    }

    void render(const Image &) throw (Error);

    static int edge(int, int size);

    static GLuint filter_to_gl(FILTER_DEFAULT);

private:

    DJV_CALLBACK(Scale_Op, width_callback, int);
    DJV_CALLBACK(Scale_Op, height_callback, int);
    DJV_CALLBACK(Scale_Op, type_callback, int);
    DJV_CALLBACK(Scale_Op, default_min_callback, int);
    DJV_CALLBACK(Scale_Op, default_mag_callback, int);
    DJV_CALLBACK(Scale_Op, custom_min_callback, int);
    DJV_CALLBACK(Scale_Op, custom_mag_callback, int);

    Value         _value;
    Render        _render;
    State_Default _state_default;
    State_Custom  _state_custom;
};

} // djv_glsl_test

#endif // DJV_GLSL_SCALE_H
