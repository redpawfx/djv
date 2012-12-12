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

//! \file djv_image_io.cpp

#include <djv_image_io.h>

#include <djv_system.h>

#include <algorithm>

namespace djv
{

//------------------------------------------------------------------------------
// Image_Io_Info
//------------------------------------------------------------------------------

Image_Io_Info::Image_Io_Info() :
    _info(1)
{}

Image_Io_Info::Image_Io_Info(const Pixel_Data_Info & in) :
    Pixel_Data_Info(in),
    _info(1)
{}

void Image_Io_Info::layer_add(const Pixel_Data_Info & in)
{
    _info += in;
}

void Image_Io_Info::layer_size(size_t in)
{
    _info.resize(in);
}

void Image_Io_Info::layer_clear()
{
    _info.resize(1);
}

size_t Image_Io_Info::layer_size() const
{
    return _info.size();
}

Pixel_Data_Info & Image_Io_Info::operator [] (size_t in)
{
    return 0 == in ? static_cast<Pixel_Data_Info &>(*this) : _info[in];
}

const Pixel_Data_Info & Image_Io_Info::operator [] (size_t in) const
{
    return 0 == in ? static_cast<const Pixel_Data_Info &>(*this) : _info[in];
}

//------------------------------------------------------------------------------
// Image_Io_Frame_Info
//------------------------------------------------------------------------------

Image_Io_Frame_Info::Image_Io_Frame_Info(
    int64_t                frame,
    int                    layer,
    Pixel_Data_Info::PROXY proxy) :
    frame     (frame),
    layer     (layer),
    proxy     (proxy)
{}

//------------------------------------------------------------------------------
// Image_Io_Base
//------------------------------------------------------------------------------

Image_Io_Base::~Image_Io_Base()
{}

List<String> Image_Io_Base::extensions() const
{
    return List<String>();
}

bool Image_Io_Base::is_seq() const
{
    return true;
}

bool Image_Io_Base::option(const String &, String *)
{
    return false;
}

String Image_Io_Base::option(const String &) const
{
    return String();
}

List<String> Image_Io_Base::options() const
{
    return List<String>();
}

namespace
{

const String
    error = "Image I/O",
    error_plugin = "Image I/O, %%",
    error_unrecognized = "Unrecognized file: %%",
    error_unsupported = "Unsupported file: %%",
    error_open = "Cannot open file: %%",
    error_read = "Error reading file: %%",
    error_write = "Error writing file: %%";

} // namespace

void Image_Io_Base::throw_error(
    const String & plugin,
    const String & error) throw (Error)
{
    throw Error(
        String_Format(error_plugin).arg(plugin),
        error);
}

void Image_Io_Base::throw_error_unrecognized(
    const String & plugin,
    const String & file_name) throw (Error)
{
    throw Error(
        String_Format(error_plugin).arg(plugin),
        String_Format(error_unrecognized).arg(file_name));
}

void Image_Io_Base::throw_error_unsupported(
    const String & plugin,
    const String & file_name) throw (Error)
{
    throw Error(
        String_Format(error_plugin).arg(plugin),
        String_Format(error_unsupported).arg(file_name));
}

void Image_Io_Base::throw_error_open(
    const String & plugin,
    const String & file_name) throw (Error)
{
    throw Error(
        String_Format(error_plugin).arg(plugin),
        String_Format(error_open).arg(file_name));
}

void Image_Io_Base::throw_error_read(
    const String & plugin,
    const String & file_name) throw (Error)
{
    throw Error(
        String_Format(error_plugin).arg(plugin),
        String_Format(error_read).arg(file_name));
}

void Image_Io_Base::throw_error_write(
    const String & plugin,
    const String & file_name) throw (Error)
{
    throw Error(
        String_Format(error_plugin).arg(plugin),
        String_Format(error_write).arg(file_name));
}

//------------------------------------------------------------------------------
// Image_Save
//------------------------------------------------------------------------------

Image_Save::~Image_Save()
{
    close();
}

void Image_Save::close() throw (Error)
{}

//------------------------------------------------------------------------------
// Image_Io_Base_Factory
//------------------------------------------------------------------------------

Image_Io_Base_Factory::Image_Io_Base_Factory(const List<String> & search_path)
    throw (Error) :
    Plugin_Factory(
        search_path.size() ? search_path : System::search_path(),
        "djv_image_io_base")
{
    //DJV_DEBUG("Image_Io_Base_Factory::Image_Io_Base_Factory");

    // Update file sequence extensions.

    Set<String> seq_extensions;

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Io_Base * plugin = dynamic_cast<Image_Io_Base *>(plugins[i]))
        {
            if (plugin->is_seq())
            {
                seq_extensions += plugin->extensions();
            }
        }
    }

    //DJV_DEBUG_PRINT("seq extensions = " << seq_extensions);

    File::seq_extensions.add(seq_extensions);
}

