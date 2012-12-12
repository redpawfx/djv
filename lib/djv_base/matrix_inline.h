/*------------------------------------------------------------------------------
 include/djv_base/matrix_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace matrix {

//------------------------------------------------------------------------------
// M<T, D>
//------------------------------------------------------------------------------

template<typename T, int D>
const int M<T, D>::dimension = D;

template<typename T, int D>
const int M<T, D>::dimension_x2 = D * D;

template<typename T, int D>
inline M<T, D>::M()
{
  identity();
}

template<typename T, int D>
inline M<T, D>::M(const M<T, D> & in)
{
  set(in);
}

template<typename T, int D>
inline M<T, D>::M(const T * in)
{
  set(in);
}

template<typename T, int D>
inline void M<T, D>::set(const M<T, D> & in)
{
  for (int i = 0; i < dimension_x2; ++i)
    e[i] = in.e[i];
}

template<typename T, int D>
inline void M<T, D>::set(const T * in)
{
  for (int i = 0; i < dimension_x2; ++i)
    e[i] = in[i];
}

template<typename T, int D>
inline void M<T, D>::identity()
{
  for (int i = 0; i < D; ++i)
    for (int j = 0; j < D; ++j)
      e[i * D + j] = (i == j) ? 1.0 : 0.0;
}

template<typename T, int D>
inline void M<T, D>::zero()
{
  for (int i = 0; i < dimension_x2; ++i)
    e[i] = 0.0;
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

template<typename T, int D>
inline bool operator == (const M<T, D> & a, const M<T, D> & b)
{
  for (int i = 0; i < a.dimension_x2; ++i)
    if (a.e[i] != b.e[i])
      return false;

  return true;
}

template<typename T, int D>
inline bool operator != (const M<T, D> & a, const M<T, D> & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

template<typename T, int D>
inline Debug & operator << (Debug & debug, const M<T, D> & in)
{
  debug << "\n";

  for (int i = 0; i < D; ++i)
  {
    for (int j = 0; j < D; ++j)
    {
      debug << in.e[i * D + j];
      
      if (j < D - 1) debug << " ";
    }
    
    if (i < D - 1) debug << "\n";
  }

  return debug;
}

//------------------------------------------------------------------------------

}
}

