/*------------------------------------------------------------------------------
 include/djv_base/box_def.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_BOX_DEF_H
#define DJV_BOX_DEF_H

namespace djv_base {
namespace box {

template<typename T, int D>
struct Box;
template<typename T>
struct Box<T, 2>;
template<typename T>
struct Box<T, 3>;

}

typedef box::Box<int, 2> Box2i;
typedef box::Box<double, 2> Box2f;
typedef box::Box<int, 3> Box3i;
typedef box::Box<double, 3> Box3f;

}

#endif

