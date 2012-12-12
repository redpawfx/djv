/*------------------------------------------------------------------------------
 include/djv_base/box.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_BOX_H
#define DJV_BOX_H

#include <djv_base/box_def.h>

#include <djv_base/vector.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::box
//
//! Axis-aligned boxes.
//------------------------------------------------------------------------------

namespace box {

//------------------------------------------------------------------------------
//!\class Box
//
//! A base class for axis-aligned boxes.
//------------------------------------------------------------------------------

template<typename T, int D>
struct Box
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  inline Box();
  inline Box(const Box<T, D> &);
  inline Box(const vector::V<T, D> & position, const vector::V<T, D> & size);
  inline Box(const vector::V<T, D> & size);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  static const int dimension;
  
  vector::V<T, D> position;
  vector::V<T, D> size;
  
  inline void set(const Box<T, D> &);
  inline void zero();
  
  inline void lower_right(const vector::V<T, D> &);
  
  inline vector::V<T, D> lower_right() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{
    
  inline Box<T, D> & operator *= (const vector::V<T, D> &);
  inline Box<T, D> & operator /= (const vector::V<T, D> &);

  inline Box<T, D> & operator *= (T);
  inline Box<T, D> & operator /= (T);
  
  //@}
};

//------------------------------------------------------------------------------
//!\class Box<T, 2>
//
//! A two-dimensional axis-aligned box.
//------------------------------------------------------------------------------

template<typename T>
struct Box<T, 2>
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline Box();
  inline Box(const Box<T, 2> &);
  inline Box(const vector::V<T, 2> & position, const vector::V<T, 2> & size);
  inline Box(const vector::V<T, 2> & size);
  inline Box(T x, T y, T w, T h);
  inline Box(T w, T h);
  inline Box<T, 2> & operator = (const Box<T, 2> &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  static const int dimension;
  
  vector::V<T, 2> position;
  vector::V<T, 2> size;
  
  T & x, & y;
  T & w, & h;
  
  inline void set(const Box<T, 2> &);
  inline void zero();

  inline void lower_right(const vector::V<T, 2> &);
  
  inline vector::V<T, 2> lower_right() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{
  
  inline Box<T, 2> & operator *= (const vector::V<T, 2> &);
  inline Box<T, 2> & operator /= (const vector::V<T, 2> &);

  inline Box<T, 2> & operator *= (T);
  inline Box<T, 2> & operator /= (T);

  //@}
  //----------------------------------------------------------------------------
  //!\name Conversion
  //@{
  
  inline operator Box<int, 2> () const;
  inline operator Box<double, 2> () const;
  
  //@}
};

//------------------------------------------------------------------------------
//!\class Box<T, 3>
//
//! A three-dimensional axis-aligned box.
//------------------------------------------------------------------------------

template<typename T>
struct Box<T, 3>
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline Box();
  inline Box(const Box<T, 3> &);
  inline Box(const vector::V<T, 3> & position, const vector::V<T, 3> & size);
  inline Box(const vector::V<T, 3> & size);
  inline Box(T x, T y, T z, T w, T h, T d);
  inline Box(T w, T h, T d);
  inline Box<T, 3> & operator = (const Box<T, 3> &);

  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  static const int dimension;
  
  vector::V<T, 3> position;
  vector::V<T, 3> size;
  
  T & x, & y, & z;
  T & w, & h, & d;
  
  inline void set(const Box<T, 3> &);
  inline void zero();

  inline void lower_right(const vector::V<T, 3> &);
  
  inline vector::V<T, 3> lower_right() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{
  
  inline Box<T, 3> & operator *= (const vector::V<T, 3> &);
  inline Box<T, 3> & operator /= (const vector::V<T, 3> &);

  inline Box<T, 3> & operator *= (T);
  inline Box<T, 3> & operator /= (T);

  //@}
  //----------------------------------------------------------------------------
  //!\name Conversion
  //@{
  
  inline operator Box<int, 3> () const;
  inline operator Box<double, 3> () const;
  
  //@}
};

//------------------------------------------------------------------------------

}
}

#include <djv_base/box_fnc.h>
#include <djv_base/box_fnc_inline.h>
#include <djv_base/box_inline.h>
#include <djv_base/box2_inline.h>
#include <djv_base/box3_inline.h>
#endif

