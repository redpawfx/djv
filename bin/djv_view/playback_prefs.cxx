/*------------------------------------------------------------------------------
 bin/djv_view/playback_prefs.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "playback_prefs.h"

#include <djv_ui/prefs.h>

namespace djv_bin {
namespace view {
namespace playback {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs() :
  loop_signal(this),
  every_frame_signal(this),
  layout_signal(this),
  _start(true),
  _loop(LOOP_REPEAT),
  _every_frame(true),
  _layout(LAYOUT_DEFAULT)
{
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "playback");
  djv_ui::prefs::get(&prefs, "start", &_start);
  djv_ui::prefs::get(&prefs, "loop", &_loop);
  djv_ui::prefs::get(&prefs, "every_frame", &_every_frame);
  djv_ui::prefs::get(&prefs, "layout", &_layout);
}

Prefs::~Prefs()
{
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "playback");
  djv_ui::prefs::set(&prefs, "start", _start);
  djv_ui::prefs::set(&prefs, "loop", _loop);
  djv_ui::prefs::set(&prefs, "every_frame", _every_frame);
  djv_ui::prefs::set(&prefs, "layout", _layout);
}

void Prefs::start(bool in)
{
  _start = in;
}

bool Prefs::start() const
{
  return _start;
}

void Prefs::loop(LOOP in)
{
  if (in == _loop)
    return;
  
  _loop = in;
  
  loop_signal.emit(_loop);
}

LOOP Prefs::loop() const
{
  return _loop;
}

void Prefs::every_frame(bool in)
{
  if (in == _every_frame)
    return;
  
  _every_frame = in;
  
  every_frame_signal.emit(_every_frame);
}

bool Prefs::every_frame() const
{
  return _every_frame;
}

void Prefs::layout(LAYOUT in)
{
  if (in == _layout)
    return;
  
  _layout = in;
  
  layout_signal.emit(_layout);
}

LAYOUT Prefs::layout() const
{
  return _layout;
}

//------------------------------------------------------------------------------

}
}
}

