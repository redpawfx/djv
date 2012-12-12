/*------------------------------------------------------------------------------
 include/djv_base/matrix.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_MATRIX_H
#define DJV_MATRIX_H

#include <djv_base/matrix_def.h>

#include <djv_base/vector.h>
#include <djv_base/box.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::matrix
//
//! Matrices.
//------------------------------------------------------------------------------

namespace matrix {

//------------------------------------------------------------------------------
//!\class M
//
//! A base class for matrices.
//------------------------------------------------------------------------------

template<typename T, int D>
struct M
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline M();
  inline M(const M<T, D> &);
  inline M(const T *);
  
  inline M<T, D> & operator = (const M<T, D> &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{

  static const int dimension;
  static const int dimension_x2;
  
  //! Components.
  
  T e [D * D];
  
  inline void set(const M<T, D> &);
  inline void set(const T *);
  inline void zero();
  inline void identity();
  
  //@}
};

//------------------------------------------------------------------------------
//!\class M<T, 3>
//
//! A 3x3 matrix.
//------------------------------------------------------------------------------

template<typename T>
struct M<T, 3>
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline M();
  inline M(const M<T, 3> &);
  inline M(T, T, T, T, T, T, T, T, T);
  inline M(const T *);
  
  inline M<T, 3> & operator = (const M<T, 3> &);

  //@}
  
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  static const int dimension;
  static const int dimension_x2;
  
  T e [3 * 3];
  
  inline void set(const M<T, 3> &);
  inline void set(const T *);
  inline void set(T, T, T, T, T, T, T, T, T);
  inline void zero();  
  inline void identity();
  
  //@}
};

//------------------------------------------------------------------------------
//!\class M<T, 4>
//
//! A 4x4 matrix.
//------------------------------------------------------------------------------

template<typename T>
struct M<T, 4>
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline M();
  inline M(const M<T, 4> &);
  inline M(T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T);
  inline M(const T *);
  inline M<T, 4> & operator = (const M<T, 4> &);

  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  static const int dimension;
  static const int dimension_x2;
  
  T e [4 * 4];
  
  inline void set(const M<T, 4> &);
  inline void set(const T *);
  inline void set(T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T);
  inline void zero();
  inline void identity();
  
  //@}
};

//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

template<typename T, int D>
inline bool operator == (const M<T, D> &, const M<T, D> &);

template<typename T, int D>
inline bool operator != (const M<T, D> &, const M<T, D> &);

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

template<typename T, int D>
inline Debug & operator << (Debug &, const M<T, D> &);

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_base/matrix_fnc.h>
#include <djv_base/matrix_fnc_inline.h>
#include <djv_base/matrix_inline.h>
#include <djv_base/matrix3_inline.h>
#include <djv_base/matrix4_inline.h>
#endif

