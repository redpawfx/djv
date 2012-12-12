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

//! \file djv_plugin.cpp

#include <djv_plugin.h>

#include <djv_core_application.h>
#include <djv_directory.h>
#include <djv_file.h>

#include <algorithm>
#if ! defined(DJV_WINDOWS)
#include <dlfcn.h>
#endif

namespace djv
{

//------------------------------------------------------------------------------
// Plugin
//------------------------------------------------------------------------------

Plugin::~Plugin()
{}

//------------------------------------------------------------------------------
// Plugin_Factory::Handle
//------------------------------------------------------------------------------

class Plugin_Factory::Handle
{
public:

    Handle() :
        _p(0)
    {}

    ~Handle()
    {
#   if defined(DJV_WINDOWS)

        if (_p)
        {
            ::FreeLibrary(_p);
        }

#   else

        if (_p)
        {
            ::dlclose(_p);
        }

#   endif
    }

    void open(const String & in) throw (Error)
    {
#   if defined(DJV_WINDOWS)

        _p = ::LoadLibrary(in.c_str());

#   else

        _p = ::dlopen(in.c_str(), RTLD_LAZY);

#   endif

        if (! _p)
        {
            String error;

#     if defined(DJV_WINDOWS)

            error = error::last_error();

#     else

            error = ::dlerror();

#     endif

            throw Error(error);
        }
    }

    void * fnc(const String & in) const
    {
#   if defined(DJV_WINDOWS)

        return (void *)::GetProcAddress(_p, in.c_str());

#   else

        return ::dlsym(_p, in.c_str());

#   endif
    }

private:

# if defined(DJV_WINDOWS)

    HINSTANCE _p;

# else

    void * _p;

# endif
};

//------------------------------------------------------------------------------
// Plugin_Factory
//------------------------------------------------------------------------------

Plugin_Factory::Plugin_Factory(
    const List<String> & search_path,
    const String &       plugin_entry,
    const String &       plugin_prefix) throw (Error)
{
    //DJV_DEBUG("Plugin_Factory::Plugin_Factory");
    //DJV_DEBUG_PRINT("search path = " << _search_path);
    //DJV_DEBUG_PRINT("plugin entry = " << plugin_entry);
    //DJV_DEBUG_PRINT("plugin prefix = " << plugin_prefix);

    //! \todo OS specific library names.

    List<String> filter;

# if defined(DJV_WINDOWS)

    filter += plugin_prefix + "*.dll";

# else

    filter += "lib" + plugin_prefix + "*.so";

# endif

    //DJV_DEBUG_PRINT("filter = " << filter);

    // Find plugins.

    List<String> list;

    for (size_t i = 0; i < search_path.size(); ++i)
    {
        const Directory directory(search_path[i]);

        //DJV_DEBUG_PRINT("searching = " << directory.get());

        List<File> files = directory.list();

        File_Util::filter(files, File_Util::FILTER_NONE, filter);

        for (size_t i = 0; i < files.size(); ++i)
        {
            //DJV_DEBUG_PRINT("found = " << files[i]);

            list += files[i];
        }
    }

    // Load plugins.

    //DJV_DEBUG_PRINT("size = " << list.size());

    for (size_t i = 0; i < list.size(); ++i)
    {
        //DJV_DEBUG_PRINT("loading = " << list[i]);

        // Open.

        std::auto_ptr<Handle> handle(new Handle);

        try
        {
            handle->open(list[i]);
        }
        catch (Error in)
        {
            //DJV_DEBUG_PRINT("plugin error = " << in);

            continue;
        }

        Plugin_Entry * entry = (Plugin_Entry *)handle->fnc(plugin_entry);

        if (! entry)
        {
            //DJV_DEBUG_PRINT("plugin entry point error");

            continue;
        }

        std::auto_ptr<Plugin> plugin;

        try
        {
            plugin = std::auto_ptr<Plugin>(entry());
        }
        catch (Error in)
        {
            plugin.reset();

            DJV_CORE_APP->error(in);
        }

        if (! plugin.get())
        {
            //DJV_DEBUG_PRINT("plugin create error");

            continue;
        }

        //DJV_DEBUG_PRINT("name = " << plugin->name());

        // Check for duplicates.

        size_t j = 0;

        for (; j < _plugins.size(); ++j)
        {
            if (plugin->name() == _plugins[j]->name())
            {
                break;
            }
        }

        if (j < _plugins.size())
        {
            //DJV_DEBUG_PRINT("duplicate");

            plugin.reset();

            continue;
        }

        // Initialize.

        try
        {
            plugin->init();
        }
        catch (Error in)
        {
            //DJV_DEBUG_PRINT("plugin init error");

            plugin->del();
            plugin.reset();

            //DJV_CORE_APP->error(in);

            continue;
        }

        // Add.

        _plugins += plugin.release();

        _handle += handle.release();
    }
}

Plugin_Factory::~Plugin_Factory()
{
    //DJV_DEBUG("Plugin_Factory::~Plugin_Factory");

    for (size_t i = 0; i < _plugins.size(); ++i)
    {
        _plugins[i]->del();

        delete _plugins[i];
    }

    _plugins.clear();

    for (size_t i = 0; i < _handle.size(); ++i)
    {
        delete _handle[i];
    }

    _handle.clear();
}

const List<Plugin *> & Plugin_Factory::plugins() const
{
    return _plugins;
}

List<String> Plugin_Factory::names() const
{
    List<String> out(_plugins.size());

    for (size_t i = 0; i < _plugins.size(); ++i)
    {
        out[i] = _plugins[i]->name();
    }

    return out;
}

} // djv
