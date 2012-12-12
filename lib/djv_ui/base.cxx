/*------------------------------------------------------------------------------
 lib/djv_ui/base.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "base.h"

#include <djv_ui/prefs.h>
#include <FL/Fl_Tooltip.H>

namespace djv_ui {
namespace base {

//------------------------------------------------------------------------------
// Cursor
//------------------------------------------------------------------------------

Cursor::Cursor(const Fl_Cursor & in)
{
  cursor(in);
}

Cursor::~Cursor()
{
  cursor(FL_CURSOR_DEFAULT);
}

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs() :
  _tooltips(true)
{
  prefs::Prefs prefs(prefs::prefs_global(), "base");
  prefs::get(&prefs, "tooltips", &_tooltips);
}

Prefs::~Prefs()
{
  prefs::Prefs prefs(prefs::prefs_global(), "base");
  prefs::set(&prefs, "tooltips", _tooltips);
}

void Prefs::tooltips(bool in)
{
  if (in == _tooltips)
    return;
  
  _tooltips = in;
  
  base::global()->tooltips(_tooltips);
}
  
bool Prefs::tooltips() const
{
  return _tooltips;
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() :
  _tooltips(prefs()->tooltips())
{
  tooltips_update();
}

Global::~Global()
{}

void Global::tooltips(bool in)
{
  if (in == _tooltips)
    return;
  
  _tooltips = in;
  
  tooltips_update();
}

bool Global::tooltips() const
{
  return _tooltips;
}

void Global::tooltips_update()
{
  if (_tooltips)
    Fl_Tooltip::enable();
  else
    Fl_Tooltip::disable();
}
  
//------------------------------------------------------------------------------

}
}
