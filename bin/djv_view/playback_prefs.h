/*------------------------------------------------------------------------------
 bin/djv_view/playback_prefs.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_PLAYBACK_PREFS_H
#define DJV_VIEW_PLAYBACK_PREFS_H

#include "playback_base.h"
#include <djv_ui/widget.h>

namespace djv_bin {
namespace view {
namespace playback {

//------------------------------------------------------------------------------
//!\class Prefs
//
//! Playback preferences.
//------------------------------------------------------------------------------

class Prefs : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Prefs();
  
  ~Prefs();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Options
  //@{
  
  void start(bool);

  bool start() const;

  void loop(LOOP);

  LOOP loop() const;

  djv_ui::callback::Signal<LOOP> loop_signal;

  void every_frame(bool);

  bool every_frame() const;

  djv_ui::callback::Signal<bool> every_frame_signal;
  
  void layout(LAYOUT);

  LAYOUT layout() const;

  djv_ui::callback::Signal<LAYOUT> layout_signal;

  //@}
  
private:

  bool _start;
  LOOP _loop;
  bool _every_frame;
  LAYOUT _layout;
};

//------------------------------------------------------------------------------
//!\class Prefs_Widget
//
//! Playback preferences widget.
//------------------------------------------------------------------------------

class Prefs_Widget : public djv_ui::Widget
{
public:

  Prefs_Widget();

private:

  DJV_CALLBACK(Prefs_Widget, start_callback, bool);
  DJV_CALLBACK(Prefs_Widget, loop_callback, int);
  DJV_CALLBACK(Prefs_Widget, every_frame_callback, bool);
  DJV_CALLBACK(Prefs_Widget, layout_callback, int);
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  prefs_title,
  prefs_group,
  prefs_start,
  prefs_loop,
  prefs_every_frame,
  prefs_ui_group,
  prefs_layout;

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

