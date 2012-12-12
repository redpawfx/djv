/*------------------------------------------------------------------------------
 bin/djv_view/image_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_base.h"

#include "application.h"
#include "image_group.h"
#include "image_prefs.h"
#include <djv_image/io.h>

namespace djv_bin {
namespace view {
namespace image {

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

V2f scale(SCALE in, const V2i & size)
{
  static const double data [] = { 1.0, 16.0 / 9.0, 1.85, 2.35 };
  
  V2f out(1.0);
  
  switch (in)
  {
    case SCALE_16_9:
    case SCALE_1_85:
    case SCALE_2_35:
      out.x = data[in] / vector::aspect(size);
      break;
    
    default: break;
  }
  
  return out;
}

double rotate(ROTATE in)
{
  static const double data [] = { 0, 90, 180, 270 };
  
  return data[in];
}

const djv_image::Image * lut_load(const File & in)
{
  if (in.get().empty())
    return 0;
  
  DJV_DEBUG("lut_load");
  DJV_DEBUG_PRINT("in = " << in);
  
  File file(in);
  if (file.is_seq_valid())
    file.type(djv_base::file::SEQ);
  
  const djv_image::Image * out = 0;
  
  try
  {
    out = djv_image::io::global()->load(file)->get();
    DJV_DEBUG_PRINT("image = " << *out);
  }
  catch (Error error)
  {
    application::global()->error(error);
  }
  
  return out;
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(SCALE, label_scale())
_DJV_STRING_OPERATOR_LABEL(ROTATE, label_rotate())

String & operator >> (String & in, Display_Profile & out) throw (String)
{
  in >> out.name;

  String tmp;
  in >> tmp;
  out.lut_file = tmp;

  in >> out.color;
  in >> out.levels;
  in >> out.soft_clip;
  
  return in;
}

String & operator << (String & out, const Display_Profile & in)
{
  return out <<
    in.name <<
    in.lut_file <<
    in.color <<
    in.levels <<
    in.soft_clip;
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_scale()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "16.9" <<
    "1.85" <<
    "2.35";
  
  DJV_ASSERT(data.size() == _SCALE_SIZE);
  
  return data;
}

const List<String> & label_rotate()
{
  static const List<String> data = List<String>() <<
    "0" <<
    "90" <<
    "180" <<
    "270";
  
  DJV_ASSERT(data.size() == _ROTATE_SIZE);
  
  return data;
}

extern const String
  menu_title = "Image",
  menu_frame_store = "&Frame Store",
  menu_frame_store_load = "&Load Frame Store",
  menu_mirror_h = "Mirror &Horizontal",
  menu_mirror_v = "Mirror &Vertical",
  menu_scale = "&Scale",
  menu_rotate = "&Rotate",
  menu_color_profile = "&Color Profile",
  menu_display_profile = "&Display Profile",
  menu_display_profile_show = "Show",
  menu_display_profile_reset = "Reset",
  menu_display_profile_add = "Add",
  menu_display_profile_del = "Delete",
  menu_display_profile_del_all = "Delete All",
  menu_channel = "C&hannel";

extern const String
  tooltip_display_profile = "Display profile\n\nShortcut: %%";

extern const String
  prefs_title = "Image",
  prefs_group = "Image Options",
  prefs_frame_store_file_reload =
  "Automatically store the current frame when files are reloaded",
  prefs_xform_group = "Image Transforms",
  prefs_xform_mirror = "Mirror",
  prefs_xform_mirror_h = "Horizontal",
  prefs_xform_mirror_v = "Vertical",
  prefs_xform_scale = "Scale",
  prefs_xform_rotate = "Rotate",
  prefs_color_profile_group = "Color Profile",
  prefs_color_profile = "Enable",
  prefs_display_profile_group = "Display Profile",
  prefs_display_profile_none = "None",
  prefs_channel_group = "Image Channels";

extern const String
  dialog_display_profile_add = "Add image display profile:",
  dialog_display_profile_del = "Delete image display profiles:",
  dialog_display_profile_del_all = "Delete all image display profiles?";

//------------------------------------------------------------------------------

}
}
}

