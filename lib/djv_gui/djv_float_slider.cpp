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

//! \file djv_float_slider.cpp

#include <djv_float_slider.h>

#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Float_Slider
//------------------------------------------------------------------------------

Float_Slider::Float_Slider() :
    signal(this)
{
    debug_path("Float_Slider");

    range(0.0, 1.0);
}

Float_Slider::~Float_Slider()
{}

int Float_Slider::handle(int in)
{
    const Box2i & box = frame_geom();

    switch (in)
    {
        case FL_PUSH:
        case FL_DRAG:
        {
            take_focus();

            const int size = handle_size();

            const int w = box.w - size - 1;

            const double v = w ?
                ((Fl::event_x() - box.x - (size - 1) / 2) /
                    static_cast<double>(w)) :
                0.0;

            set(min() + v * (max() - min()));
        }

        return 1;

        case FL_KEYBOARD:
        
            if (! Fl::event_state(FL_SHIFT | FL_CTRL | FL_ALT))
            {
                switch (Fl::event_key())
                {
                    case FL_Home:
                        min_action();
                        return 1;

                    case FL_End:
                        max_action();
                        return 1;

                    case FL_Left:
                    case FL_Down:
                        dec_small_action();
                        return 1;

                    case FL_Right:
                    case FL_Up:
                        inc_small_action();
                        return 1;

                    case FL_Page_Down:
                        dec_large_action();
                        return 1;

                    case FL_Page_Up:
                        inc_large_action();
                        return 1;
                }
            }

            break;
    }

    if (Abstract_Float_Input::handle(in, visible_focus()))
    {
        return 1;
    }

    return Abstract_Slider::handle(in);
}

void Float_Slider::draw()
{
    Abstract_Slider::draw();

    const bool inside = Abstract_Widget::inside();

    handle_draw(
        max() != min() ?
        ((get() - min()) / static_cast<double>(max() - min())) :
        0.0,
        frame_geom(),
        inside);
}

void Float_Slider::value_callback()
{
    redraw();

    signal.emit(get());
}

} // djv

