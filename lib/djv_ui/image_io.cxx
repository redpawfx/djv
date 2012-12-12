/*------------------------------------------------------------------------------
 lib/djv_ui/image_io.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG
//#define _X_PLUGIN

#include "image_io.h"

#if defined(DJV_BUILD_MINIMAL)
#include <djv_ui/image_io_ppm.h>
#else
#include <djv_ui/image_io_cineon.h>
#include <djv_ui/image_io_iff.h>
#include <djv_ui/image_io_dpx.h>
#include <djv_ui/image_io_lut.h>
#include <djv_ui/image_io_ppm.h>
#include <djv_ui/image_io_sgi.h>
#include <djv_ui/image_io_targa.h>
#endif
#include <djv_base/system.h>

namespace djv_ui {
namespace image_io {

//------------------------------------------------------------------------------
// Base
//------------------------------------------------------------------------------

Base::Base() :
  signal(this)
{}

Base::~Base()
{}

//------------------------------------------------------------------------------
// Base_Factory
//------------------------------------------------------------------------------

Base_Factory::Base_Factory()
{
  DJV_DEBUG("Base_Factory::Base_Factory");
  
#if ! defined(_X_PLUGIN)
  set(djv_base::system::search_path());
#endif
}

Base_Factory::~Base_Factory()
{
  DJV_DEBUG("Base_Factory::~Base_Factory");
  
  const List<Base *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
    list[i]->widget::Widget::del();
}

Base * Base_Factory::get(djv_image::io::Base * in) const
{
  if (in)
  {
    const List<plugin::Plugin *> & list = Factory::list();
  
    const String name = in->name();
  
    for (list::size_type i = 0; i < list.size(); ++i)
      if (list[i]->name() == name)
        return static_cast<Base *>(list[i]->copy());
  }
  
  return 0;
}

List<Base *> Base_Factory::list() const
{
  const List<plugin::Plugin *> & list = Factory::list();
  
  List<Base *> out;
  
  list::convert(Factory::list(), out);
  
  return out;
}

//------------------------------------------------------------------------------
// Load_Factory
//------------------------------------------------------------------------------

Load_Factory::Load_Factory()
{
  DJV_DEBUG("Load_Factory::Load_Factory");

#if defined(DJV_BUILD_MINIMAL)
#else
  add(new image_io_cineon::Load);
  add(new image_io_dpx::Load);
  add(new image_io_lut::Load);
#endif

#if ! defined(_X_PLUGIN)
  set(djv_base::system::search_path());
#endif
}

Load_Factory::~Load_Factory()
{
  DJV_DEBUG("Load_Factory::~Load_Factory");
  
  const List<Load *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
    list[i]->widget::Widget::del();
}

Load * Load_Factory::get(djv_image::io::Load * in) const
{
  if (in)
  {
    const List<plugin::Plugin *> & list = Factory::list();
  
    const String name = in->name();
  
    for (list::size_type i = 0; i < list.size(); ++i)
      if (list[i]->name() == name)
        return static_cast<Load *>(list[i]->copy());
  }
  
  return 0;
}

List<Load *> Load_Factory::list() const
{
  List<Load *> out;
  
  list::convert(Factory::list(), out);
  
  return out;
}

//------------------------------------------------------------------------------
// Save_Factory
//------------------------------------------------------------------------------

Save_Factory::Save_Factory()
{
  DJV_DEBUG("Save_Factory::Save_Factory");

#if defined(DJV_BUILD_MINIMAL)
  add(new image_io_ppm::Save);
#else
  add(new image_io_cineon::Save);
  add(new image_io_dpx::Save);
  add(new image_io_iff::Save);
  add(new image_io_lut::Save);
  add(new image_io_ppm::Save);
  add(new image_io_sgi::Save);
  add(new image_io_targa::Save);
#endif

#if ! defined(_X_PLUGIN)
  set(djv_base::system::search_path());
#endif
}

Save_Factory::~Save_Factory()
{
  DJV_DEBUG("Save_Factory::~Save_Factory");
  
  const List<Save *> & list = this->list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
    list[i]->widget::Widget::del();
}

Save * Save_Factory::get(djv_image::io::Save * in) const
{
  if (in)
  {
    const List<plugin::Plugin *> & list = Factory::list();
  
    const String name = in->name();
  
    for (list::size_type i = 0; i < list.size(); ++i)
      if (list[i]->name() == name)
        return static_cast<Save *>(list[i]->copy());
  }
  
  return 0;
}

List<Save *> Save_Factory::list() const
{
  List<Save *> out;
  
  list::convert(Factory::list(), out);
  
  return out;
}

//------------------------------------------------------------------------------

}
}
