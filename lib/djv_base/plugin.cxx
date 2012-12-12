/*------------------------------------------------------------------------------
 lib/djv_base/plugin.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#undef DJV_DEBUG
//#define DJV_DEBUG

#include "plugin.h"

#include <djv_base/application.h>
#include <djv_base/directory.h>
#include <djv_base/file.h>
#include <algorithm>
#if ! defined(DJV_WINDOWS)
#include <dlfcn.h>
#endif

namespace djv_base {
namespace plugin {

//------------------------------------------------------------------------------
// Plugin
//------------------------------------------------------------------------------

Plugin::~Plugin()
{}

//------------------------------------------------------------------------------
// Factory::Handle
//------------------------------------------------------------------------------

class Factory::Handle
{
public:

  Handle() :
    _p(0)
  {}

  ~Handle()
  {
#if defined(DJV_WINDOWS)
    if (_p) ::FreeLibrary(_p);
#else
#if ! defined(DJV_IRIX) // XXX IRIX dlopen/pthread bug.
    if (_p) ::dlclose(_p);
#endif
#endif
  }
  
  void open(const String & in) throw (Error)
  {
#if defined(DJV_WINDOWS)
    _p = ::LoadLibrary(in.c_str());
#else
    _p = ::dlopen(in.c_str(), RTLD_LAZY);
#endif

    if (! _p)
    {
      String error;
#if defined(DJV_WINDOWS)
      error = error::last_error();
#else
      error = ::dlerror();
#endif
      throw Error(error);
    }
  }
    
  void * fnc(const String & in) const
  {
#if defined(DJV_WINDOWS)
    return (void *)::GetProcAddress(_p, in.c_str());
#else
    return ::dlsym(_p, in.c_str());
#endif
  }
  
private:
  
#if defined(DJV_WINDOWS)
  HINSTANCE _p;
#else
  void * _p;
#endif
};

//------------------------------------------------------------------------------
// Factory
//------------------------------------------------------------------------------

Factory::Factory()
{
  DJV_DEBUG("Factory::Factory");
}

Factory::~Factory()
{
  DJV_DEBUG("Factory::~Factory");
  
  del();
}

void Factory::set(const List<String> & search_path)
{
  //return;

  DJV_DEBUG("Factory::set");
  DJV_DEBUG_PRINT("search path = " << search_path);
  DJV_DEBUG_PRINT("plugin entry = " << plugin_entry());

  //del();

  // XXX OS specific library names.

  List<String> filter;
#if defined(DJV_WINDOWS)
  filter += plugin_prefix() + "*.dll";
#else
  filter += "lib" + plugin_prefix() + "*.so";
#endif
  DJV_DEBUG_PRINT("filter = " << filter);

  // Find plugins.
 
  List<String> list;
  for (list::size_type i = 0; i < search_path.size(); ++i)
  {
    const Directory directory(search_path[i]);
    
    DJV_DEBUG_PRINT("searching = " << directory.get());
    
    List<File> file_list = directory.list();
    file::filter(file_list, file::FILTER_NONE, filter);
    for (list::size_type i = 0; i < file_list.size(); ++i)
    {
      DJV_DEBUG_PRINT("found = " << file_list[i]);
      
      list += file_list[i];
    }
  }
  
  // Load plugins.

  //DJV_DEBUG_PRINT("size = " << list.size());
  
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    DJV_DEBUG_PRINT("loading = " << list[i]);
    
    // Open.

    std::auto_ptr<Handle> handle(new Handle);
    
    try
    {
      handle->open(list[i]);
    }
    catch (Error in)
    {
      DJV_DEBUG_PRINT("plugin error = " << in);
      
      continue;
    }
    
    Plugin_Entry * entry = (Plugin_Entry *)handle->fnc(plugin_entry());
    if (! entry)
    {
      DJV_DEBUG_PRINT("plugin entry point error");
      
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
      application::global()->error(in);
    }
    if (! plugin.get())
    {
      DJV_DEBUG_PRINT("plugin create error");
      
      continue;
    }
    
    DJV_DEBUG_PRINT("name = " << plugin->name());

    // Check for duplicates.
    
    list::size_type j = 0;
    for (; j < _list.size(); ++j)
      if (plugin->name() == _list[j]->name()) break;
    if (j < _list.size())
    {
      DJV_DEBUG_PRINT("duplicate");
      
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
      DJV_DEBUG_PRINT("plugin init error");
      
      plugin->del();
      plugin.reset();
      
      //application::global()->error(in);
      
      continue;
    }
    
    // Add.
    
    add(plugin.release());
    
    _handle += handle.release();
  }
}

void Factory::add(Plugin * in)
{
  DJV_DEBUG("Factory::add");
  DJV_DEBUG_PRINT("in = " << in->name());
  
  _list += in;
}

void Factory::del()
{
  DJV_DEBUG("Factory::del");

  for (list::size_type i = 0; i < _list.size(); ++i)
  {
    _list[i]->del();
    delete _list[i];
  }
  _list.clear();

  for (list::size_type i = 0; i < _handle.size(); ++i)
    delete _handle[i];
  _handle.clear();
}

const List<Plugin *> & Factory::list() const
{
  return _list;
}

List<String> Factory::name_list() const
{
  List<String> out(_list.size());
  for (list::size_type i = 0; i < _list.size(); ++i)
    out[i] = _list[i]->name();
  return out;
}

//------------------------------------------------------------------------------

}
}
