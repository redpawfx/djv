/*------------------------------------------------------------------------------
 bin/djv_view/playback_base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_PLAYBACK_BASE_H
#define DJV_VIEW_PLAYBACK_BASE_H

#include "base.h"
#include <djv_ui/callback.h>
#include <djv_image/time.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::playback
//
//! Playback.
//------------------------------------------------------------------------------

namespace playback {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Playback.

enum PLAYBACK
{
  STOP,
  FORWARD,
  REVERSE,
  
  _PLAYBACK_SIZE
};

//! Loop mode.

enum LOOP
{
  LOOP_ONCE,
  LOOP_REPEAT,
  LOOP_PING_PONG,
  
  _LOOP_SIZE
};

//! Internal timer.

enum TIMER
{
  TIMER_SLEEP,
  TIMER_TIMEOUT,
  
  _TIMER_SIZE
};

//! User-interface layout.

enum LAYOUT
{
  LAYOUT_DEFAULT,
  LAYOUT_LEFT,
  LAYOUT_CENTER,
  LAYOUT_MINIMAL,
  
  _LAYOUT_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

String & operator >> (String &, PLAYBACK &) throw (String);
String & operator >> (String &, LOOP &) throw (String);
String & operator >> (String &, TIMER &) throw (String);
String & operator >> (String &, LAYOUT &) throw (String);

String & operator << (String &, PLAYBACK);
String & operator << (String &, LOOP);
String & operator << (String &, TIMER);
String & operator << (String &, LAYOUT);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_playback();
const List<String> & label_loop();
const List<String> & label_timer();
const List<String> & label_layout();

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Global options.

class Global : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Global();
  
  ~Global();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Timer
  //@{

  void timer(TIMER);
  void timer_resolution(double);
  void timer_add(Group *);
  void timer_del(Group *);
  
  TIMER timer() const;
  double timer_resolution() const;

  //@}
  
private:

  DJV_FL_CALLBACK(Global, timer_callback);

  TIMER _timer;
  List<playback::Group *> _timer_list;
  double _timer_resolution;
  djv_base::time::Timer _timer_time;
};

//! Access to global options.

Global * global();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

