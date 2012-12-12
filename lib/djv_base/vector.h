/*------------------------------------------------------------------------------
 include/djv_base/vector.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VECTOR_H
#define DJV_VECTOR_H

#include <djv_base/vector_def.h>

#include <djv_base/base.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::vector
//
//! Vectors.
//------------------------------------------------------------------------------

namespace vector {

//------------------------------------------------------------------------------
//!\class V
//
//! A base class for vectors.
//------------------------------------------------------------------------------

template<typename T, int D>
struct V
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline V();
  inline V(const V<T, D> &);
  inline V(T);
  
  V<T, D> & operator = (const V<T, D> &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  static const int dimension;
  
  //! Components.
  
  T e [D];
  
  inline void set(const V<T, D> &);
  inline void set(T);
  inline void zero();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{
  
  inline V<T, D> & operator += (const V<T, D> &);
  inline V<T, D> & operator -= (const V<T, D> &);
  inline V<T, D> & operator *= (const V<T, D> &);
  inline V<T, D> & operator /= (const V<T, D> &);

  inline V<T, D> & operator += (T);
  inline V<T, D> & operator -= (T);
  inline V<T, D> & operator *= (T);
  inline V<T, D> & operator /= (T);
  
  //@}
};

//------------------------------------------------------------------------------
//!\class V<T, 2>
//
//! A two-dimensional vector.
//------------------------------------------------------------------------------

template<typename T>
struct V<T, 2>
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline V();
  inline V(const V<T, 2> &);
  inline V(T, T);
  inline V(T);
  
  inline V<T, 2> & operator = (const V<T, 2> &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  static const int dimension;

  T e [2];

  T & x, & y;

  inline void set(const V<T, 2> &);
  inline void set(T);
  inline void zero();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{
  
  inline V<T, 2> & operator += (const V<T, 2> &);
  inline V<T, 2> & operator -= (const V<T, 2> &);
  inline V<T, 2> & operator *= (const V<T, 2> &);
  inline V<T, 2> & operator /= (const V<T, 2> &);
  
  inline V<T, 2> & operator += (T);
  inline V<T, 2> & operator -= (T);
  inline V<T, 2> & operator *= (T);
  inline V<T, 2> & operator /= (T);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Conversion
  //@{
  
  inline operator V<int, 2> () const;
  inline operator V<double, 2> () const;
  
  //@}
};

//------------------------------------------------------------------------------
//!\class V<T, 3>
//
//! A tree-dimensional vector.
//------------------------------------------------------------------------------

template<typename T>
struct V<T, 3>
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline V();
  inline V(const V<T, 3> &);
  inline V(T, T, T);
  inline V(T);
  
  inline V<T, 3> & operator = (const V<T, 3> &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  static const int dimension;
  
  T e [3];
  
  T & x, & y, & z;
  
  inline void set(const V<T, 3> &);
  inline void set(T);
  inline void zero();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{
  
  inline V<T, 3> & operator += (const V<T, 3> &);
  inline V<T, 3> & operator -= (const V<T, 3> &);
  inline V<T, 3> & operator *= (const V<T, 3> &);
  inline V<T, 3> & operator /= (const V<T, 3> &);
  
  inline V<T, 3> & operator += (T);
  inline V<T, 3> & operator -= (T);
  inline V<T, 3> & operator *= (T);
  inline V<T, 3> & operator /= (T);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Conversion
  //@{

  inline operator V<int, 3> () const;
  inline operator V<double, 3> () const;
  
  //@}
};

//------------------------------------------------------------------------------

}
}

#include <djv_base/vector_fnc.h>
#include <djv_base/vector_fnc_inline.h>
#include <djv_base/vector_inline.h>
#include <djv_base/vector2_inline.h>
#include <djv_base/vector3_inline.h>
#endif

