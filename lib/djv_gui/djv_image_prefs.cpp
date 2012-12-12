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

//! \file djv_image_prefs.cpp

#include <djv_image_prefs.h>

#include <djv_application.h>
#include <djv_prefs.h>

#include <djv_image_io.h>

namespace djv
{

//------------------------------------------------------------------------------
// Image_Prefs
//------------------------------------------------------------------------------

namespace
{

void _load(const Prefs & in, Image_Io_Base * plugin)
{
    Prefs prefs(in, plugin->name());

    const List<String> & option = plugin->options();

    for (size_t i = 0; i < option.size(); ++i)
    {
        String tmp;

        if (prefs.get(option[i], &tmp))
        {
            plugin->option(option[i], &tmp);
        }
    }
}

void _save(Prefs * in, const Image_Io_Base * plugin)
{
    Prefs prefs(in, plugin->name());

    const List<String> & option = plugin->options();

    for (size_t i = 0; i < option.size(); ++i)
    {
        prefs.set(option[i], plugin->option(option[i]));
    }
}

} // namespace

Image_Prefs::Image_Prefs()
{
    //DJV_DEBUG("Image_Prefs::Image_Prefs");

    Prefs prefs(Prefs::prefs_global(), "image");

    Gl_Image_Filter filter;
    Speed::FPS fps(Speed::FPS(0));
    Time::UNITS units(Time::UNITS(0));

    if (Prefs::get_(&prefs, "filter", &filter))
    {
        Gl_Image_Filter::default_filter = filter;
    }

    if (Prefs::get_(&prefs, "speed", &fps))
    {
        Speed::default_fps = fps;
    }

    if (Prefs::get_(&prefs, "units", &units))
    {
        Time::default_units = units;
    }

    Prefs base(prefs, "base");

    const List<Plugin *> & base_plugins =
        Image_Io_Base_Factory::global()->plugins();

    for (size_t i = 0; i < base_plugins.size(); ++i)
    {
        if (Image_Io_Base * plugin =
            dynamic_cast<Image_Io_Base *>(base_plugins[i]))
        {
            _load(base, plugin);
        }
    }

    Prefs load(prefs, "load");

    const List<Plugin *> & load_plugins =
        Image_Load_Factory::global()->plugins();

    for (size_t i = 0; i < base_plugins.size(); ++i)
    {
        if (Image_Io_Base * plugin =
            dynamic_cast<Image_Io_Base *>(load_plugins[i]))
        {
            _load(load, plugin);
        }
    }

    Prefs save(prefs, "save");

    const List<Plugin *> & save_plugins =
        Image_Save_Factory::global()->plugins();

    for (size_t i = 0; i < base_plugins.size(); ++i)
    {
        if (Image_Io_Base * plugin =
            dynamic_cast<Image_Io_Base *>(save_plugins[i]))
        {
            _load(save, plugin);
        }
    }
}

Image_Prefs::~Image_Prefs()
{
    //DJV_DEBUG("Image_Prefs::~Image_Prefs");

    Prefs prefs(Prefs::prefs_global(), "image");

    Prefs::set_(&prefs, "filter", Gl_Image_Filter::default_filter);
    Prefs::set_(&prefs, "speed", Speed::default_fps);
    Prefs::set_(&prefs, "units", Time::default_units);

    Prefs base(prefs, "base");

    const List<Plugin *> & base_plugins =
        Image_Io_Base_Factory::global()->plugins();

    for (size_t i = 0; i < base_plugins.size(); ++i)
    {
        if (Image_Io_Base * plugin =
            dynamic_cast<Image_Io_Base *>(base_plugins[i]))
        {
            _save(&base, plugin);
        }
    }

    Prefs load(prefs, "load");

    const List<Plugin *> & load_plugins =
        Image_Load_Factory::global()->plugins();

    for (size_t i = 0; i < base_plugins.size(); ++i)
    {
        if (Image_Io_Base * plugin =
            dynamic_cast<Image_Io_Base *>(load_plugins[i]))
        {
            _save(&load, plugin);
        }
    }

    Prefs save(prefs, "save");

    const List<Plugin *> & save_plugins =
        Image_Save_Factory::global()->plugins();

    for (size_t i = 0; i < base_plugins.size(); ++i)
    {
        if (Image_Io_Base * plugin =
            dynamic_cast<Image_Io_Base *>(save_plugins[i]))
        {
            _save(&save, plugin);
        }
    }
}

void Image_Prefs::filter(const Gl_Image_Filter & in)
{
    if (in == filter())
    {
        return;
    }

    Gl_Image_Filter::default_filter = in;

    DJV_APP->window_update();
}

const Gl_Image_Filter & Image_Prefs::filter() const
{
    return Gl_Image_Filter::default_filter;
}

void Image_Prefs::speed(Speed::FPS in)
{
    if (in == speed())
    {
        return;
    }

    Speed::default_fps = in;

    DJV_APP->window_update();
}

Speed::FPS Image_Prefs::speed() const
{
    return Speed::default_fps;
}

void Image_Prefs::units(Time::UNITS in)
{
    if (in == units())
    {
        return;
    }

    Time::default_units = in;

    DJV_APP->window_update();
}

Time::UNITS Image_Prefs::units() const
{
    return Time::default_units;
}

} // djv
