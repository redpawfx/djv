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

//! \file djv_view_input.h

#ifndef DJV_VIEW_INPUT_H
#define DJV_VIEW_INPUT_H

#include <djv_string.h>

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \struct Input
//!
//! This struct provides device input functionality.
//------------------------------------------------------------------------------

struct Input
{
    //! Mouse wheel actions.

    enum MOUSE_WHEEL
    {
        MOUSE_WHEEL_VIEW_ZOOM,
        MOUSE_WHEEL_PLAYBACK_SHUTTLE,
        MOUSE_WHEEL_PLAYBACK_SPEED,

        _MOUSE_WHEEL_SIZE
    };

    //! Get the mouse wheel action labels.

    static const List<String> & label_mouse_wheel();
};

//------------------------------------------------------------------------------

String & operator >> (String &, Input::MOUSE_WHEEL &) throw (String);

String & operator << (String &, Input::MOUSE_WHEEL);

} // djv_view

#endif // DJV_VIEW_INPUT_H

