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

//! \file djv_matrix_test.cpp

#include <djv_assert.h>
#include <djv_matrix.h>

using namespace djv;

void transpose()
{
    DJV_ASSERT(
        Matrix_Util::transpose(M3f(0, 1, 2, 3, 4, 5, 6, 7, 8)) ==
        M3f(0, 3, 6,
            1, 4, 7,
            2, 5, 8));
}

void translate()
{
    DJV_ASSERT(
        Matrix_Util::translate3f(V2f(1, 2)) ==
        M3f(1, 0, 0,
            0, 1, 0,
            1, 2, 1));
}

void scale()
{
    DJV_ASSERT(
        Matrix_Util::scale3f(V2f(2.0, 4.0)) ==
        M3f(2, 0, 0,
            0, 4, 0,
            0, 0, 1));
}

void rotate()
{
    /*DJV_ASSERT(
      matrix::rotate3f(90.0) ==
      M3f( 0, 1, 0,
          -1, 0, 0,
           0, 0, 1));*/
}

void comparison()
{
    DJV_ASSERT(
        M3f(0, 1, 2, 3, 4, 5, 6, 7, 8) ==
        M3f(0, 1, 2, 3, 4, 5, 6, 7, 8));
    DJV_ASSERT(
        M3f(0, 1, 2, 3, 4, 5, 6, 7, 8) !=
        M3f(8, 7, 6, 5, 4, 3, 2, 1, 0));
}

int main(int argc, char ** argv)
{
    transpose();
    translate();
    scale();
    rotate();
    comparison();

    return 0;
}

