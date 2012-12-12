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

//! \file djv_image_io_widget.cpp

#include <djv_image_io_widget.h>

#include <djv_row_layout.h>
#include <djv_tabs.h>

#include <djv_system.h>

namespace djv
{

//------------------------------------------------------------------------------
// Image_Io_Base_Widget
//------------------------------------------------------------------------------

Image_Io_Base_Widget::Image_Io_Base_Widget() :
    signal(this)
{}

Image_Io_Base_Widget::~Image_Io_Base_Widget()
{}

//------------------------------------------------------------------------------
// Image_Io_Base_Widget_Factory
//------------------------------------------------------------------------------

Image_Io_Base_Widget_Factory::Image_Io_Base_Widget_Factory(
    const List<String> & search_path)
    throw (Error) :
    Plugin_Factory(
        search_path.size() ? search_path : System::search_path(),
        "djv_image_io_base_widget"),
    signal(this)
{
    //DJV_DEBUG("Image_Io_Base_Widget_Factory::Image_Io_Base_Widget_Factory");
}

Image_Io_Base_Widget_Factory::~Image_Io_Base_Widget_Factory()
{
    //DJV_DEBUG("Image_Io_Base_Widget_Factory::~Image_Io_Base_Widget_Factory");

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Io_Base_Widget * plugin =
                    dynamic_cast<Image_Io_Base_Widget *>(plugins[i]))
        {
            plugin->Widget::del();
        }
    }
}

Image_Io_Base_Widget * Image_Io_Base_Widget_Factory::get(
    Image_Io_Base * plugin) const
{
    if (plugin)
    {
        const String name = plugin->name();

        const List<Plugin *> & plugins = this->plugins();

        for (size_t i = 0; i < plugins.size(); ++i)
        {
            if (Image_Io_Base_Widget * tmp =
                        dynamic_cast<Image_Io_Base_Widget *>(plugins[i]))
            {
                if (tmp->name() == name)
                {
                    Image_Io_Base_Widget * ui_plugin =
                        static_cast<Image_Io_Base_Widget *>(tmp->copy());

                    ui_plugin->set(plugin);

                    ui_plugin->signal.set(
                        const_cast<Image_Io_Base_Widget_Factory *>(this),
                        callback);

                    return ui_plugin;
                }
            }
        }
    }

    return 0;
}

Image_Io_Base_Widget_Factory * Image_Io_Base_Widget_Factory::global()
{
    static Image_Io_Base_Widget_Factory * global = 0;

    if (! global)
    {
        global = new Image_Io_Base_Widget_Factory;
    }

    return global;
}

void Image_Io_Base_Widget_Factory::callback(Image_Io_Base * plugin)
{
    signal.emit(plugin);
}

//------------------------------------------------------------------------------
// Image_Load_Widget_Factory
//------------------------------------------------------------------------------

Image_Load_Widget_Factory::Image_Load_Widget_Factory(
    const List<String> & search_path)
    throw (Error) :
    Plugin_Factory(
        search_path.size() ? search_path : System::search_path(),
        "djv_image_load_widget"),
    signal(this)
{
    //DJV_DEBUG("Image_Load_Widget_Factory::Image_Load_Widget_Factory");
}

Image_Load_Widget_Factory::~Image_Load_Widget_Factory()
{
    //DJV_DEBUG("Image_Load_Widget_Factory::~Image_Load_Widget_Factory");

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Load_Widget * plugin =
                    dynamic_cast<Image_Load_Widget *>(plugins[i]))
        {
            plugin->Widget::del();
        }
    }
}

Image_Load_Widget * Image_Load_Widget_Factory::get(Image_Load * plugin) const
{
    if (plugin)
    {
        const String name = plugin->name();

        const List<Plugin *> & plugins = this->plugins();

        for (size_t i = 0; i < plugins.size(); ++i)
        {
            if (Image_Load_Widget * tmp =
                        dynamic_cast<Image_Load_Widget *>(plugins[i]))
            {
                if (tmp->name() == name)
                {
                    Image_Load_Widget * ui_plugin =
                        static_cast<Image_Load_Widget *>(tmp->copy());

                    ui_plugin->set(plugin);

                    ui_plugin->signal.set(
                        const_cast<Image_Load_Widget_Factory *>(this),
                        callback);

                    return ui_plugin;
                }
            }
        }
    }

    return 0;
}

Image_Load_Widget_Factory * Image_Load_Widget_Factory::global()
{
    static Image_Load_Widget_Factory * global = 0;

    if (! global)
    {
        global = new Image_Load_Widget_Factory;
    }

    return global;
}

void Image_Load_Widget_Factory::callback(Image_Io_Base * plugin)
{
    signal.emit(plugin);
}

