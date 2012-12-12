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

//! \file djv_image_io.h

#ifndef DJV_IMAGE_IO_H
#define DJV_IMAGE_IO_H

#include <djv_file.h>
#include <djv_image.h>
#include <djv_plugin.h>
#include <djv_time.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Image_Io_Info
//!
//! This class provides image I/O information.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Image_Io_Info : public Pixel_Data_Info
{
public:

    //! Constructor.

    Image_Io_Info();

    //! Constructor.

    Image_Io_Info(const Pixel_Data_Info &);

    //! Add a layer.

    void layer_add(const Pixel_Data_Info &);

    //! Set the number of layers.

    void layer_size(size_t);

    //! Remove all the layers.

    void layer_clear();

    //! Get the number of layers.

    size_t layer_size() const;

    Pixel_Data_Info & operator [] (size_t);

    const Pixel_Data_Info & operator [] (size_t) const;

    Image_Tag tag;
    Seq       seq;

private:

    List<Pixel_Data_Info> _info;
};

//------------------------------------------------------------------------------
//! \struct Image_Io_Frame_Info
//!
//! This struct provides image I/O frame information.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Image_Io_Frame_Info
{
    //! Constructor.

    Image_Io_Frame_Info(
        int64_t                frame = -1,
        int                    layer = 0,
        Pixel_Data_Info::PROXY proxy = Pixel_Data_Info::PROXY_NONE);

    int64_t                frame;
    int                    layer;
    Pixel_Data_Info::PROXY proxy;
};

//------------------------------------------------------------------------------
//! \class Image_Io_Base
//!
//! This class provides the base functionality for image I/O plugins.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Image_Io_Base : public Plugin
{
public:

    //! Destructor.

    virtual ~Image_Io_Base() = 0;

    //! List of supported file extensions.

    virtual List<String> extensions() const;

    //! Does the plugin use file sequences?

    virtual bool is_seq() const;

    //! Set an option.

    virtual bool option(const String &, String *);

    //! Get an option.

    virtual String option(const String &) const;

    //! Get the list of options.

    virtual List<String> options() const;

    //! Throw an error.

    static void throw_error(
        const String & plugin,
        const String & error) throw (Error);

    //! Throw an error.

    static void throw_error_unrecognized(
        const String & plugin,
        const String & file_name) throw (Error);

    //! Throw an error.

    static void throw_error_unsupported(
        const String & plugin,
        const String & file_name) throw (Error);

    //! Throw an error.

    static void throw_error_open(
        const String & plugin,
        const String & file_name) throw (Error);

    //! Throw an error.

    static void throw_error_read(
        const String & plugin,
        const String & file_name) throw (Error);

    //! Throw an error.

    static void throw_error_write(
        const String & plugin,
        const String & file_name) throw (Error);
};

//------------------------------------------------------------------------------
//! \class Image_Load
//!
//! This class provides the base functionality for image load plugins.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Image_Load : public Image_Io_Base
{
public:

    //! Open an image.

    virtual void open(
        const File &,
        Image_Io_Info &) throw (Error) = 0;

    //! Load an image.

    virtual void load(
        Image &,
        const Image_Io_Frame_Info & = Image_Io_Frame_Info()) throw (Error) = 0;
};

//------------------------------------------------------------------------------
//! \class Image_Save
//!
//! This class provides the base functionality for image save plugins.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Image_Save : public Image_Io_Base
{
public:

    //! Destructor.

    virtual ~Image_Save() = 0;

    //! Open an image.

    virtual void open(
        const File &,
        const Image_Io_Info &) throw (Error) = 0;

    //! Close the image.

    virtual void close() throw (Error);

    //! Save an image.

    virtual void save(
        const Image &,
        const Image_Io_Frame_Info & = Image_Io_Frame_Info()) throw (Error) = 0;
};

//------------------------------------------------------------------------------
//! \class Image_Io_Base_Factory
//!
//! This class provides a factory for image I/O plugins.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Image_Io_Base_Factory : public Plugin_Factory
{
public:

    //! Constructor.

    Image_Io_Base_Factory(const List<String> & search_path = List<String>())
    throw (Error);

    //! Destructor.

    virtual ~Image_Io_Base_Factory();

    //! Plugin options.

    bool option(const String & name, const String &, String *);

    //! Get the global factory.

    static Image_Io_Base_Factory * global();
};

//------------------------------------------------------------------------------
//! \class Image_Load_Factory
//!
//! This class provides a factory for image load plugins.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Image_Load_Factory : public Plugin_Factory
{
public:

    //! Constructor.

    Image_Load_Factory(const List<String> & search_path = List<String>())
    throw (Error);

    //! Destructor.

    virtual ~Image_Load_Factory();

    //! Test if a file is an image.

    bool is_valid(
        const File &,
        const String & name = String()) const;

    //! Create a new plugin.

    Image_Load * get(
        const File &    file,
        Image_Io_Info * info = 0,
        const String &  name = String()) const throw (Error);

    //! Plugin options.

    bool option(const String & name, const String &, String *);

    //! Get the global factory.

    static Image_Load_Factory * global();
};

//------------------------------------------------------------------------------
//! \class Image_Save_Factory
//!
//! This class provides a factory for image save plugin factory.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Image_Save_Factory : public Plugin_Factory
{
public:

    //! Constructor.

    Image_Save_Factory(const List<String> & search_path = List<String>())
    throw (Error);

    //! Destructor.

    virtual ~Image_Save_Factory();

    //! Create a new plugin.

    Image_Save * get(
        const File &          file,
        const Image_Io_Info & info,
        const String &        name = String()) const throw (Error);

    //! Plugin options.

    bool option(const String & name, const String &, String *);

    //! Get the global factory.

    static Image_Save_Factory * global();
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT bool operator == (
    const Image_Io_Info &,
    const Image_Io_Info &);
DJV_CORE_EXPORT bool operator == (
    const Image_Io_Frame_Info &,
    const Image_Io_Frame_Info &);

DJV_CORE_EXPORT bool operator != (
    const Image_Io_Info &,
    const Image_Io_Info &);
DJV_CORE_EXPORT bool operator != (
    const Image_Io_Frame_Info &,
    const Image_Io_Frame_Info &);

DJV_CORE_EXPORT Debug & operator << (Debug &, const Image_Io_Info &);
DJV_CORE_EXPORT Debug & operator << (Debug &, const Image_Io_Frame_Info &);

} // djv

#endif

