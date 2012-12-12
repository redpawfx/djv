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

//! \file djv_view_help_group.cpp

#include <djv_view_help_group.h>

#include <djv_application.h>
#include <djv_application_about_dialog.h>
#include <djv_application_info_dialog.h>
#include <djv_menu.h>

#include <FL/Fl.H>

namespace djv_view
{

//------------------------------------------------------------------------------
// Help_Group
//------------------------------------------------------------------------------

Help_Group::Help_Group(Menu * menu, const Help_Group * copy) :
    _menu(menu)
{}

Help_Group::~Help_Group()
{}

namespace
{

const String
menu_title = "&Help",
menu_help = "&Help",
menu_info = "&Information",
menu_about = "&About";

} // namespace

void Help_Group::menu_update(Menu_Item_Group * in)
{
    // Menu:
    //
    // * Documentation
    // * Information
    // * About

    in->add(menu_title, 0, 0, 0, Menu_Item::SUB_MENU);

    in->add(menu_help, 0, _help_callback, this);
    in->add(menu_info, 0, _info_callback, this);
    in->add(menu_about, 0, _about_callback, this);

    in->end();
}

void Help_Group::_help_callback()
{
    DJV_APP->help();
}

void Help_Group::_info_callback()
{
    Application_Info_Dialog::global()->show();
}

void Help_Group::_about_callback()
{
    Application_About_Dialog::global()->show();
}

} // djv_view

