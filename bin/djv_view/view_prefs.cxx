/*------------------------------------------------------------------------------
 bin/djv_view/view_prefs.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "view_prefs.h"

#include "application.h"
#include <djv_ui/global.h>
#include <djv_ui/prefs.h>

namespace djv_bin {
namespace view {
namespace view {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs() :
  resize_signal(this),
  grid_signal(this),
  hud_signal(this),
  _background(djv_image::Color(0.0)),
  _resize(RESIZE_NONE),
  _grid(GRID_NONE),
  _grid_color(djv_image::Color(0.25)),
  _hud(false),
  _hud_show(true, _HUD_SHOW_SIZE),
  _hud_color(djv_image::Color(1.0)),
  _hud_background(HUD_BACKGROUND_SHADOW),
  _hud_background_color(djv_image::Color(0.0))
{
  DJV_DEBUG("Prefs::Prefs");
  
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "view");
  djv_ui::prefs::get(&prefs, "background", &_background);
  djv_ui::prefs::get(&prefs, "resize", &_resize);
  djv_ui::prefs::get(&prefs, "grid", &_grid);
  djv_ui::prefs::get(&prefs, "grid_color", &_grid_color);
  djv_ui::prefs::get(&prefs, "hud", &_hud);
  djv_ui::prefs::get(&prefs, "hud_show", &_hud_show);
  djv_ui::prefs::get(&prefs, "hud_color", &_hud_color);
  djv_ui::prefs::get(&prefs, "hud_background", &_hud_background);
  djv_ui::prefs::get(&prefs, "hud_background_color", &_hud_background_color);
}

Prefs::~Prefs()
{
  DJV_DEBUG("Prefs::~Prefs");

  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "view");
  djv_ui::prefs::set(&prefs, "background", _background);
  djv_ui::prefs::set(&prefs, "resize", _resize);
  djv_ui::prefs::set(&prefs, "grid", _grid);
  djv_ui::prefs::set(&prefs, "grid_color", _grid_color);
  djv_ui::prefs::set(&prefs, "hud", _hud);
  djv_ui::prefs::set(&prefs, "hud_show", _hud_show);
  djv_ui::prefs::set(&prefs, "hud_color", _hud_color);
  djv_ui::prefs::set(&prefs, "hud_background", _hud_background);
  djv_ui::prefs::set(&prefs, "hud_background_color", _hud_background_color);
}

void Prefs::background(const djv_image::Color & in)
{
  if (in == _background)
    return;
  
  _background = in;
  
  djv_ui::global()->window_update();
}

const djv_image::Color & Prefs::background() const
{
  return _background;
}

void Prefs::resize(RESIZE in)
{
  if (in == _resize)
    return;
  
  _resize = in;
  
  resize_signal.emit(_resize);
}

RESIZE Prefs::resize() const
{
  return _resize;
}

void Prefs::grid(GRID in)
{
  if (in == _grid)
    return;
  
  _grid = in;
  
  grid_signal.emit(_grid);
}

GRID Prefs::grid() const
{
  return _grid;
}

void Prefs::grid_color(const djv_image::Color & in)
{
  if (in == _grid_color)
    return;
  
  _grid_color = in;
  
  djv_ui::global()->window_update();
}

const djv_image::Color & Prefs::grid_color() const
{
  return _grid_color;
}

void Prefs::hud(bool in)
{
  if (in == _hud)
    return;
  
  _hud = in;
  
  hud_signal.emit(_hud);
}

bool Prefs::hud() const
{
  return _hud;
}

void Prefs::hud_show(HUD_SHOW show, bool in)
{
  if (in == _hud_show[show])
    return;
  
  _hud_show[show] = in;
  
  djv_ui::global()->window_update();
}

List<bool> Prefs::hud_show() const
{
  return _hud_show;
}

bool Prefs::hud_show(HUD_SHOW in) const
{
  return _hud_show[in];
}

void Prefs::hud_color(const djv_image::Color & in)
{
  if (in == _hud_color)
    return;
  
  _hud_color = in;
  
  djv_ui::global()->window_update();
}

const djv_image::Color & Prefs::hud_color() const
{
  return _hud_color;
}

void Prefs::hud_background(HUD_BACKGROUND in)
{
  if (in == _hud_background)
    return;
  
  _hud_background = in;
  
  djv_ui::global()->window_update();
}

HUD_BACKGROUND Prefs::hud_background() const
{
  return _hud_background;
}

void Prefs::hud_background_color(const djv_image::Color & in)
{
  if (in == _hud_background_color)
    return;
  
  _hud_background_color = in;
  
  djv_ui::global()->window_update();
}

const djv_image::Color & Prefs::hud_background_color() const
{
  return _hud_background_color;
}

//------------------------------------------------------------------------------

}
}
}

