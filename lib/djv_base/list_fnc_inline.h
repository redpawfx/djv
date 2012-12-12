/*------------------------------------------------------------------------------
 include/djv_base/list_fnc_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace list {

//------------------------------------------------------------------------------
// find()
//------------------------------------------------------------------------------

template<typename T>
inline int find(const T & in, const List<T> & list)
{
  int out = -1;
  
  const size_type size = list.size();
  for (size_type i = 0; i < size; ++i)
    if (in == list[i])
    {
      out = static_cast<int>(i);
      break;
    }
  
  return out;
}

//------------------------------------------------------------------------------
// unique()
//------------------------------------------------------------------------------

template<typename T>
inline List<T> unique(const List<T> & in)
{
  List<T> out;
  
  for (
    typename List<T>::const_iterator i = in.begin(), end = in.end();
    i != end;
    ++i
  )
    if (-1 == find(*i, out))
      out.add(*i);
  
  return out;
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

template<typename A, typename B>
inline void convert(const List<A> & in, List<B> & out)
{
  const size_type size = in.size();
  
  out.resize(size);
  
  for (size_type i = 0; i < size; ++i)
    out[i] = static_cast<B>(in[i]);
}

template<typename A, typename B>
inline void convert_append(const List<A> & in, List<B> & out)
{
  const size_type in_size = in.size();
  const size_type out_size = out.size();
  
  out.resize(in_size + out_size);
  
  for (size_type i = 0; i < in_size; ++i)
    out[out_size + i] = static_cast<B>(in[i]);
}

//------------------------------------------------------------------------------

}
}

