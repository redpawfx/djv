/*------------------------------------------------------------------------------
 lib/djv_ui/image_io_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_io.h"

namespace djv_ui {
namespace image_io {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  prefs_title = "Image I/O",
  prefs_base_title = "General",
  prefs_load_title = "Load",
  prefs_save_title = "Save";

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() :
  signal(this),
  _base(new Base_Factory),
  _load(new Load_Factory),
  _save(new Save_Factory)
{
  DJV_DEBUG("Global::Global");
}

Global::~Global()
{
  DJV_DEBUG("Global::~Global");
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

List<Base *> Global::plugin_list() const
{
  List<Base *> out;
  
  list::convert_append(_base->list(), out);
  list::convert_append(_load->list(), out);
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
  djv_image::io::group_name(base_list, name_list);
  djv_image::io::group_name(load_list, name_list);
  djv_image::io::group_name(save_list, name_list);
  
  for (list::size_type i = 0; i < name_list.size(); ++i)
    out += Group(
      name_list[i],
      dynamic_cast<Base *>(djv_image::io::group_find(name_list[i], base_list)),
      dynamic_cast<Load *>(djv_image::io::group_find(name_list[i], load_list)),
      dynamic_cast<Save *>(djv_image::io::group_find(name_list[i], save_list))
    );
  
  return out;
}

//------------------------------------------------------------------------------

}
}

