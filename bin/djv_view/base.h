/*------------------------------------------------------------------------------
 bin/djv_view/base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_BASE_H
#define DJV_VIEW_BASE_H

#include <djv_base/base.h>

namespace djv_bin {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view
//
//! Image and movie viewer.
//------------------------------------------------------------------------------

namespace view {
using namespace djv_base;

namespace file {
class Group;
}

namespace window {
class Window;
}

namespace view {
class Group;
class Widget;
}

namespace image {
class Group;
}

namespace playback
{
  class Group;
}

namespace tool {
class Group;
}

namespace help {
class Group;
}

//------------------------------------------------------------------------------

}
}

#endif

