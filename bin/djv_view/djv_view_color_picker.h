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

//! \file djv_view_color_picker.h

#ifndef DJV_VIEW_COLOR_PICKER_H
#define DJV_VIEW_COLOR_PICKER_H

#include <djv_dialog.h>

#include <djv_gl_image.h>

namespace djv
{

class Color_Swatch;
class Color_Widget;
class Choice_Widget;
class Gl_Offscreen_Buffer;
class Push_Button;
class Tool_Button;

} // djv

namespace djv_view
{
using namespace djv;

class View_Widget;

//------------------------------------------------------------------------------
//! \class Color_Picker
//!
//! This class provides a color picker dialog.
//------------------------------------------------------------------------------

class Color_Picker : public Dialog
{
public:

    //! Color picker sizes.

    enum SIZE
    {
        SIZE_1,
        SIZE_3,
        SIZE_5,
        SIZE_7,
        SIZE_9,
        SIZE_51,
        SIZE_101,

        _SIZE_SIZE
    };

    //! Get color picker size labels.

    static const List<String> & label_size();

    //! Get a color picker size.

    int size(SIZE);

    //! Constructor.

    Color_Picker();

    //! Destructor.

    virtual ~Color_Picker();

    void del();

    //! Set the view.

    void set(const View_Widget *, const V2i &);

    //! Set the view pick position.

    void pick(const View_Widget *, const V2i &);

    //! Update the color picker.

    void update(const View_Widget *);

    //! Remove the view.

    void del(const View_Widget *);

    virtual void show();

    virtual void update()
    {
        Dialog::update();
    }

    //! Get the global dialog.

    static Color_Picker * global();

private:

    DJV_CALLBACK(Color_Picker, widget_callback, const Color &);
    DJV_CALLBACK(Color_Picker, size_callback, int);
    DJV_CALLBACK(Color_Picker, color_profile_callback, bool);
    DJV_CALLBACK(Color_Picker, display_profile_callback, bool);
    DJV_CALLBACK(Color_Picker, lock_callback, bool);
    DJV_CALLBACK(Color_Picker, close_callback, bool);

    void widget_update();

    const View_Widget *   _view;
    V2i                   _pick;
    Color                 _value;
    SIZE                  _size;
    bool                  _color_profile;
    bool                  _display_profile;
    bool                  _lock;
    Gl_Image_State        _state;
    Gl_Offscreen_Buffer * _buffer;
    Color_Widget *        _widget;
    Color_Swatch *        _swatch;
    Choice_Widget *       _size_widget;
    Tool_Button *         _color_profile_widget;
    Tool_Button *         _display_profile_widget;
    Tool_Button *         _lock_widget;
    Push_Button *         _close_widget;
};

//------------------------------------------------------------------------------

String & operator >> (String &, Color_Picker::SIZE &) throw (String);

String & operator << (String &, Color_Picker::SIZE);

} // djv_view

#endif // DJV_VIEW_COLOR_PICKER_H

