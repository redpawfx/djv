/*------------------------------------------------------------------------------
 bin/djv_view/window_prefs.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "window_prefs.h"

#include <djv_ui/prefs.h>

namespace djv_bin {
namespace view {
namespace window {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs() :
  toolbar_signal(this),
  _resize_fit(true),
  _resize_max(RESIZE_MAX_75),
  _toolbar(_TOOLBAR_SIZE)
{
  _toolbar[TOOLBAR_BUTTONS] = true;
  _toolbar[TOOLBAR_PLAYBACK] = true;
  _toolbar[TOOLBAR_INFO] = true;
  
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "window");
  djv_ui::prefs::get(&prefs, "resize_fit", &_resize_fit);
  djv_ui::prefs::get(&prefs, "resize_max", &_resize_max);
  djv_ui::prefs::get(&prefs, "toolbar", &_toolbar);
}

Prefs::~Prefs()
{
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "window");
  djv_ui::prefs::set(&prefs, "resize_fit", _resize_fit);
  djv_ui::prefs::set(&prefs, "resize_max", _resize_max);
  djv_ui::prefs::set(&prefs, "toolbar", _toolbar);
}

void Prefs::resize_fit(bool in)
{
  _resize_fit = in;
}

bool Prefs::resize_fit() const
{
  return _resize_fit;
}

void Prefs::resize_max(RESIZE_MAX in)
{
  _resize_max = in;
}

RESIZE_MAX Prefs::resize_max() const
{
  return _resize_max;
}

void Prefs::toolbar(const List<bool> & in)
{
  if (in == _toolbar)
    return;
  
  _toolbar = in;
  
  toolbar_signal.emit(_toolbar);
}

const List<bool> & Prefs::toolbar() const
{
  return _toolbar;
}

//------------------------------------------------------------------------------

}
}
}

