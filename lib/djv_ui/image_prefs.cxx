/*------------------------------------------------------------------------------
 lib/djv_ui/image_prefs.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_prefs.h"

#include <djv_ui/global.h>
#include <djv_ui/prefs.h>
#include <djv_image/io.h>

namespace djv_ui {
namespace image {

//------------------------------------------------------------------------------
// _load(), _save()
//------------------------------------------------------------------------------

namespace {

void _load(const prefs::Prefs & in, djv_image::io::Base * plugin)
{
  prefs::Prefs prefs(in, plugin->name());
  
  const List<String> & option = plugin->option_list();
  for (list::size_type i = 0; i < option.size(); ++i)
  {
    String tmp;
    if (prefs.get(option[i], &tmp))
      plugin->option(option[i], &tmp);
  }
}

void _save(prefs::Prefs * in, const djv_image::io::Base * plugin)
{
  prefs::Prefs prefs(in, plugin->name());
  
  const List<String> & option = plugin->option_list();
  for (list::size_type i = 0; i < option.size(); ++i)
    prefs.set(option[i], plugin->option(option[i]));
}

}

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs()
{
  DJV_DEBUG("Prefs::Prefs");
  
  prefs::Prefs prefs(prefs::prefs_global(), "image");
  
  djv_image::gl_data::Filter filter;
  djv_image::time::FPS fps(djv_image::time::FPS(0));
  djv_image::time::UNITS units(djv_image::time::UNITS(0));
  
  if (prefs::get(&prefs, "filter", &filter))
    djv_image::gl_data::global()->filter(filter);
  if (prefs::get(&prefs, "speed", &fps))
    djv_image::time::global()->speed(fps);
  if (prefs::get(&prefs, "units", &units))
    djv_image::time::global()->units(units);

  prefs::Prefs base(prefs, "base");
  const List<djv_image::io::Base *> & base_list =
    djv_image::io::global()->base()->list();
  for (list::size_type i = 0; i < base_list.size(); ++i)
    _load(base, base_list[i]);

  prefs::Prefs load(prefs, "load");
  const List<djv_image::io::Load *> & load_list =
    djv_image::io::global()->load()->list();
  for (list::size_type i = 0; i < load_list.size(); ++i)
    _load(load, load_list[i]);
  
  prefs::Prefs save(prefs, "save");
  const List<djv_image::io::Save *> & save_list =
    djv_image::io::global()->save()->list();
  for (list::size_type i = 0; i < save_list.size(); ++i)
    _load(save, save_list[i]);
}

Prefs::~Prefs()
{
  DJV_DEBUG("Prefs::~Prefs");

  prefs::Prefs prefs(prefs::prefs_global(), "image");
  
  prefs::set(&prefs, "filter", djv_image::gl_data::global()->filter());
  prefs::set(&prefs, "speed", djv_image::time::global()->speed());
  prefs::set(&prefs, "units", djv_image::time::global()->units());

  prefs::Prefs base(prefs, "base");
  const List<djv_image::io::Base *> & base_list =
    djv_image::io::global()->base()->list();
  for (list::size_type i = 0; i < base_list.size(); ++i)
    _save(&base, base_list[i]);

  prefs::Prefs load(prefs, "load");
  const List<djv_image::io::Load *> & load_list =
    djv_image::io::global()->load()->list();
  for (list::size_type i = 0; i < load_list.size(); ++i)
    _save(&load, load_list[i]);
  
  prefs::Prefs save(prefs, "save");
  const List<djv_image::io::Save *> & save_list =
    djv_image::io::global()->save()->list();
  for (list::size_type i = 0; i < save_list.size(); ++i)
    _save(&save, save_list[i]);
}

void Prefs::filter(const djv_image::gl_data::Filter & in)
{
  if (in == filter())
    return;
  
  djv_image::gl_data::global()->filter(in);
  
  djv_ui::global()->window_update();
}

const djv_image::gl_data::Filter & Prefs::filter() const
{
  return djv_image::gl_data::global()->filter();
}

void Prefs::speed(djv_image::time::FPS in)
{
  if (in == speed())
    return;
  
  djv_image::time::global()->speed(in);
  
  djv_ui::global()->window_update();
}

djv_image::time::FPS Prefs::speed() const
{
  return djv_image::time::global()->speed();
}

void Prefs::units(djv_image::time::UNITS in)
{
  if (in == units())
    return;
  
  djv_image::time::global()->units(in);
  
  djv_ui::global()->window_update();
}

djv_image::time::UNITS Prefs::units() const
{
  return djv_image::time::global()->units();
}

//------------------------------------------------------------------------------

}
}
