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

//! \file djv_box_test.cpp

#include <djv_assert.h>
#include <djv_box.h>

using namespace djv;

void components()
{
    Box2i tmp(1, 2);
    DJV_ASSERT(1 == tmp.w);
    DJV_ASSERT(2 == tmp.h);

    tmp = Box2i(0, 10, 10, 20);
    DJV_ASSERT(0 == tmp.x);
    DJV_ASSERT(10 == tmp.y);
    DJV_ASSERT(10 == tmp.w);
    DJV_ASSERT(20 == tmp.h);
}

void corners()
{
    DJV_ASSERT(
        Box_Util::corners(V2i(0, 10), V2i(9, 29)) == Box2i(0, 10, 10, 20));
    DJV_ASSERT(
        Box_Util::corners(V2f(0.0, 10.0), V2f(10.0, 30.0)) ==
        Box2f(0.0, 10.0, 10.0, 20.0));
}

void swap()
{
    DJV_ASSERT(Box_Util::swap(Box2i(0, 10, 10, 20)) == Box2i(10, 0, 20, 10));
}

void intersect()
{
    DJV_ASSERT(
        Box_Util::intersect(Box2i(0, 5, 10, 15), Box2i(5, 10, 15, 20)) ==
        Box2i(5, 10, 5, 10));
    DJV_ASSERT(Box_Util::intersect(Box2i(0, 5, 10, 15), V2i(5, 10)));
}

void bound()
{
    DJV_ASSERT(
        Box_Util::bound(Box2i(0, 5, 5, 10), Box2i(5, 10, 10, 15)) ==
        Box2i(0, 5, 15, 20));
}

void expand()
{
    DJV_ASSERT(
        Box_Util::expand(Box2i(0, 5, 5, 10), V2i(9, 19)) ==
        Box2i(0, 5, 10, 15));
}

void border()
{
    DJV_ASSERT(
        Box_Util::border(Box2i(0, 5, 5, 10), V2i(5, 5)) ==
        Box2i(-5, 0, 15, 20));
}

void is_size_valid()
{
    DJV_ASSERT(Box_Util::is_size_valid(Box2i(0, 5, 10, 15)));
}

void arithmetic()
{
    DJV_ASSERT(Box2i(0, 1, 2, 3) + 1 == Box2i(1, 2, 3, 4));
    DJV_ASSERT(Box2i(0, 1, 2, 3) - 1 == Box2i(-1, 0, 1, 2));
    DJV_ASSERT(Box2i(0, 1, 2, 3) * 2 == Box2i(0, 2, 4, 6));
    DJV_ASSERT(Box2i(0, 1, 2, 3) / 2 == Box2i(0, 0, 1, 1));
}

void comparison()
{
    DJV_ASSERT(Box2i(0, 1, 2, 3) == Box2i(0, 1, 2, 3));
    DJV_ASSERT(Box2i(0, 1, 2, 3) != Box2i(3, 2, 1, 0));
}

void conversion()
{
    const Box2i tmp =
        Box_Util::convert<double, int, 2>(Box2f(0.0, 1.0, 2.0, 3.0));
    DJV_ASSERT(tmp == Box2i(0, 1, 2, 3));

    DJV_ASSERT("1 2 3 4" == String_Util::label(Box2i(1, 2, 3, 4)));
}

int main(int argc, char ** argv)
{
    components();
    corners();
    swap();
    intersect();
    bound();
    expand();
    border();
    is_size_valid();
    arithmetic();
    comparison();
    conversion();
    
    return 0;
}

