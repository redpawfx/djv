/*------------------------------------------------------------------------------
 lib/djv_ui/prefs_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_ui {
namespace prefs {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

template<typename T>
inline void set(Prefs * prefs, const String & name, const T & in)
{
  String tmp;
  prefs->set(name, tmp << in);
}

template<typename T>
inline bool get(Prefs * prefs, const String & name, T * out)
{
  String tmp;
  if (! prefs->get(name, &tmp))
    return false;
  
  try
  {
    tmp >> *out;
  }
  catch (String)
  {
    return false;
  }
  
  return true;
}

//------------------------------------------------------------------------------

}
}