Image_Io_Base_Factory::~Image_Io_Base_Factory()
{
    //DJV_DEBUG("Image_Io_Base_Factory::~Image_Io_Base_Factory");
}

bool Image_Io_Base_Factory::option(
    const String & name,
    const String & option,
    String *       data)
{
    //DJV_DEBUG("Image_Io_Base_Factory::option");
    //DJV_DEBUG_PRINT("name = " << name);
    //DJV_DEBUG_PRINT("option = " << option);
    //DJV_DEBUG_PRINT("data = " << *data);

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Io_Base * plugin = dynamic_cast<Image_Io_Base *>(plugins[i]))
        {
            if (String_Util::compare_no_case(name, plugin->name()))
            {
                return plugin->option(option, data);
            }
        }
    }

    return false;
}

Image_Io_Base_Factory * Image_Io_Base_Factory::global()
{
    static Image_Io_Base_Factory * factory = 0;

    if (! factory)
    {
        factory = new Image_Io_Base_Factory;
    }

    return factory;
}

//------------------------------------------------------------------------------
// Image_Load_Factory
//------------------------------------------------------------------------------

Image_Load_Factory::Image_Load_Factory(const List<String> & search_path)
    throw (Error) :
    Plugin_Factory(
        search_path.size() ? search_path : System::search_path(),
        "djv_image_load")
{
    //DJV_DEBUG("Image_Load_Factory::Image_Load_Factory");

    // Update file sequence extensions.

    Set<String> seq_extensions;

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Load * plugin = dynamic_cast<Image_Load *>(plugins[i]))
        {
            if (plugin->is_seq())
            {
                seq_extensions += plugin->extensions();
            }
        }
    }

    //DJV_DEBUG_PRINT("seq extensions = " << seq_extensions);

    File::seq_extensions.add(seq_extensions);
}

Image_Load_Factory::~Image_Load_Factory()
{
    //DJV_DEBUG("Image_Load_Factory::~Image_Load_Factory");
}

bool Image_Load_Factory::is_valid(
    const File &   file,
    const String & name) const
{
    //DJV_DEBUG("Image_Load_Factory::is_valid");
    //DJV_DEBUG_PRINT("file = " << file);
    //DJV_DEBUG_PRINT("name = " << name);

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Load * plugin = dynamic_cast<Image_Load *>(plugins[i]))
        {
            if (name.size())
            {
                if (! String_Util::compare_no_case(name, plugin->name()))
                {
                    continue;
                }
            }

            if (String_Util::find_no_case(
                        file.extension(),
                        plugin->extensions()))
            {
                return true;
            }
        }
    }

    return false;
}

Image_Load * Image_Load_Factory::get(
    const File &    file,
    Image_Io_Info * info,
    const String &  name) const throw (Error)
{
    //DJV_DEBUG("Image_Load_Factory::get");
    //DJV_DEBUG_PRINT("file = " << file);
    //DJV_DEBUG_PRINT("extension = " << file.extension());
    //DJV_DEBUG_PRINT("name = " << name);

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Load * plugin = dynamic_cast<Image_Load *>(plugins[i]))
        {
            if (name.size())
            {
                if (! String_Util::compare_no_case(name, plugin->name()))
                {
                    continue;
                }
            }
            else
            {
                if (! String_Util::find_no_case(
                            file.extension(),
                            plugin->extensions()))
                {
                    continue;
                }
            }

            std::auto_ptr<Image_Load> tmp(
                static_cast<Image_Load *>(plugin->copy()));

            Image_Io_Info _info;

            tmp->open(file, _info);

            if (info)
            {
                *info = _info;
            }

            return tmp.release();
        }
    }

    throw Error(error, String_Format(error_unrecognized).arg(file));

    return 0;
}

bool Image_Load_Factory::option(
    const String & name,
    const String & option,
    String *       data)
{
    //DJV_DEBUG("Image_Load_Factory::option");
    //DJV_DEBUG_PRINT("name = " << name);
    //DJV_DEBUG_PRINT("option = " << option);
    //DJV_DEBUG_PRINT("data = " << *data);

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Load * plugin = dynamic_cast<Image_Load *>(plugins[i]))
        {
            if (String_Util::compare_no_case(name, plugin->name()))
            {
                return plugin->option(option, data);
            }
        }
    }

    return false;
}

