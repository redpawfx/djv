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

//! \file djv_icon.h

#ifndef DJV_ICON_H
#define DJV_ICON_H

#include <djv_gui_export.h>

#include <djv_color_profile.h>
#include <djv_string.h>

#include <FL/Fl_Image.H>

#include <map>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Icon_Factory
//!
//! This class provides an icon factory.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Icon_Factory
{
public:

    //! Constructor.

    Icon_Factory();

    //! Destructor.

    ~Icon_Factory();

    //! Add an icon.

    void add(const String & name, Fl_Image *);

    //! Get an icon.
    //!
    //! \todo FLTK should return a const pointer.

    Fl_Image * get(const String & name) const;

    //! Get an icon.

    static Fl_Image * get_(const String & name);

    //! Convert an image to FLTK.

    static Fl_Image * image_to_fltk(
        const Pixel_Data &,
        const V2i & resize = V2i(),
        const Color_Profile & = Color_Profile());

    //! Get the global icon factory.

    static Icon_Factory * global();

private:

    typedef std::map<String, Fl_Image *> List;

    List     _list;
    Fl_Image _empty;
};

} // djv

#endif // DJV_ICON_H

