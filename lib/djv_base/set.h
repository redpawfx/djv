/*------------------------------------------------------------------------------
 include/djv_base/set.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_SET_H
#define DJV_SET_H

#include <djv_base/set_def.h>

#include <djv_base/list.h>
#include <set>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::set
//
//! Sets.
//------------------------------------------------------------------------------

namespace set {}

//------------------------------------------------------------------------------
//!\class Set
//
//! A set of items. Inherited from std::set.
//------------------------------------------------------------------------------

template<typename T>
class Set : public std::set<T>
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline Set();
  inline Set(const Set<T> &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Add Items
  //@{

  inline void add(const T &);
  inline void add(const Set<T> &);
  inline void add(const List<T> &);

  inline Set<T> & operator += (const T &);
  inline Set<T> & operator += (const Set<T> &);
  inline Set<T> & operator += (const List<T> &);
  
  inline Set<T> & operator << (const T &);
  inline Set<T> & operator << (const Set<T> &);
  inline Set<T> & operator << (const List<T> &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Conversion
  //@{
  
  inline operator List<T> () const;
  
  //@}
};

//------------------------------------------------------------------------------

}

#include <djv_base/set_inline.h>
#endif

