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

//! \file djv_application_info_dialog.cpp

#include <djv_application_info_dialog.h>

#include <djv_application.h>
#include <djv_form_widget.h>
#include <djv_group_box.h>
#include <djv_image_io_widget.h>
#include <djv_multiline_label.h>
#include <djv_row_layout.h>
#include <djv_prefs.h>
#include <djv_push_button.h>
#include <djv_scroll.h>
#include <djv_style.h>

#include <djv_gl_context.h>
#include <djv_image_io.h>
#include <djv_memory.h>
#include <djv_system.h>

namespace djv
{

//------------------------------------------------------------------------------
// Application_Info_Dialog
//------------------------------------------------------------------------------

Application_Info_Dialog::Application_Info_Dialog() :
    Dialog("Information"),
    _close_widget(0)
{
    //DJV_DEBUG("Application_Info_Dialog::Application_Info_Dialog");

    // Create widgets.
    
    Scroll * scroll = new Scroll;
    
    _close_widget = new Push_Button("Clos&e");

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(scroll);
    layout->stretch(scroll);
    
    Vertical_Layout * layout_v = new Vertical_Layout(scroll);
    layout_v->margin(Style::global()->margin_large());
    layout_v->spacing(Style::global()->spacing_large());
    
    Group_Box * group_box = new Group_Box("General");
    layout_v->add(group_box);
    Form_Widget * general_form = new Form_Widget;
    group_box->layout()->add(general_form);
    general_form->add_row("Version:", new Multiline_Label(DJV_VERSION_NAME));
    general_form->add_row("System:", new Multiline_Label(DJV_SYSTEM_NAME));
    general_form->add_row("Information:", new Multiline_Label(System::info()));
    general_form->add_row("Endian:",
        new Multiline_Label(String_Util::label(Memory::endian())));
    general_form->add_row("Search Path:",
        new Multiline_Label(String_Util::join(System::search_path(), ", ")));

    group_box = new Group_Box("OpenGL");
    layout_v->add(group_box);
    Form_Widget * opengl_form = new Form_Widget;
    group_box->layout()->add(opengl_form);
    opengl_form->add_row("Vendor:",
        new Multiline_Label(DJV_APP->context()->vendor()));
    opengl_form->add_row("Renderer:",
        new Multiline_Label(DJV_APP->context()->renderer()));
    opengl_form->add_row("Version:",
        new Multiline_Label(DJV_APP->context()->version()));

    group_box = new Group_Box("Image I/O Plugins");
    layout_v->add(group_box);
    Form_Widget * image_io_form = new Form_Widget;
    group_box->layout()->add(image_io_form);
    image_io_form->add_row("Base:",
        new Multiline_Label(String_Util::join(
            Image_Io_Base_Factory::global()->names(), ", ")));
    image_io_form->add_row("Load:",
        new Multiline_Label(String_Util::join(
            Image_Load_Factory::global()->names(), ", ")));
    image_io_form->add_row("Save:",
        new Multiline_Label(String_Util::join(
            Image_Save_Factory::global()->names(), ", ")));

    group_box = new Group_Box("Image I/O Widget Plugins");
    layout_v->add(group_box);
    Form_Widget * image_io_widget_form = new Form_Widget;
    group_box->layout()->add(image_io_widget_form);
    image_io_widget_form->add_row("Base:",
        new Multiline_Label(String_Util::join(
            Image_Io_Base_Widget_Factory::global()->names(), ", ")));
    image_io_widget_form->add_row("Load:",
        new Multiline_Label(String_Util::join(
            Image_Load_Widget_Factory::global()->names(), ", ")));
    image_io_widget_form->add_row("Save:",
        new Multiline_Label(String_Util::join(
            Image_Save_Widget_Factory::global()->names(), ", ")));
    
    group_box = new Group_Box("Preferences");
    layout_v->add(group_box);
    Form_Widget * prefs_form = new Form_Widget;
    group_box->layout()->add(prefs_form);
    prefs_form->add_row("Application:",
        new Multiline_Label(Prefs::prefs()->file_name()));
    prefs_form->add_row("Global:",
        new Multiline_Label(Prefs::prefs_global()->file_name()));

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

Application_Info_Dialog::~Application_Info_Dialog()
{}

void Application_Info_Dialog::show()
{
    Dialog::show();

    _close_widget->take_focus();
}

Application_Info_Dialog * Application_Info_Dialog::global()
{
    static Application_Info_Dialog * dialog = 0;

    if (! dialog)
    {
        dialog = new Application_Info_Dialog;
    }

    return dialog;
}

void Application_Info_Dialog::close_callback(bool)
{
    hide();
}

} // djv

