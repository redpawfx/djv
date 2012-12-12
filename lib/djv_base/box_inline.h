/*------------------------------------------------------------------------------
 include/djv_base/box_inline.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace box {

//------------------------------------------------------------------------------
// Box<T, D>
//------------------------------------------------------------------------------

template<typename T, int D>
const int Box<T, D>::dimension = D;

template<typename T, int D>
inline Box<T, D>::Box() {}

template<typename T, int D>
inline Box<T, D>::Box(const Box<T, D> & in)
{
  set(in);
}

template<typename T, int D>
inline Box<T, D>::Box(
  const vector::V<T, D> & position,
  const vector::V<T, D> & size
) :
  position(position),
  size(size)
{}

template<typename T, int D>
inline Box<T, D>::Box(const vector::V<T, D> & size) : size(size) {}

template<typename T, int D>
inline void Box<T, D>::set(const Box<T, D> & in)
{
  position = in.position;
  size = in.size;
}

template<typename T, int D>
inline void Box<T, D>::zero()
{
  position.zero();
  size.zero();
}

template<typename T, int D>
inline void Box<T, D>::lower_right(const vector::V<T, D> & in)
{
  size = in - position;
}

template<typename T, int D>
inline vector::V<T, D> Box<T, D>::lower_right() const
{
  return position + size;
}

template<typename T, int D>
inline Box<T, D> & Box<T, D>::operator *= (const vector::V<T, D> & in)
{
  position *= in;
  size *= in;
}

template<typename T, int D>
inline Box<T, D> & Box<T, D>::operator /= (const vector::V<T, D> & in)
{
  position /= in;
  size /= in;
}

template<typename T, int D>
inline Box<T, D> & Box<T, D>::operator *= (T in)
{
  position *= in;
  size *= in;
}

template<typename T, int D>
inline Box<T, D> & Box<T, D>::operator /= (T in)
{
  position /= in;
  size /= in;
}

//------------------------------------------------------------------------------

}
}

