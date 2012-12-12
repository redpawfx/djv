/*------------------------------------------------------------------------------
 lib/djv_image/io.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG
//#define _X_PLUGIN

#include "io.h"

#if defined(DJV_BUILD_MINIMAL)
#include <djv_image/io_ppm.h>
#else
#include <djv_image/io_cineon.h>
#include <djv_image/io_dpx.h>
#include <djv_image/io_iff.h>
#include <djv_image/io_ifl.h>
#include <djv_image/io_lut.h>
#include <djv_image/io_pic.h>
#include <djv_image/io_ppm.h>
#include <djv_image/io_rla.h>
#include <djv_image/io_sgi.h>
#include <djv_image/io_targa.h>
#endif
#include <djv_base/system.h>
#include <algorithm>

namespace djv_image {
namespace io {

//------------------------------------------------------------------------------
// Info
//------------------------------------------------------------------------------

Info::Info() :
  _info(1)
{}

Info::Info(const data::Info & in) :
  data::Info(in),
  _info(1)
{}
  
void Info::layer_add(const data::Info & in)
{
  _info += in;
}

void Info::layer_size(list::size_type in)
{
  _info.resize(in);
}

void Info::layer_clear()
{
  _info.resize(1);
}
 
list::size_type Info::layer_size() const
{
  return _info.size();
}

data::Info & Info::operator [] (list::size_type in)
{
  return 0 == in ? static_cast<data::Info &>(*this) : _info[in];
}

const data::Info & Info::operator [] (list::size_type in) const
{
  return 0 == in ? static_cast<const data::Info &>(*this) : _info[in];
}

//------------------------------------------------------------------------------
// Frame_Info
//------------------------------------------------------------------------------

Frame_Info::Frame_Info(
  int64_t frame,
  int layer,
  data::PROXY proxy
) :
  frame(frame),
  layer(layer),
  proxy(proxy)
{}

//------------------------------------------------------------------------------
// Base
//------------------------------------------------------------------------------

List<String> Base::extension_list() const
{
  return List<String>();
}
  
bool Base::is_seq() const
{
  return true;
}
  
bool Base::option(const String &, String *)
{
  return false;
}
  
String Base::option(const String &) const
{
  return String();
}
  
List<String> Base::option_list() const
{
  return List<String>();
}
  
List<String> Base::option_value(const String &) const
{
  return List<String>();
}
  
String Base::option_default(const String &) const
{
  return String();
}

//------------------------------------------------------------------------------
// Load::Frame_Cache
//------------------------------------------------------------------------------

Load::Frame_Cache::Frame_Cache()
{}

Load::Frame_Cache::Frame_Cache(
  const String & file_name,
  const Frame_Info & info
) :
  file(file_name),
  info(info)
{}

bool Load::Frame_Cache::operator == (const Frame_Cache & in) const
{
  return
    in.file == file &&
    in.info == info;
};

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::~Save()
{
  close();
}

void Save::close() throw (Error)
{}

//------------------------------------------------------------------------------
// Base_Factory
//------------------------------------------------------------------------------

Base_Factory::Base_Factory()
{
  DJV_DEBUG("Base_Factory::Base_Factory");
  
  // Add built-in image plugins.
  
  // Search for image plugins.
  
  const List<String> search_path = djv_base::system::search_path();
  
  DJV_DEBUG_PRINT("search path = " << search_path);
  
#if ! defined(_X_PLUGIN)
  set(search_path);
#endif
}

Base_Factory::~Base_Factory()
{
  DJV_DEBUG("Base_Factory::~Base_Factory");
}

bool Base_Factory::option(
  const String & plugin,
  const String & option,
  String * data
) {
  DJV_DEBUG("Base_Factory::option");
  DJV_DEBUG_PRINT("plugin = " << plugin);
  DJV_DEBUG_PRINT("option = " << option);
  DJV_DEBUG_PRINT("data = " << *data);
  
  const List<Base *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
    if (string::compare_no_case(plugin, list[i]->name()))
    {
      const List<String> option_list = list[i]->option_list();
      
      for (list::size_type j = 0; j < option_list.size(); ++j)
        if (string::compare_no_case(option, option_list[j]))
          return list[i]->option(option, data);
    }
  
  return false;
}

List<Base *> Base_Factory::list() const
{
  List<Base *> out;
  
  list::convert(plugin::Factory::list(), out);
  
  return out;
}

//------------------------------------------------------------------------------
// Load_Factory
//------------------------------------------------------------------------------

Load_Factory::Load_Factory()
{
  DJV_DEBUG("Load_Factory::Load_Factory");
  
  // Add built-in image plugins.

#if defined(DJV_BUILD_MINIMAL)
  add(new io_ppm::Load);
#else
  add(new io_cineon::Load);
  add(new io_dpx::Load);
  add(new io_iff::Load);
  add(new io_ifl::Load);
  add(new io_lut::Load);
  add(new io_pic::Load);
  add(new io_ppm::Load);
  add(new io_rla::Load);
  add(new io_sgi::Load);
  add(new io_targa::Load);
#endif

  // Search for image plugins.
  
#if ! defined(_X_PLUGIN)
  set(djv_base::system::search_path());
#endif
}

Load_Factory::~Load_Factory()
{
  DJV_DEBUG("Load_Factory::~Load_Factory");
}

bool Load_Factory::is_valid(const File & in, const String & plugin) const
{
  const List<Load *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    if (plugin.size())
      if (! string::compare_no_case(plugin, list[i]->name()))
        continue;
    
    if (string::find_no_case(in.extension(), list[i]->extension_list()))
      return true;
  }
  
  return false;
}

Load * Load_Factory::get(
  const File & in,
  Info * info,
  const String & plugin
) const throw (Error) {

  DJV_DEBUG("Load_Factory::get");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("extension = " << in.extension());
  DJV_DEBUG_PRINT("plugin = " << plugin);

  const List<Load *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    if (plugin.size())
    {
      if (! string::compare_no_case(plugin, list[i]->name()))
        continue;
    }
    else
    {
      if (! string::find_no_case(in.extension(), list[i]->extension_list()))
        continue;
    }
    
    std::auto_ptr<Load> plugin(static_cast<Load *>(list[i]->copy()));
    
    Info _info;
    plugin->open(in, _info);
    if (info)
      *info = _info;
    
    return plugin.release();
  }
  
  throw Error(error, string::Format(error_unrecognized).arg(in));
  return 0;
}

bool Load_Factory::option(
  const String & plugin,
  const String & option,
  String * data
) {
  DJV_DEBUG("Load_Factory::option");
  DJV_DEBUG_PRINT("plugin = " << plugin);
  DJV_DEBUG_PRINT("option = " << option);
  DJV_DEBUG_PRINT("data = " << *data);
  
  const List<Load *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
    if (string::compare_no_case(plugin, list[i]->name()))
    {
      const List<String> option_list = list[i]->option_list();
      
      for (list::size_type j = 0; j < option_list.size(); ++j)
        if (string::compare_no_case(option, option_list[j]))
          return list[i]->option(option, data);
    }
  
  return false;
}

List<Load *> Load_Factory::list() const
{
  List<Load *> out;
  
  list::convert(plugin::Factory::list(), out);
  
  return out;
}

//------------------------------------------------------------------------------
// Save_Factory
//------------------------------------------------------------------------------

Save_Factory::Save_Factory()
{
  DJV_DEBUG("Save_Factory::Save_Factory");
  
  // Add built-in image plugins.
  
#if defined(DJV_BUILD_MINIMAL)
  add(new io_ppm::Save);
#else
  add(new io_cineon::Save);
  add(new io_dpx::Save);
  add(new io_iff::Save);
  add(new io_lut::Save);
  add(new io_ppm::Save);
  add(new io_sgi::Save);
  add(new io_targa::Save);
#endif

  // Search for image plugins.
  
#if ! defined(_X_PLUGIN)
  set(djv_base::system::search_path());
#endif
}

Save_Factory::~Save_Factory()
{
  DJV_DEBUG("Save_Factory::~Save_Factory");
}

Save * Save_Factory::get(
  const File & in,
  const Info & info,
  const String & plugin
) const throw (Error) {

  DJV_DEBUG("Save_Factory::get");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("info = " << info);
  
  const List<Save *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    if (plugin.size())
    {
      if (! string::compare_no_case(plugin, list[i]->name()))
        continue;
    }
    else
    {
      if (! string::find_no_case(in.extension(), list[i]->extension_list()))
        continue;
    }
    
    std::auto_ptr<Save> plugin(static_cast<Save *>(list[i]->copy()));
    plugin->open(in, info);
    return plugin.release();
  }

  throw Error(error, string::Format(error_unrecognized).arg(in));
  return 0;
}

bool Save_Factory::option(
  const String & plugin,
  const String & option,
  String * data
) {
  DJV_DEBUG("Save_Factory::option");
  DJV_DEBUG_PRINT("plugin = " << plugin);
  DJV_DEBUG_PRINT("option = " << option);
  DJV_DEBUG_PRINT("data = " << *data);

  const List<Save *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
    if (string::compare_no_case(plugin, list[i]->name()))
    {
      const List<String> option_list = list[i]->option_list();
      
      for (list::size_type j = 0; j < option_list.size(); ++j)
        if (string::compare_no_case(option, option_list[j]))
          return list[i]->option(option, data);
    }
  
  return false;
}

List<Save *> Save_Factory::list() const
{
  List<Save *> out;
  
  list::convert(plugin::Factory::list(), out);
  
  return out;
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() :
  _base(std::auto_ptr<Base_Factory>(new Base_Factory)),
  _load(std::auto_ptr<Load_Factory>(new Load_Factory)),
  _save(std::auto_ptr<Save_Factory>(new Save_Factory))
{
  DJV_DEBUG("Global::Global");

  // Set file sequence extension list.
  
  Set<String> seq_extension_list;
  
  const List<Base *> & list = this->list();

  for (list::size_type i = 0; i < list.size(); ++i)
    if (list[i]->is_seq())
      seq_extension_list += list[i]->extension_list();
  
  DJV_DEBUG_PRINT("seq extension list = " << seq_extension_list);

  file::File::seq_extension_list(seq_extension_list);
}

Global::~Global()
{
  DJV_DEBUG("Global::~Global");
}

bool Global::is_valid(const File & in, const String & plugin) const
{
  return _load->is_valid(in, plugin);
}

Load * Global::load(
  const File & in,
  Info * info,
  const String & plugin
) const throw (Error) {
  return _load->get(in, info, plugin);
}

Save * Global::save(
  const File & in,
  const Info & info,
  const String & plugin
) const throw (Error) {
  return _save->get(in, info, plugin);
}

Base_Factory * Global::base() const
{
  return _base.get();
}

Load_Factory * Global::load() const
{
  return _load.get();
}

Save_Factory * Global::save() const
{
  return _save.get();
}

List<Base *> Global::list() const
{
  List<Base *> out;
  
  list::convert_append(_base->list(), out);
  list::convert_append(_load->list(), out);
  list::convert_append(_save->list(), out);
  
  return out;
}

List<Base *> Global::base_list() const
{
  List<Base *> out;
  
  list::convert_append(_base->list(), out);
  
  return out;
}

List<Base *> Global::load_list() const
{
  List<Base *> out;
  
  list::convert_append(_load->list(), out);
  
  return out;
}

List<Base *> Global::save_list() const
{
  List<Base *> out;
  
  list::convert_append(_save->list(), out);
  
  return out;
}

Global::Group::Group(
  const String & name,
  Base * base,
  Load * load,
  Save * save
) :
  name(name),
  base(base),
  load(load),
  save(save)
{}

List<Global::Group> Global::group_list() const
{
  List<Group> out;
  
  List<plugin::Plugin *> base_list;
  List<plugin::Plugin *> load_list;
  List<plugin::Plugin *> save_list;
  
  list::convert_append(_base->list(), base_list);
  list::convert_append(_load->list(), load_list);
  list::convert_append(_save->list(), save_list);
  
  List<String> name_list;
  group_name(base_list, name_list);
  group_name(load_list, name_list);
  group_name(save_list, name_list);
  
  for (list::size_type i = 0; i < name_list.size(); ++i)
    out += Group(
      name_list[i],
      dynamic_cast<Base *>(group_find(name_list[i], base_list)),
      dynamic_cast<Load *>(group_find(name_list[i], load_list)),
      dynamic_cast<Save *>(group_find(name_list[i], save_list))
    );
  
  return out;
}

//------------------------------------------------------------------------------
// group_name()
//------------------------------------------------------------------------------

void group_name(
  const List<plugin::Plugin *> & in,
  List<String> & out
) {
  for (list::size_type i = 0; i < in.size(); ++i)
  {
    const String & name = in[i]->name();
    if (std::find(out.begin(), out.end(), name) == out.end())
      out += name;
  }
}

//------------------------------------------------------------------------------
// group_find()
//------------------------------------------------------------------------------

plugin::Plugin * group_find(
  const String & name,
  const List<plugin::Plugin *> & in
) {
  for (list::size_type i = 0; i < in.size(); ++i)
    if (name == in[i]->name())
      return in[i];
  
  return 0;
}

//------------------------------------------------------------------------------

}
}
