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

//! \file djv_view_view.h

#ifndef DJV_VIEW_VIEW_H
#define DJV_VIEW_VIEW_H

#include <djv_string.h>

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \struct View
//!
//! This struct provides view functionality.
//------------------------------------------------------------------------------

struct View
{
    //! What to do with the image when the view resizes.

    enum RESIZE
    {
        RESIZE_NONE,
        RESIZE_FIT,
        RESIZE_CENTER,

        _RESIZE_SIZE
    };

    //! Get the view resize labels.

    static const List<String> & label_resize();

    //! Grid overlay.

    enum GRID
    {
        GRID_NONE,
        GRID_1x1,
        GRID_10x10,
        GRID_100x100,

        _GRID_SIZE
    };

    //! Get the grid overlay labels.

    static const List<String> & label_grid();

    //! HUD options.

    enum HUD_SHOW
    {
        HUD_FILE_NAME,
        HUD_LAYER,
        HUD_SIZE,
        HUD_PROXY,
        HUD_PIXEL,
        HUD_TAG,
        HUD_FRAME,
        HUD_SPEED,

        _HUD_SHOW_SIZE
    };

    //! Get the HUD option labels.

    static const List<String> & label_hud_show();

    //! HUD background.

    enum HUD_BACKGROUND
    {
        HUD_BACKGROUND_NONE,
        HUD_BACKGROUND_SOLID,
        HUD_BACKGROUND_SHADOW,

        _HUD_BACKGROUND_SIZE
    };

    //! Get the HUD background labels.

    static const List<String> & label_hud_background();
};

//------------------------------------------------------------------------------

String & operator >> (String &, View::RESIZE &) throw (String);
String & operator >> (String &, View::GRID &) throw (String);
String & operator >> (String &, View::HUD_BACKGROUND &) throw (String);

String & operator << (String &, View::RESIZE);
String & operator << (String &, View::GRID);
String & operator << (String &, View::HUD_BACKGROUND);

} // djv_view

#endif // DJV_VIEW_VIEW_H

