/*------------------------------------------------------------------------------
 bin/djv_view/window_factory.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "window.h"

#include "application.h"
#include <algorithm>

namespace djv_bin {
namespace view {
namespace window {

//------------------------------------------------------------------------------
// Factory
//------------------------------------------------------------------------------

Factory::~Factory()
{
  DJV_DEBUG("window::Factory::~Factory");
  
  for (list::size_type i = 0; i < _list.size(); ++i)
    _list[i]->del();
}

Window * Factory::add(const Window * copy)
{
  DJV_DEBUG("window::Factory::add");
  DJV_DEBUG_PRINT("copy = " << static_cast<int>(copy));
  
  Window * window = new Window(copy);
  _list += window;
  return window;
}

void Factory::del(Window * in)
{
  DJV_DEBUG("window::Factory::del");
  DJV_DEBUG_PRINT("in = " << static_cast<int>(in));
  
  in->del();
  
  _list.erase(std::find(_list.begin(), _list.end(), in));  

  // Exit application if no windows are left.
  
  if (! _list.size())
    application::global()->exit();
}

const List<Window *> Factory::list() const
{
  return _list;
}

//------------------------------------------------------------------------------

}
}
}

