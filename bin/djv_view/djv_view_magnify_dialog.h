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

//! \file djv_view_magnify_dialog.h

#ifndef DJV_VIEW_MAGNIFY_DIALOG_H
#define DJV_VIEW_MAGNIFY_DIALOG_H

#include <djv_dialog.h>

namespace djv
{

class Choice_Widget;
class Push_Button;

} // djv

namespace djv_view
{
using namespace djv;

class Magnify_Widget;
class View_Widget;

//------------------------------------------------------------------------------
//! \class Magnify_Dialog
//!
//! This class provides a magnify dialog.
//------------------------------------------------------------------------------

class Magnify_Dialog : public Dialog
{
public:

    //! Magnification.

    enum ZOOM
    {
        ZOOM_100,
        ZOOM_200,
        ZOOM_400,
        ZOOM_800,
        ZOOM_1600,

        _ZOOM_SIZE
    };

    //! Get the magnification labels.

    static const List<String> & label_zoom();

    //! Constructor.

    Magnify_Dialog();

    //! Destructor.

    virtual ~Magnify_Dialog();

    virtual void del();

    //! Set the view.

    void set(const View_Widget *, const V2i &);

    //! Update the view pick position.

    void pick(const View_Widget *, const V2i &);

    //! Update the dialog.

    void update(const View_Widget *);

    //! Remove the view.

    void del(const View_Widget *);

    virtual void show();

    virtual void update()
    {
        Dialog::update();
    }

    //! Get the global dialog.

    static Magnify_Dialog * global();

private:

    DJV_CALLBACK(Magnify_Dialog, zoom_callback, int);
    DJV_CALLBACK(Magnify_Dialog, close_callback, bool);

    void value_update();
    void widget_update();

    const View_Widget * _view;
    V2i                 _pick;
    ZOOM                _zoom;
    Magnify_Widget *    _widget;
    Choice_Widget *     _zoom_widget;
    Push_Button *       _close_widget;
};

//------------------------------------------------------------------------------

String & operator >> (String &, Magnify_Dialog::ZOOM &) throw (String);

String & operator << (String &, Magnify_Dialog::ZOOM);

} // djv_view

#endif // DJV_VIEW_MAGNIFY_DIALOG_H

