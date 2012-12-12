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

//! \file djv_view_view.cpp

#include <djv_view_view.h>

#include <djv_assert.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// View
//------------------------------------------------------------------------------

const List<String> & View::label_resize()
{
    static const List<String> data = List<String>() <<
        "None" <<
        "Fit Image" <<
        "Center Image";

    DJV_ASSERT(data.size() == _RESIZE_SIZE);

    return data;
}

const List<String> & View::label_grid()
{
    static const List<String> data = List<String>() <<
        "None" <<
        "1x1" <<
        "10x10" <<
        "100x100";

    DJV_ASSERT(data.size() == _GRID_SIZE);

    return data;
}

const List<String> & View::label_hud_show()
{
    static const List<String> data = List<String>() <<
        "File Name" <<
        "Layer" <<
        "Size" <<
        "Proxy" <<
        "Pixel" <<
        "Tags" <<
        "Playback Frame" <<
        "Playback Speed";

    DJV_ASSERT(data.size() == _HUD_SHOW_SIZE);

    return data;
}

const List<String> & View::label_hud_background()
{
    static const List<String> data = List<String>() <<
        "None" <<
        "Solid" <<
        "Shadow";

    DJV_ASSERT(data.size() == _HUD_BACKGROUND_SIZE);

    return data;
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(View::RESIZE, View::label_resize())
_DJV_STRING_OPERATOR_LABEL(View::GRID, View::label_grid())
_DJV_STRING_OPERATOR_LABEL(View::HUD_BACKGROUND, View::label_hud_background())

} // djv_view

