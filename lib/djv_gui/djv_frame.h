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

//! \file djv_frame.h

#ifndef DJV_FRAME_H
#define DJV_FRAME_H

#include <djv_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Frame
//!
//! This class provides a frame widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Frame : public Widget
{
public:

    //! Style.

    enum STYLE
    {
        STYLE_NORMAL,
        STYLE_H1,
        STYLE_H2,
        STYLE_H3,
        STYLE_V1,
        STYLE_V2,
        STYLE_V3,
        STYLE_THIN,
        STYLE_BORDER,
        STYLE_FLAT,
        STYLE_EMPTY
    };

    //! Constructor.

    Frame(STYLE = STYLE_NORMAL, bool down = false);

    //! Destructor.

    virtual ~Frame();

    //! Set the style.

    void style(STYLE);

    //! Get the style.

    STYLE style() const;

    //! Set whether the frame is down.

    void down(bool);

    //! Get whether the frame is down.

    bool down() const;

    //! Set the margin.

    void margin(int);

    //! Get the margin.

    int margin() const;

    Signal<const Frame *> select_signal;

    Signal<const Frame *> select_off_signal;

    //! This signal is emitted when the mouse enters the frame.

    Signal<const Frame *> highlight_signal;

    //! This signal is emitted when the mouse leaves the frame.

    Signal<const Frame *> highlight_off_signal;

    virtual int handle(int);

    virtual void draw();

    virtual int frame_size() const;

    virtual Box2i frame_geom() const;

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

protected:

    static Fl_Boxtype box_type(STYLE, bool down);

private:

    STYLE _style;
    bool  _down;
    int   _margin;
};

} // djv

#endif // DJV_FRAME_H

