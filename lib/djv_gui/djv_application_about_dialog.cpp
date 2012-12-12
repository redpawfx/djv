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

//! \file djv_application_about_dialog.cpp

#include <djv_application_about_dialog.h>

#include <djv_application.h>
#include <djv_image_label.h>
#include <djv_multiline_label.h>
#include <djv_row_layout.h>
#include <djv_push_button.h>
#include <djv_scroll.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H>

#include "durant.xpm"

namespace djv
{

//------------------------------------------------------------------------------
// Application_About_Dialog
//------------------------------------------------------------------------------

Application_About_Dialog::Application_About_Dialog() :
    Dialog("About"),
    _close_widget(0)
{
    _image = std::auto_ptr<Fl_Image>(new Fl_Pixmap((const char **)durant_xpm));

    // Create widgets.

    Image_Label * image = new Image_Label(_image.get());

    Scroll * scroll = new Scroll;
    
    Multiline_Label * label = new Multiline_Label(DJV_APP->about());
    label->align(LEFT, LEFT);

    _close_widget = new Push_Button("Clos&e");

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(image);
    layout->add(scroll);
    layout->stretch(scroll);
    
    Vertical_Layout * layout_scroll = new Vertical_Layout(scroll);
    layout_scroll->add(label);
    layout_scroll->stretch(label);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add_stretch();
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    size(V2i(600, 600));

    // Callbacks.

    _close_widget->signal.set(this, close_callback);
}

Application_About_Dialog::~Application_About_Dialog()
{}

void Application_About_Dialog::show()
{
    _close_widget->take_focus();

    Dialog::show();
}

Application_About_Dialog * Application_About_Dialog::global()
{
    static Application_About_Dialog * dialog = 0;

    if (! dialog)
    {
        dialog = new Application_About_Dialog;
    }

    return dialog;
}

void Application_About_Dialog::close_callback(bool)
{
    hide();
}

} // djv

