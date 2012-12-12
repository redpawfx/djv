/*------------------------------------------------------------------------------
 include/djv_base/list_fnc.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace list {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Find the index of an item within a list. If no match is found -1 is
//! returned.

template<typename T>
inline int find(const T &, const List<T> &);

//! Return the unique items in a list.

template<typename T>
inline List<T> unique(const List<T> &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

template<typename A, typename B>
inline void convert(const List<A> &, List<B> &);

template<typename A, typename B>
inline void convert_append(const List<A> &, List<B> &);

//@}
//------------------------------------------------------------------------------

}
}

