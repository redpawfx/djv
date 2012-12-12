/*------------------------------------------------------------------------------
 include/djv_base/matrix_def.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_MATRIX_DEF_H
#define DJV_MATRIX_DEF_H

namespace djv_base {
namespace matrix {

template<typename T, int D>
struct M;
template<typename T>
struct M<T, 3>;
template<typename T>
struct M<T, 4>;

}

typedef matrix::M<double, 3> M3f;

typedef matrix::M<double, 4> M4f;

}

#endif

