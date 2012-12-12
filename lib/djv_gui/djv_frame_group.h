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

//! \file djv_frame_group.h

#ifndef DJV_FRAME_GROUP_H
#define DJV_FRAME_GROUP_H

#include <djv_frame.h>

namespace djv
{

class Abstract_Row_Layout;

//------------------------------------------------------------------------------
//! \class Abstract_Frame_Group
//!
//! This class provides the base functionality for frame groups.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Abstract_Frame_Group : public Widget
{
public:

    //! Constructor.

    Abstract_Frame_Group(
        bool horizontal,
        const List<Frame *> & = List<Frame *>());

    //! Destructor.

    virtual ~Abstract_Frame_Group() = 0;

    //! Set the group.

    void group(const List<Frame *> &);

    //! Get the group.

    const List<Frame *> & group() const;

    virtual void draw();

private:

    DJV_CALLBACK(Abstract_Frame_Group, draw_select_callback, const Frame *);
    DJV_CALLBACK(Abstract_Frame_Group, draw_select_off_callback, const Frame *);
    DJV_CALLBACK(Abstract_Frame_Group, draw_highlight_callback, const Frame *);
    DJV_CALLBACK(
        Abstract_Frame_Group,
        draw_highlight_off_callback,
        const Frame *);

    void widget_update();

    bool                  _horizontal;
    List<Frame *>         _group;
    const Frame *         _draw_select;
    const Frame *         _draw_highlight;
    Abstract_Row_Layout * _layout;
};

//------------------------------------------------------------------------------
//! \class Horizontal_Frame_Group
//!
//! This class provides a horizontal frame group.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Horizontal_Frame_Group : public Abstract_Frame_Group
{
public:

    //! Constructor.

    Horizontal_Frame_Group(const List<Frame *> & = List<Frame *>());
};

//------------------------------------------------------------------------------
//! \class Vertical_Frame_Group
//!
//! This class provides a vertical frame group.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Vertical_Frame_Group : public Abstract_Frame_Group
{
public:

    //! Constructor.

    Vertical_Frame_Group(const List<Frame *> & = List<Frame *>());
};

} // djv

#endif // DJV_FRAME_GROUP_H

