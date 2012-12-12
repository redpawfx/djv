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

//! \file djv_view_histogram_dialog.h

#ifndef DJV_VIEW_HISTOGRAM_DIALOG_H
#define DJV_VIEW_HISTOGRAM_DIALOG_H

#include <djv_dialog.h>
#include <djv_frame.h>

#include <djv_gl_image.h>

namespace djv
{

class Choice_Widget;
class Push_Button;
class Text_Display;

} // djv

namespace djv_view
{
using namespace djv;

class Histogram_Widget;
class View_Widget;

//------------------------------------------------------------------------------
//! \class Histogram_Dialog
//!
//! This class provides a histogram dialog.
//------------------------------------------------------------------------------

class Histogram_Dialog : public Dialog
{
public:

    //! Constructor.

    Histogram_Dialog();

    //! Destructor.

    virtual ~Histogram_Dialog();

    virtual void del();

    //! Set the view.

    void set(const View_Widget *);

    //! Update the view pick position.

    void pick(const View_Widget *);

    //! Update the histogram.

    void update(const View_Widget *);

    //! Remove the view.

    void del(const View_Widget *);

    virtual void show();

    virtual void update()
    {
        Dialog::update();
    }

    //! Get the global dialog.

    static Histogram_Dialog * global();

private:

    DJV_CALLBACK(Histogram_Dialog, size_callback, int);
    DJV_CALLBACK(Histogram_Dialog, close_callback, bool);

    void widget_update();

    const View_Widget * _view;
    Gl_Image::HISTOGRAM _size;
    Pixel_Data          _histogram;
    Color               _min;
    Color               _max;
    Histogram_Widget *  _widget;
    Choice_Widget *     _size_widget;
    Text_Display *      _min_widget;
    Text_Display *      _max_widget;
    Push_Button *       _close_widget;
};

} // djv_view

#endif // DJV_VIEW_HISTOGRAM_DIALOG_H

