/*------------------------------------------------------------------------------
 bin/djv_view/window_base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_WINDOW_BASE_H
#define DJV_VIEW_WINDOW_BASE_H

#include "base.h"
#include <djv_base/base.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::window
//
//! Windows.
//------------------------------------------------------------------------------

namespace window {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Maximum percentage of the screen to use when resizing.

enum RESIZE_MAX
{
  RESIZE_MAX_UNLIMITED,
  RESIZE_MAX_25,
  RESIZE_MAX_50,
  RESIZE_MAX_75,
  
  _RESIZE_SIZE
};

//! Toolbar options.

enum TOOLBAR
{
  TOOLBAR_BUTTONS,
  TOOLBAR_PLAYBACK,
  TOOLBAR_INFO,
  
  _TOOLBAR_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

double resize_max(RESIZE_MAX);

String & operator >> (String &, RESIZE_MAX &) throw (String);
String & operator >> (String &, TOOLBAR &) throw (String);

String & operator << (String &, RESIZE_MAX);
String & operator << (String &, TOOLBAR);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_resize_max();
const List<String> & label_toolbar();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

