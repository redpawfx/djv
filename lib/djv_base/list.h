/*------------------------------------------------------------------------------
 include/djv_base/list.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_LIST_H
#define DJV_LIST_H

#include <djv_base/list_def.h>

#include "config.h"
#include <djv_base/export.h>
#include <djv_base/type.h>
#include <vector>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::list
//
//! Lists.
//------------------------------------------------------------------------------

namespace list {

typedef size_t size_type;

}

//------------------------------------------------------------------------------
//!\class List
//
//! A list ot items. Inherited from std::vector.
//------------------------------------------------------------------------------

template<typename T>
class List : public std::vector<T>
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline List();
  inline List(const List<T> &);
  inline List(const List<T> &, list::size_type position, list::size_type size);
  inline List(const T &, list::size_type size);
  inline List(list::size_type size);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Push/Pop Items
  //@{

  inline void push_front(const T &);
  
  inline T pop_front();
  inline T pop_back();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Add Items
  //@{
  
  inline void add(const T &);
  inline void add(const List<T> &);

  inline List<T> & operator += (const T &);
  inline List<T> & operator += (const List<T> &);
  
  inline List<T> & operator << (const T &);
  inline List<T> & operator << (const List<T> &);
  
  //@}
};

//@}
//------------------------------------------------------------------------------

//}
}

#include <djv_base/list_fnc.h>
#include <djv_base/list_fnc_inline.h>
#include <djv_base/list_inline.h>
#endif

