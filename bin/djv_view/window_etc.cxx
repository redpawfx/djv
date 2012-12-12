/*------------------------------------------------------------------------------
 bin/djv_view/window_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "window_base.h"

#include "window.h"
#include "window_prefs.h"

namespace djv_bin {
namespace view {
namespace window {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

double resize_max(RESIZE_MAX in)
{
  switch (in)
  {
    case RESIZE_MAX_25: return 0.25;
    case RESIZE_MAX_50: return 0.50;
    case RESIZE_MAX_75: return 0.75;
    default: break;
  }
  
  return 1.0;
}

_DJV_STRING_OPERATOR_LABEL(RESIZE_MAX, label_resize_max())
_DJV_STRING_OPERATOR_LABEL(TOOLBAR, label_toolbar())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_resize_max()
{
  static const List<String> data = List<String>() <<
    "Unlimited" <<
    "25% of screen" <<
    "50% of screen" <<
    "75% of screen";
  
  DJV_ASSERT(data.size() == _RESIZE_SIZE);
  
  return data;
}

const List<String> & label_toolbar()
{
  static const List<String> data = List<String>() <<
    "Buttons" <<
    "Playback" <<
    "Information";
  
  DJV_ASSERT(data.size() == _TOOLBAR_SIZE);
  
  return data;
}

extern const String
  label_toolbar_info_pixel = "Pixel: %%, %%, %%",
  label_toolbar_info_pixel_size = "Pixel: 4096, 4096, RGBA F32 1.0 1.0 1.0 1.0",
  label_toolbar_info_image = "Image: %%x%%:%% %%",
  label_toolbar_info_cache = "Cache: %% / (%% / %%) MB";

extern const String
  menu_title = "Window",
  menu_new = "&New",
  menu_copy = "&Copy",
  menu_close = "Clos&e",
  menu_fit = "&Fit",
  menu_fullscreen = "F&ullscreen",
  menu_toolbar_buttons = "Bu&ttons",
  menu_toolbar_playback = "Pla&yback",
  menu_toolbar_info = "Information";

extern const String
  tooltip_new = "Create a new window\n\nShortcut: %%",
  tooltip_copy = "Copy this window\n\nShortcut: %%",
  tooltip_close = "Close this window\n\nShortcut: %%",
  tooltip_fit = "Resize window to fit image\n\nShortcut: %%",
  tooltip_fullscreen = "Fullscreen\n\nShortcut: %%",
  tooltip_info_pixel =
  "Pixel information\n\nClick and drag inside the image.",
  tooltip_info_image = "Image information",
  tooltip_info_cache =
  "Cache information\n\nKey: Window-usage / (Total-usage / Maximum)";

extern const String
  prefs_title = "Window",
  prefs_resize_group = "Window Resizing",
  prefs_resize_fit = "Fit window to image",
  prefs_resize_max = "Maximum Size",
  prefs_toolbar_group = "Toolbars";

//------------------------------------------------------------------------------

}
}
}

