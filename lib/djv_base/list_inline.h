/*------------------------------------------------------------------------------
 include/djv_base/list_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
//namespace list {

//------------------------------------------------------------------------------
// List
//------------------------------------------------------------------------------

template<typename T>
inline List<T>::List()
{}

template<typename T>
inline List<T>::List(const List<T> & in) :
  std::vector<T>(in)
{}

template<typename T>
inline List<T>::List(
  const List<T> & in,
  list::size_type position,
  list::size_type size
) :
  std::vector<T>(size)
{
  const list::size_type in_size = in.size();
  for (list::size_type i = 0, j = position; i < size && j < in_size; ++i, ++j)
    (*this)[i] = in[j];
}

template<typename T>
inline List<T>::List(const T & in, list::size_type size) :
  std::vector<T>(size, in)
{}

template<typename T>
inline List<T>::List(list::size_type size) :
  std::vector<T>(size)
{}

template<typename T>
inline void List<T>::push_front(const T & in)
{
  insert(List<T>::begin(), in);
}

template<typename T>
inline T List<T>::pop_front()
{
  const T out = List<T>::front();
  
  erase(List<T>::begin());
  
  return out;
}

template<typename T>
inline T List<T>::pop_back()
{
  const T out = List<T>::back();
  
  std::vector<T>::pop_back();
  
  return out;
}

template<typename T>
inline void List<T>::add(const T & in)
{
  push_back(in);
}

template<typename T>
inline void List<T>::add(const List<T> & in)
{
  for (
    typename List<T>::const_iterator i = in.begin(), end = in.end();
    i != end;
    ++i
  )
    push_back(*i);
}

template<typename T>
inline List<T> & List<T>::operator += (const T & in)
{
  add(in);
  
  return *this;
}

template<typename T>
inline List<T> & List<T>::operator += (const List<T> & in)
{
  add(in);
  
  return *this;
}

template<typename T>
inline List<T> & List<T>::operator << (const T & in)
{
  add(in);
  
  return *this;
}

template<typename T>
inline List<T> & List<T>::operator << (const List<T> & in)
{
  add(in);
  
  return *this;
}

//------------------------------------------------------------------------------

//}
}

