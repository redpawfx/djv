/*------------------------------------------------------------------------------
 plugin/djv_openexr_base/base.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "base.h"

#include <ImfThreading.h>

namespace djv_plugin {
namespace openexr {

//------------------------------------------------------------------------------
// djv_image_io()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_io()
{
  return new Base;
}

}

//------------------------------------------------------------------------------
// Base
//------------------------------------------------------------------------------

Base::Base()
{
  update_thread_count();
}

plugin::Plugin * Base::copy() const
{
  Base * out = new Base;
  out->_option = _option;
  return out;
}

String Base::name() const
{
  return openexr::name;
}

List<String> Base::extension_list() const
{
  return openexr::extension_list;
}

bool Base::option(const String & in, String * data)
{
  const List<String> & list = option_list();
  
  try
  {
    if (string::compare_no_case(in, list[BASE_THREAD_COUNT]))
    {
      *data >> _option.thread_count;
      
      update_thread_count();
    }
  }
  catch (String)
  {
    return false;
  }
  
  return true;
}

String Base::option(const String & in) const
{
  const List<String> & list = option_list();

  String out;
  
  if (string::compare_no_case(in, list[BASE_THREAD_COUNT]))
    out << _option.thread_count;
  
  return out;
}

List<String> Base::option_list() const
{
  return label_base_option();
}

List<String> Base::option_value(const String & in) const
{
  List<String> out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_base_option(), &index))
    switch (index)
    {
      case BASE_THREAD_COUNT: out << label_thread_count_value; break;
    }
  
  return out;
}

String Base::option_default(const String & in) const
{
  String out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_base_option(), &index))
    switch (index)
    {
      case BASE_THREAD_COUNT: out << _option.thread_count; break;
    }
  
  return out;
}

void Base::update_thread_count()
{
  Imf::setGlobalThreadCount(_option.thread_count);
}

//------------------------------------------------------------------------------

}
}

