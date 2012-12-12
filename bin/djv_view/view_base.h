/*------------------------------------------------------------------------------
 bin/djv_view/view_base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_VIEW_BASE_H
#define DJV_VIEW_VIEW_BASE_H

#include "base.h"
#include <djv_image/data.h>
#include <djv_image/tag.h>
#include <djv_image/time.h>
#include <djv_base/file.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::view
//
//! Views.
//------------------------------------------------------------------------------

namespace view {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! What to do with the image when the view resizes.

enum RESIZE
{
  RESIZE_NONE,
  RESIZE_FIT,
  RESIZE_CENTER,
  
  _RESIZE_SIZE
};

//! Grid overlay.

enum GRID
{
  GRID_NONE,
  GRID_1x1,
  GRID_10x10,
  GRID_100x100,
  
  _GRID_SIZE
};

//! HUD options.

enum HUD_SHOW
{
  HUD_FILE_NAME,
  HUD_LAYER,
  HUD_SIZE,
  HUD_PROXY,
  HUD_PIXEL,
  HUD_TAG,
  HUD_FRAME,
  HUD_SPEED,
  
  _HUD_SHOW_SIZE
};

//! HUD background.

enum HUD_BACKGROUND
{
  HUD_BACKGROUND_NONE,
  HUD_BACKGROUND_SOLID,
  HUD_BACKGROUND_SHADOW,
  
  _HUD_BACKGROUND_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Hud_Info
//
//! HUD information.
//------------------------------------------------------------------------------

struct Hud_Info
{
  Hud_Info() :
    frame(0),
    speed_real(0.0),
    dropped_frames(false)
  {}

  djv_image::data::Info info;
  djv_image::tag::Tag tag;
  int64_t frame;
  djv_image::time::Speed speed;
  double speed_real;
  bool dropped_frames;
};

//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

String & operator >> (String &, RESIZE &) throw (String);
String & operator >> (String &, GRID &) throw (String);
String & operator >> (String &, HUD_BACKGROUND &) throw (String);

String & operator << (String &, RESIZE);
String & operator << (String &, GRID);
String & operator << (String &, HUD_BACKGROUND);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_resize();
const List<String> & label_grid();
const List<String> & label_hud_show();
const List<String> & label_hud_background();

extern const String
  label_hud_file_name,
  label_hud_layer,
  label_hud_size,
  label_hud_proxy,
  label_hud_pixel,
  label_hud_tag,
  label_hud_frame,
  label_hud_speed;

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

