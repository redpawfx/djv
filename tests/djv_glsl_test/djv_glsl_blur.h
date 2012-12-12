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

//! \file djv_glsl_blur.h

#ifndef DJV_GLSL_BLUR_H
#define DJV_GLSL_BLUR_H

#include <djv_glsl_op.h>

namespace djv_glsl_test
{
using namespace djv;

class Blur_Op : public Op
{
public:

    enum FILTER
    {
        BOX,
        GAUSSIAN
    };

    static const List<String> & label_filter();

    struct Value
    {
        Value() :
            radius(3),
            filter(GAUSSIAN)
        {}

        int    radius;
        FILTER filter;
    };

    struct Render
    {

        Offscreen offscreen;
        Shader    shader;
        Kernel    kernel;
        Texture   texture_tmp;
    };

    class State
    {
    public:

        State();
        State(const Value &);

        bool operator != (const State &) const;

    private:

        Value _value;
        bool  _init;
    };

    Blur_Op();

    String name() const
    {
        return "Blur";
    }

    void render(const Image &) throw (Error);

private:

    DJV_CALLBACK(Blur_Op, radius_callback, int);
    DJV_CALLBACK(Blur_Op, filter_callback, int);

    Value  _value;
    Render _render;
    State  _state;
};

} // djv_glsl_test

#endif // DJV_GLSL_BLUR_H
