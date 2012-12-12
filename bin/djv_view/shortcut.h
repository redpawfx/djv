/*------------------------------------------------------------------------------
 bin/djv_view/shortcut.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_SHORTCUT_H
#define DJV_SHORTCUT_H

#include "base.h"
#include <djv_ui/shortcut.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::shortcut
//
//! Keyboard shortcuts.
//------------------------------------------------------------------------------

namespace shortcut {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

#if defined(DJV_WINDOWS)
#undef FILE_OPEN // XXX Windows name-space pollution.
#endif

//! Keyboard shortcuts.

enum SHORTCUT
{
  EXIT,
  
  FILE_OPEN,
  FILE_RELOAD,
  FILE_SAVE,
  FILE_SAVE_FRAME,
  FILE_CLOSE,
  FILE_LAYER_DEFAULT,
  FILE_LAYER_1,
  FILE_LAYER_2,
  FILE_LAYER_3,
  FILE_LAYER_4,
  FILE_LAYER_5,
  FILE_LAYER_6,
  FILE_LAYER_7,
  FILE_LAYER_8,
  FILE_LAYER_9,
  FILE_LAYER_10,
  FILE_LAYER_PREV,
  FILE_LAYER_NEXT,
  FILE_PROXY_NONE,
  FILE_PROXY_1_2,
  FILE_PROXY_1_4,
  FILE_PROXY_1_8,
  
  WINDOW_NEW,
  WINDOW_COPY,
  WINDOW_CLOSE,
  WINDOW_FIT,
  WINDOW_FULLSCREEN,
  WINDOW_TOOLBAR_BUTTONS,
  WINDOW_TOOLBAR_PLAYBACK,
  WINDOW_TOOLBAR_INFO,
  
  VIEW_LEFT,
  VIEW_RIGHT,
  VIEW_UP,
  VIEW_DOWN,
  VIEW_CENTER,
  VIEW_ZOOM_IN,
  VIEW_ZOOM_OUT,
  VIEW_ZOOM_RESET,
  VIEW_RESET,
  VIEW_FIT,
  VIEW_HUD,
  
  IMAGE_FRAME_STORE,
  IMAGE_FRAME_STORE_LOAD,
  IMAGE_MIRROR_HORIZONTAL,
  IMAGE_MIRROR_VERTICAL,
  IMAGE_SCALE_NONE,
  IMAGE_SCALE_16_9,
  IMAGE_SCALE_1_85,
  IMAGE_SCALE_2_35,
  IMAGE_ROTATE_0,
  IMAGE_ROTATE_90,
  IMAGE_ROTATE_180,
  IMAGE_ROTATE_270,
  IMAGE_COLOR_PROFILE,
  IMAGE_CHANNEL_RED,
  IMAGE_CHANNEL_GREEN,
  IMAGE_CHANNEL_BLUE,
  IMAGE_CHANNEL_ALPHA,
  IMAGE_DISPLAY_PROFILE,
  IMAGE_DISPLAY_PROFILE_RESET,
  IMAGE_DISPLAY_PROFILE_1,
  IMAGE_DISPLAY_PROFILE_2,
  IMAGE_DISPLAY_PROFILE_3,
  IMAGE_DISPLAY_PROFILE_4,
  IMAGE_DISPLAY_PROFILE_5,
  IMAGE_DISPLAY_PROFILE_6,
  IMAGE_DISPLAY_PROFILE_7,
  IMAGE_DISPLAY_PROFILE_8,
  IMAGE_DISPLAY_PROFILE_9,
  IMAGE_DISPLAY_PROFILE_10,

  PLAYBACK_STOP,
  PLAYBACK_FORWARD,
  PLAYBACK_REVERSE,
  PLAYBACK_LOOP,
  PLAYBACK_START,
  PLAYBACK_START_ABS,
  PLAYBACK_END,
  PLAYBACK_END_ABS,
  PLAYBACK_PREV,
  PLAYBACK_PREV_X10,
  PLAYBACK_PREV_X100,
  PLAYBACK_NEXT,
  PLAYBACK_NEXT_X10,
  PLAYBACK_NEXT_X100,
  PLAYBACK_IN_OUT,
  PLAYBACK_IN_MARK,
  PLAYBACK_IN_RESET,
  PLAYBACK_OUT_MARK,
  PLAYBACK_OUT_RESET,
  
  TOOL_MAGNIFY,
  TOOL_COLOR_PICKER,
  TOOL_HISTOGRAM,
  TOOL_INFO,
  
  _SHORTCUT_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Prefs
//
//! Keyboard shortcut preferences.
//------------------------------------------------------------------------------

class Prefs : public djv_ui::Callback_Base
{
public:

  Prefs();
  
  ~Prefs();

  void shortcut(const List<djv_ui::shortcut::Shortcut> &);
  
  const List<djv_ui::shortcut::Shortcut> & shortcut() const;

private:

  List<djv_ui::shortcut::Shortcut> _shortcut;
};

//------------------------------------------------------------------------------
//!\class Prefs_Widget
//
//! Keyboard shortcut preferences widget.
//------------------------------------------------------------------------------

class Prefs_Widget : public djv_ui::Widget
{
public:

  Prefs_Widget();

private:

  DJV_CALLBACK(Prefs_Widget, shortcut_callback,
    const List<djv_ui::shortcut::Shortcut> &);
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_shortcut();

extern const String
  prefs_title;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to preferences.

Prefs * prefs();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