Image_Load_Factory * Image_Load_Factory::global()
{
    static Image_Load_Factory * factory = 0;

    if (! factory)
    {
        factory = new Image_Load_Factory;
    }

    return factory;
}

//------------------------------------------------------------------------------
// Image_Save_Factory
//------------------------------------------------------------------------------

Image_Save_Factory::Image_Save_Factory(const List<String> & search_path)
    throw (Error) :
    Plugin_Factory(
        search_path.size() ? search_path : System::search_path(),
        "djv_image_save")
{
    //DJV_DEBUG("Image_Save_Factory::Image_Save_Factory");

    // Update file sequence extensions.

    Set<String> seq_extensions;

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Save * plugin = dynamic_cast<Image_Save *>(plugins[i]))
        {
            if (plugin->is_seq())
            {
                seq_extensions += plugin->extensions();
            }
        }
    }

    //DJV_DEBUG_PRINT("seq extensions = " << seq_extensions);

    File::seq_extensions.add(seq_extensions);
}

Image_Save_Factory::~Image_Save_Factory()
{
    //DJV_DEBUG("Image_Save_Factory::~Image_Save_Factory");
}

Image_Save * Image_Save_Factory::get(
    const File &          file,
    const Image_Io_Info & info,
    const String &        name) const throw (Error)
{
    //DJV_DEBUG("Image_Save_Factory::get");
    //DJV_DEBUG_PRINT("file = " << file);
    //DJV_DEBUG_PRINT("info = " << info);
    //DJV_DEBUG_PRINT("name = " << name);

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Save * plugin = dynamic_cast<Image_Save *>(plugins[i]))
        {
            if (name.size())
            {
                if (! String_Util::compare_no_case(name, plugin->name()))
                {
                    continue;
                }
            }
            else
            {
                if (! String_Util::find_no_case(
                            file.extension(),
                            plugin->extensions()))
                {
                    continue;
                }
            }

            std::auto_ptr<Image_Save> tmp(
                static_cast<Image_Save *>(plugin->copy()));

            tmp->open(file, info);

            return tmp.release();
        }
    }

    throw Error(error, String_Format(error_unrecognized).arg(file));

    return 0;
}

bool Image_Save_Factory::option(
    const String & name,
    const String & option,
    String *       data)
{
    //DJV_DEBUG("Image_Save_Factory::option");
    //DJV_DEBUG_PRINT("name = " << name);
    //DJV_DEBUG_PRINT("option = " << option);
    //DJV_DEBUG_PRINT("data = " << *data);

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Save * plugin = dynamic_cast<Image_Save *>(plugins[i]))
        {
            if (String_Util::compare_no_case(name, plugin->name()))
            {
                return plugin->option(option, data);
            }
        }
    }

    return false;
}

Image_Save_Factory * Image_Save_Factory::global()
{
    static Image_Save_Factory * factory = 0;

    if (! factory)
    {
        factory = new Image_Save_Factory;
    }

    return factory;
}

//------------------------------------------------------------------------------

bool operator == (const Image_Io_Info & a, const Image_Io_Info & b)
{
    if (a.layer_size() != b.layer_size())
    {
        return false;
    }

    for (size_t i = 0; i < a.layer_size(); ++i)
        if (a[i] != b[i])
        {
            return false;
        }

    return
        operator == (
            static_cast<const Pixel_Data_Info &>(a),
            static_cast<const Pixel_Data_Info &>(b)) &&
        a.tag == b.tag &&
        a.seq == b.seq;
}

bool operator != (const Image_Io_Info & a, const Image_Io_Info & b)
{
    return ! (a == b);
}

bool operator == (const Image_Io_Frame_Info & a, const Image_Io_Frame_Info & b)
{
    return
        a.frame == b.frame &&
        a.layer == b.layer &&
        a.proxy == b.proxy;
}

bool operator != (const Image_Io_Frame_Info & a, const Image_Io_Frame_Info & b)
{
    return ! (a == b);
}

Debug & operator << (Debug & debug, const Image_Io_Info & in)
{
    //! \todo Is this still necessary?

    return operator << (debug, static_cast<const Pixel_Data_Info &>(in));
}

Debug & operator << (Debug & debug, const Image_Io_Frame_Info & in)
{
    //! \todo Is this still necessary?

    return debug << in.frame;
}

} // djv
