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

//! \file djv_view_info_dialog.h

#ifndef DJV_VIEW_INFO_DIALOG_H
#define DJV_VIEW_INFO_DIALOG_H

#include <djv_dialog.h>

#include <djv_image_io.h>

namespace djv
{

class Multiline_Text_Display;
class Push_Button;
class Text_Display;

} // djv

namespace djv_view
{
using namespace djv;

class View_Widget;

//------------------------------------------------------------------------------
//! \class Info_Dialog
//!
//! This class provides an information dialog.
//------------------------------------------------------------------------------

class Info_Dialog : public Dialog
{
public:

    //! Constructor.

    Info_Dialog();

    //! Destructor.

    virtual ~Info_Dialog();

    virtual void del()
    {
        Dialog::del();
    }

    //! Set the view.

    void set(const View_Widget *, const Image_Io_Info &);

    //! Update the view pick position.

    void pick(const View_Widget *, const Image_Io_Info &);

    //! Update the dialog.

    void update(const View_Widget *, const Image_Io_Info &);

    //! Remove the view.

    void del(const View_Widget *);

    virtual void show();

    virtual void update()
    {
        Dialog::update();
    }

    //! Get the global dialog.

    static Info_Dialog * global();

private:

    DJV_CALLBACK(Info_Dialog, close_callback, bool);

    void widget_update();

    const View_Widget *      _view;
    Image_Io_Info            _info;
    Text_Display *           _file_name_widget;
    Text_Display *           _layer_name_widget;
    Text_Display *           _size_widget;
    Text_Display *           _pixel_widget;
    Text_Display *           _time_widget;
    Multiline_Text_Display * _tag_widget;
    Push_Button *            _close_widget;
};

} // djv_view

#endif // DJV_VIEW_INFO_DIALOG_H

