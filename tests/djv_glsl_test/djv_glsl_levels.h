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

//! \file djv_glsl_levels.h

#ifndef DJV_GLSL_LEVELS_H
#define DJV_GLSL_LEVELS_H

#include <djv_glsl_op.h>

namespace djv_glsl_test
{
using namespace djv;

class Levels_Op : public Op
{
public:

    struct Value
    {
        Value() :
            input0   (0.0),
            input1   (1.0),
            gamma    (1.0),
            output0  (0.0),
            output1  (1.0),
            soft_clip(0.0)
        {}

        double input0;
        double input1;
        double gamma;
        double output0;
        double output1;
        double soft_clip;
    };

    struct Render
    {
        Shader shader;
    };

    class State
    {
    public:

        State();
        State(const Value &);

        bool operator != (const State &) const;

    private:

        double _soft_clip;
        bool   _init;
    };

    Levels_Op();

    String name() const
    {
        return "Levels";
    }

    void render(const Image &) throw (Error);

private:

    DJV_CALLBACK(Levels_Op, input0_callback, double);
    DJV_CALLBACK(Levels_Op, input1_callback, double);
    DJV_CALLBACK(Levels_Op, gamma_callback, double);
    DJV_CALLBACK(Levels_Op, output0_callback, double);
    DJV_CALLBACK(Levels_Op, output1_callback, double);
    DJV_CALLBACK(Levels_Op, soft_clip_callback, double);

    Value  _value;
    Render _render;
    State  _state;
};

} // djv_glsl_test

#endif // DJV_GLSL_LEVELS_H
