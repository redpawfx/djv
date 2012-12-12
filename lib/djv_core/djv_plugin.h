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

//! \file djv_plugin.h

#ifndef DJV_PLUGIN_H
#define DJV_PLUGIN_H

#include <djv_error.h>
#include <djv_string.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Plugin
//!
//! This class provides the base functionality for plugins.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Plugin
{
public:

    //! Destructor.

    virtual ~Plugin() = 0;

    //! Initialize the plugin.

    virtual void init() throw (Error) {}

    //! De-initialize the plugin.

    virtual void del() {}

    //! Copy the plugin.

    virtual Plugin * copy() const = 0;

    //! Get the plugin name.

    virtual String name() const = 0;
};

//! Plugin entry point export.

#if defined(DJV_WINDOWS)
#define DJV_PLUGIN_EXPORT __declspec(dllexport)
#else
#define DJV_PLUGIN_EXPORT
#endif

//! Plugin entry point.

typedef Plugin * (Plugin_Entry)();

//------------------------------------------------------------------------------
//! \class Plugin_Factory
//!
//! This class provides the base functionality for plugin factories.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Plugin_Factory
{
public:

    //! Constructor.

    Plugin_Factory(
        const List<String> & search_path,
        const String &       plugin_entry,
        const String &       plugin_prefix = "djv_") throw (Error);

    //! Destructor.

    virtual ~Plugin_Factory() = 0;

    //! Get the list of plugins.

    const List<Plugin *> & plugins() const;

    //! Get the list of plugin names.

    List<String> names() const;

private:

    class Handle;

    String         _plugin_prefix;
    String         _plugin_entry;
    List<Plugin *> _plugins;
    List<Handle *> _handle;
};

} // djv

#endif // DJV_PLUGIN_H

