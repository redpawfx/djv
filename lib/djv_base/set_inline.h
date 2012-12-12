/*------------------------------------------------------------------------------
 include/djv_base/set_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
//namespace set {

//------------------------------------------------------------------------------
// Set
//------------------------------------------------------------------------------

template<typename T>
inline Set<T>::Set()
{}

template<typename T>
inline Set<T>::Set(const Set<T> & in) :
  std::set<T>(in)
{}

template<typename T>
inline void Set<T>::add(const T & in)
{
  Set<T>::insert(in);
}

template<typename T>
inline void Set<T>::add(const Set<T> & in)
{
  for (
    typename Set<T>::const_iterator i = in.begin(), end = in.end();
    i != end;
    ++i
  )
    Set<T>::insert(*i);
}

template<typename T>
inline void Set<T>::add(const List<T> & in)
{
  for (
    typename List<T>::const_iterator i = in.begin(), end = in.end();
    i != end;
    ++i
  )
    Set<T>::insert(*i);
}

template<typename T>
inline Set<T> & Set<T>::operator += (const T & in)
{
  add(in);
  
  return *this;
}

template<typename T>
inline Set<T> & Set<T>::operator += (const Set<T> & in)
{
  add(in);
  
  return *this;
}

template<typename T>
inline Set<T> & Set<T>::operator += (const List<T> & in)
{
  add(in);
  
  return *this;
}

template<typename T>
inline Set<T> & Set<T>::operator << (const T & in)
{
  add(in);
  
  return *this;
}

template<typename T>
inline Set<T> & Set<T>::operator << (const Set<T> & in)
{
  add(in);
  
  return *this;
}

template<typename T>
inline Set<T>::operator List<T> () const
{
  List<T> out;
  
  for (
    typename Set<T>::const_iterator i = Set<T>::begin(), end = Set<T>::end();
    i != end;
    ++i
  )
    out.add(*i);
  
  return out;
}

//------------------------------------------------------------------------------

//}
}

