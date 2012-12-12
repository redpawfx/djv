/*------------------------------------------------------------------------------
 include/djv_base/debug_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace debug {

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

template<class T>
inline Debug & operator << (Debug & debug, const List<T> & in)
{
  for (list::size_type i = 0; i < in.size(); ++i)
  {
    if (i)
      debug << " ";
    
    debug << in[i];
  }
  
  return debug;
}

template<class T>
inline Debug & operator << (Debug & debug, const Set<T> & in)
{
  bool space = false;
  
  for (typename Set<T>::const_iterator i = in.begin(); i != in.end(); ++i)
  {
    if (space)
      debug << " ";
    
    debug << *i;
    
    space = true;
  }
  
  return debug;
}

//------------------------------------------------------------------------------

}
}

