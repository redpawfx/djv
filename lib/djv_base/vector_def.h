/*------------------------------------------------------------------------------
 include/djv_base/vector_def.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VECTOR_DEF_H
#define DJV_VECTOR_DEF_H

namespace djv_base {
namespace vector {

template<typename T, int D>
struct V;
template<typename T>
struct V<T, 2>;
template<typename T>
struct V<T, 3>;

}

typedef vector::V<bool, 2> V2b;
typedef vector::V<int, 2> V2i;
typedef vector::V<double, 2> V2f;

typedef vector::V<bool, 3> V3b;
typedef vector::V<int, 3> V3i;
typedef vector::V<double, 3> V3f;

}

#endif

