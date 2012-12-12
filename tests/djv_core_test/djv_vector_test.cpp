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

//! \file djv_vector_test.cpp

#include <djv_assert.h>
#include <djv_vector.h>

using namespace djv;

void components()
{
    V2i vec2i(1, 2);
    V2f vec2f(1.0, 2.0);

    DJV_ASSERT(1 == vec2i.x);
    DJV_ASSERT(2 == vec2i.y);
    DJV_ASSERT(1.0 == vec2f.x);
    DJV_ASSERT(2.0 == vec2f.y);

    DJV_ASSERT(V2i(V2f(1.0, 2.0)) == V2i(1, 2));
    DJV_ASSERT(V2f(V2i(1, 2)) == V2f(1.0, 2.0));
}

void abs()
{
    DJV_ASSERT(Vector_Util::abs(V2i(-1, -2)) == V2i(1, 2));
}

void swap()
{
    DJV_ASSERT(Vector_Util::swap(V2i(1, 2)) == V2i(2, 1));
}

void range()
{
    DJV_ASSERT(Vector_Util::min(V2i(1, 2), V2i(3, 4)) == V2i(1, 2));
    DJV_ASSERT(Vector_Util::max(V2i(1, 2), V2i(3, 4)) == V2i(3, 4));
    DJV_ASSERT(
        Vector_Util::clamp(V2i(-10, 10), V2i(1, 2), V2i(3, 4)) == V2i(1, 4));
}

void is_size_valid()
{
    DJV_ASSERT(
        Vector_Util::is_size_valid(V2i(1, 2)) &&
        ! Vector_Util::is_size_valid(V2i(-1, -2)));
}

void length()
{
    DJV_ASSERT(Vector_Util::length(V2i(0, 2)) == 2);
}

void normalize()
{
    DJV_ASSERT(Vector_Util::normalize(V2f(0.0, 2.0)) == V2f(0.0, 1.0));
}

void dot()
{
    DJV_ASSERT(Vector_Util::dot(V2f(1.0, 0.0), V2f(0.0, 1.0)) == 0.0);
}

void aspect()
{
    DJV_ASSERT(Vector_Util::aspect(V2i(2, 1)) == 2.0);
}

void arithmetic()
{
    V2i vec2i = V2i(1, 2);
    vec2i += V2i(1, 2);
    DJV_ASSERT(V2i(2, 4) == vec2i);
    vec2i -= V2i(1, 2);
    DJV_ASSERT(V2i(1, 2) == vec2i);
    vec2i *= V2i(1, 2);
    DJV_ASSERT(V2i(1, 4) == vec2i);
    vec2i /= V2i(1, 2);
    DJV_ASSERT(V2i(1, 2) == vec2i);

    DJV_ASSERT(V2i(1, 2) + V2i(1, 2) == V2i(2, 4));
    DJV_ASSERT(V2i(1, 2) - V2i(1, 2) == V2i(0, 0));
    DJV_ASSERT(V2i(1, 2) * V2i(1, 2) == V2i(1, 4));
    DJV_ASSERT(V2i(1, 2) / V2i(1, 2) == V2i(1, 1));

    DJV_ASSERT(-V2i(1, 2) == V2i(-1, -2));
}

void comparison()
{
    DJV_ASSERT(V2i(1, 2) == V2i(1, 2));
    DJV_ASSERT(V2i(1, 2) != V2i(0, 0));
    DJV_ASSERT(V2i(1, 2) <  V2i(2, 3));
    DJV_ASSERT(V2i(1, 2) <= V2i(1, 2));
    DJV_ASSERT(V2i(2, 3) >  V2i(1, 2));
    DJV_ASSERT(V2i(1, 2) >= V2i(1, 2));
}

void conversion()
{
    V2i vec2i = Vector_Util::convert<double, int, 2>(V2f(1.0, 2.0));
    DJV_ASSERT(vec2i == V2i(1, 2));
    vec2i = Vector_Util::ceil<double, int, 2>(V2f(0.5, 0.5));
    DJV_ASSERT(vec2i == V2i(1, 1));
    vec2i = Vector_Util::floor<double, int, 2>(V2f(0.5, 0.5));
    DJV_ASSERT(vec2i == V2i(0, 0));

    DJV_ASSERT("1 2" == String_Util::label(V2i(1, 2)));
}

int main(int argc, char ** argv)
{
    components();
    abs();
    swap();
    range();
    is_size_valid();
    length();
    normalize();
    dot();
    aspect();
    arithmetic();
    comparison();
    conversion();

    return 0;
}

