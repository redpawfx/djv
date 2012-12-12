/*------------------------------------------------------------------------------
 bin/djv_view/view_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "view_base.h"

#include "view_group.h"
#include "view_prefs.h"

namespace djv_bin {
namespace view {
namespace view {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(RESIZE, label_resize())
_DJV_STRING_OPERATOR_LABEL(GRID, label_grid())
_DJV_STRING_OPERATOR_LABEL(HUD_BACKGROUND, label_hud_background())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_resize()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "Fit image" <<
    "Center image";
  
  DJV_ASSERT(data.size() == _RESIZE_SIZE);
  
  return data;
}

const List<String> & label_grid()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "1x1" <<
    "10x10" <<
    "100x100";
  
  DJV_ASSERT(data.size() == _GRID_SIZE);
  
  return data;
}

const List<String> & label_hud_show()
{
  static const List<String> data = List<String>() <<
    "File name" <<
    "Layer" <<
    "Size" <<
    "Proxy" <<
    "Pixel" <<
    "Tags" <<
    "Playback frame" <<
    "Playback speed";
  
  DJV_ASSERT(data.size() == _HUD_SHOW_SIZE);
  
  return data;
}

const List<String> & label_hud_background()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "Solid" <<
    "Shadow";
  
  DJV_ASSERT(data.size() == _HUD_BACKGROUND_SIZE);
  
  return data;
}

extern const String
  label_hud_file_name = "File name = %%",
  label_hud_layer = "Layer = %%",
  label_hud_size = "Size = %%x%%:%%",
  label_hud_proxy = "Proxy = %%",
  label_hud_pixel = "Pixel = %%",
  label_hud_tag = "Tags:",
  label_hud_frame = "Frame = %%",
  label_hud_speed = "Speed = %%/";

extern const String
  menu_title = "View",
  menu_left = "&Left",
  menu_right = "&Right",
  menu_up = "&Up",
  menu_down = "&Down",
  menu_center = "&Center",
  menu_zoom_in = "Zoom &In",
  menu_zoom_out = "Zoom &Out",
  menu_zoom_reset = "Zoom Rese&t",
  menu_reset = "R&eset",
  menu_fit = "&Fit",
  menu_grid = "&Grid",
  menu_hud = "&HUD";

extern const String
  toolbar_zoom = "%%%",
  toolbar_zoom_format = "000%";

extern const String
  tooltip_zoom = "Zoom",
  tooltip_zoom_in = "Zoom in\n\nShortcut: %%",
  tooltip_zoom_out = "Zoom out\n\nShortcut: %%",
  tooltip_zoom_reset = "Reset zoom\n\nShortcut: %%",
  tooltip_fit = "Fit image to window\n\nShortcut: %%";

extern const String
  prefs_title = "View",
  prefs_group = "View Options",
  prefs_background = "Background color:",
  prefs_resize_group = "View Resizing",
  prefs_grid_group = "Grid",
  prefs_grid_color = "Color:",
  prefs_hud_group = "HUD (Heads Up Display)",
  prefs_hud = "Enable",
  prefs_hud_show = "Show",
  prefs_hud_color = "Color:",
  prefs_hud_background = "Background",
  prefs_hud_background_color = "Background color:";

//------------------------------------------------------------------------------

}
}
}