//------------------------------------------------------------------------------
// Image_Save_Widget_Factory
//------------------------------------------------------------------------------

Image_Save_Widget_Factory::Image_Save_Widget_Factory(
    const List<String> & search_path)
    throw (Error) :
    Plugin_Factory(
        search_path.size() ? search_path : System::search_path(),
        "djv_image_save_widget"),
    signal(this)
{
    //DJV_DEBUG("Image_Save_Widget_Factory::Image_Save_Widget_Factory");
}

Image_Save_Widget_Factory::~Image_Save_Widget_Factory()
{
    //DJV_DEBUG("Image_Save_Widget_Factory::~Image_Save_Widget_Factory");

    const List<Plugin *> & plugins = this->plugins();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        if (Image_Save_Widget * plugin =
                    dynamic_cast<Image_Save_Widget *>(plugins[i]))
        {
            plugin->Widget::del();
        }
    }
}

Image_Save_Widget * Image_Save_Widget_Factory::get(Image_Save * plugin) const
{
    if (plugin)
    {
        const String name = plugin->name();

        const List<Plugin *> & plugins = this->plugins();

        for (size_t i = 0; i < plugins.size(); ++i)
        {
            if (Image_Save_Widget * tmp =
                        dynamic_cast<Image_Save_Widget *>(plugins[i]))
            {
                if (tmp->name() == name)
                {
                    Image_Save_Widget * ui_plugin =
                        static_cast<Image_Save_Widget *>(tmp->copy());

                    ui_plugin->set(plugin);

                    ui_plugin->signal.set(
                        const_cast<Image_Save_Widget_Factory *>(this),
                        callback);

                    return ui_plugin;
                }
            }
        }
    }

    return 0;
}

Image_Save_Widget_Factory * Image_Save_Widget_Factory::global()
{
    static Image_Save_Widget_Factory * global = 0;

    if (! global)
    {
        global = new Image_Save_Widget_Factory;
    }

    return global;
}

void Image_Save_Widget_Factory::callback(Image_Io_Base * plugin)
{
    signal.emit(plugin);
}

//------------------------------------------------------------------------------
// Image_Io_Prefs_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_base = "Base",
    label_load = "Load",
    label_save = "Save";

} // namespace

Image_Io_Prefs_Widget::Image_Io_Prefs_Widget()
{
    //DJV_DEBUG("Image_Io_Prefs_Widget::Image_Io_Prefs_Widget");

    // Create widgets.

    Tabs * tabs = new Tabs;

    Tabs * tabs_base = new Tabs;
    tabs->tab_add(label_base, tabs_base);

    const List<Plugin *> & base =
        Image_Io_Base_Factory::global()->plugins();

    //DJV_DEBUG_PRINT("base = " << base.size());

    for (size_t i = 0; i < base.size(); ++i)
    {
        if (Image_Io_Base * plugin = dynamic_cast<Image_Io_Base *>(base[i]))
        {
            if (Image_Io_Base_Widget * widget =
                        Image_Io_Base_Widget_Factory::global()->get(plugin))
            {
                tabs_base->tab_add(base[i]->name(), widget);
            }
        }
    }

    Tabs * tabs_load = new Tabs;
    tabs->tab_add(label_load, tabs_load);

    const List<Plugin *> & load =
        Image_Load_Factory::global()->plugins();

    //DJV_DEBUG_PRINT("load = " << load.size());

    for (size_t i = 0; i < load.size(); ++i)
    {
        if (Image_Load * plugin = dynamic_cast<Image_Load *>(load[i]))
        {
            if (Image_Load_Widget * widget =
                        Image_Load_Widget_Factory::global()->get(plugin))
            {
                tabs_load->tab_add(load[i]->name(), widget);
            }
        }
    }

    Tabs * tabs_save = new Tabs;
    tabs->tab_add(label_save, tabs_save);

    //DJV_DEBUG_PRINT("save = " << save.size());

    const List<Plugin *> & save =
        Image_Save_Factory::global()->plugins();

    for (size_t i = 0; i < save.size(); ++i)
    {
        if (Image_Save * plugin = dynamic_cast<Image_Save *>(save[i]))
        {
            if (Image_Save_Widget * widget =
                        Image_Save_Widget_Factory::global()->get(plugin))
            {
                tabs_save->tab_add(save[i]->name(), widget);
            }
        }
    }

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(0);
    layout->add(tabs);
}

Image_Io_Prefs_Widget * Image_Io_Prefs_Widget::global()
{
    static Image_Io_Prefs_Widget * global = 0;

    if (! global)
    {
        global = new Image_Io_Prefs_Widget;
    }

    return global;
}

} // djv
