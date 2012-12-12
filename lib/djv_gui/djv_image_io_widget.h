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

//! \file djv_image_io_widget.h

#ifndef DJV_IMAGE_IO_WIDGET_H
#define DJV_IMAGE_IO_WIDGET_H

#include <djv_widget.h>

#include <djv_image_io.h>
#include <djv_plugin.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Image_Io_Base_Widget
//!
//! This class provides the base functionality for image I/O widget plugins.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Image_Io_Base_Widget : public Widget, public Plugin
{
public:

    //! Constructor.

    Image_Io_Base_Widget();

    //! Destructor.

    virtual ~Image_Io_Base_Widget();

    //! Set the plugin.

    virtual void set(Image_Io_Base *) = 0;

    //! This signal is emitted when an option is changed.

    Signal<Image_Io_Base *> signal;
};

//------------------------------------------------------------------------------
//! \class Image_Load_Widget
//!
//! This class provides the base functionality for image load widget plugins.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Image_Load_Widget : public Image_Io_Base_Widget {};

//------------------------------------------------------------------------------
//! \class Image_Save_Widget
//!
//! This class provides the base functionality for image save widget plugins.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Image_Save_Widget : public Image_Io_Base_Widget {};

//------------------------------------------------------------------------------
//! \class Image_Io_Base_Widget_Factory
//!
//! This class provides a factory for image I/O widget plugins.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Image_Io_Base_Widget_Factory :
    public Callback,
    public Plugin_Factory
{
public:

    //! Constructor.

    Image_Io_Base_Widget_Factory(
        const List<String> & search_path = List<String>()) throw (Error);

    //! Destructor.

    virtual ~Image_Io_Base_Widget_Factory();

    //! Get a widget.

    Image_Io_Base_Widget * get(Image_Io_Base *) const;

    //! This signal is emitted when an option is changed.

    Signal<Image_Io_Base *> signal;

    //! Get the global factory.

    static Image_Io_Base_Widget_Factory * global();

private:

    String plugin_entry() const
    {
        return "djv_image_io_base_widget";
    }

    DJV_CALLBACK(Image_Io_Base_Widget_Factory, callback, Image_Io_Base *);
};

//------------------------------------------------------------------------------
//! \class Image_Load_Widget_Factory
//!
//! This class provides a factory for image load widget plugins.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Image_Load_Widget_Factory :
    public Callback,
    public Plugin_Factory
{
public:

    //! Constructor.

    Image_Load_Widget_Factory(const List<String> & search_path = List<String>())
    throw (Error);

    //! Destructor.

    virtual ~Image_Load_Widget_Factory();

    //! Get a widget.

    Image_Load_Widget * get(Image_Load *) const;

    //! This signal is emitted when an option is changed.

    Signal<Image_Io_Base *> signal;

    //! Get the global factory.

    static Image_Load_Widget_Factory * global();

private:

    String plugin_entry() const
    {
        return "djv_image_load_widget";
    }

    DJV_CALLBACK(Image_Load_Widget_Factory, callback, Image_Io_Base *);
};

//------------------------------------------------------------------------------
//! \class Image_Save_Widget_Factory
//!
//! This class provides a factory for image save widget plugins.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Image_Save_Widget_Factory :
    public Callback,
    public Plugin_Factory
{
public:

    //! Constructor.

    Image_Save_Widget_Factory(const List<String> & search_path = List<String>())
    throw (Error);

    //! Destructor.

    virtual ~Image_Save_Widget_Factory();

    //! Get a widget.

    Image_Save_Widget * get(Image_Save *) const;

    //! This signal is emitted when an option is changed.

    Signal<Image_Io_Base *> signal;

    //! Get the global factory.

    static Image_Save_Widget_Factory * global();

private:

    String plugin_entry() const
    {
        return "djv_image_save_widget";
    }

    DJV_CALLBACK(Image_Save_Widget_Factory, callback, Image_Io_Base *);
};

//------------------------------------------------------------------------------
//! \class Image_Io_Prefs_Widget
//!
//! This class provides a preferences widget for image I/O widget plugins.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Image_Io_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    Image_Io_Prefs_Widget();

    //! Get the global preferences.

    static Image_Io_Prefs_Widget * global();
};

} // djv

#endif // DJV_IMAGE_IO_WIDGET_H

