/*------------------------------------------------------------------------------
 include/djv_base/plugin.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_PLUGIN_H
#define DJV_PLUGIN_H

#include <djv_base/plugin_def.h>

#include <djv_base/base.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::plugin
//
//! Plugins.
//------------------------------------------------------------------------------

namespace plugin {

//------------------------------------------------------------------------------
//!\class Plugin
//
//! A base class for plugins.
//------------------------------------------------------------------------------

class _DJV_BASE Plugin
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  virtual ~Plugin() = 0;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Initialization
  //@{
  
  virtual void init() throw (Error) {}
  
  virtual void del() {}
  
  virtual Plugin * copy() const = 0;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Information
  //@{
  
  virtual String name() const = 0;
  
  //@}
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
//!\class Factory
//
//! A base class for plugin factories.
//------------------------------------------------------------------------------

class _DJV_BASE Factory
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Factory();
  
  virtual ~Factory();
  
  //@}
  //----------------------------------------------------------------------------
  //\name Functions
  //@{
  
  //! Initialize plugins.
  
  virtual void set(const List<String> & search_path = List<String>());
  
  //! Add a plugin.
  
  virtual void add(Plugin *);
  
  //! Delete all plugins.
  
  virtual void del();
  
  //! Get a list of plugins.
  
  const List<Plugin *> & list() const;
  
  //! Get a list of plugin names.
  
  List<String> name_list() const;

  //@}
  
protected:
  
  //! Plugin file name prefix.
  
  virtual String plugin_prefix() const { return "djv"; }
  
  //! Plugin entry point.
  
  virtual String plugin_entry() const = 0;

private:

  class Handle;
  
  List<Plugin *> _list;
  List<Handle *> _handle;
};

//------------------------------------------------------------------------------

}
}

#endif

