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

//! \file djv_abstract_value_widget.cpp

#include <djv_abstract_value_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
// Abstract_Int_Input
//------------------------------------------------------------------------------

const int Abstract_Int_Input::int_min = -1000000;
const int Abstract_Int_Input::int_max = 1000000;

Abstract_Int_Input::Abstract_Int_Input(int min, int max) :
    Abstract_Value_Input<int>( min, max)
{
    inc(1, 10);
}

//------------------------------------------------------------------------------
// Abstract_Float_Input
//------------------------------------------------------------------------------

const double Abstract_Float_Input::float_min = -1000000.0;
const double Abstract_Float_Input::float_max = 1000000.0;

Abstract_Float_Input::Abstract_Float_Input(double min, double max) :
    Abstract_Value_Input<double>(min, max),
    _precision(0)
{
    inc(0.1, 1.0);
}

void Abstract_Float_Input::precision(int in)
{
    if (in == _precision)
    {
        return;
    }

    _precision = in;

    set(get());

    value_callback();
}

int Abstract_Float_Input::precision() const
{
    return _precision;
}

} // djv

