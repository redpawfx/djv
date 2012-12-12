/*------------------------------------------------------------------------------
 bin/djv_view/file_prefs.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_prefs.h"

#include "application.h"
#include <djv_ui/file.h>
#include <djv_ui/global.h>
#include <djv_ui/prefs.h>

namespace djv_bin {
namespace view {
namespace file {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs() :
  recent_signal(this),
  seq_auto_signal(this),
  proxy_signal(this),
  cache_signal(this),
  _seq_auto(true),
  _command_line_combine(false),
  _proxy(djv_image::data::PROXY(0)),
  _cache(false),
  _cache_size(cache::default_size()[4]),
  _cache_type(cache::CACHE_LRU_PLAYBACK),
  _cache_display(true)
{
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "file");

  djv_ui::prefs::get(&prefs, "recent", &_recent);
  djv_ui::prefs::get(&prefs, "seq_auto", &_seq_auto);
  djv_ui::prefs::get(&prefs, "command_line_combine", &_command_line_combine);
  djv_ui::prefs::get(&prefs, "proxy", &_proxy);
  djv_ui::prefs::get(&prefs, "cache", &_cache);
  djv_ui::prefs::get(&prefs, "cache_size", &_cache_size);
  djv_ui::prefs::get(&prefs, "cache_type", &_cache_type);
  djv_ui::prefs::get(&prefs, "cache_display", &_cache_display);
  
  cache::global()->max(_cache_size);
  cache::global()->type(_cache_type);
}

Prefs::~Prefs()
{
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "file");
  
  djv_ui::prefs::set(&prefs, "recent", _recent);
  djv_ui::prefs::set(&prefs, "seq_auto", _seq_auto);
  djv_ui::prefs::set(&prefs, "command_line_combine", _command_line_combine);
  djv_ui::prefs::set(&prefs, "proxy", _proxy);
  djv_ui::prefs::set(&prefs, "cache", _cache);
  djv_ui::prefs::set(&prefs, "cache_size", _cache_size);
  djv_ui::prefs::set(&prefs, "cache_type", _cache_type);
  djv_ui::prefs::set(&prefs, "cache_display", _cache_display);
}

void Prefs::recent(const String & in)
{
  DJV_DEBUG("Prefs::recent");
  DJV_DEBUG_PRINT("in = " << in);
  
  djv_ui::file::recent(in, _recent);
  
  recent_signal.emit(_recent);
}

const List<String> & Prefs::recent() const
{
  return _recent;
}

void Prefs::seq_auto(bool in)
{
  if (in == _seq_auto)
    return;
  
  _seq_auto = in;
  
  seq_auto_signal.emit(_seq_auto);
}

bool Prefs::seq_auto() const
{
  return _seq_auto;
}

void Prefs::command_line_combine(bool in)
{
  _command_line_combine = in;
}

bool Prefs::command_line_combine() const
{
  return _command_line_combine;
}

void Prefs::proxy(djv_image::data::PROXY in)
{
  if (in == _proxy)
    return;
  
  _proxy = in;
  
  proxy_signal.emit(_proxy);
}

djv_image::data::PROXY Prefs::proxy() const
{
  return _proxy;
}

void Prefs::cache(bool in)
{
  if (in == _cache)
    return;
  
  _cache = in;
  
  if (! _cache)
    cache::global()->del();

  cache_signal.emit(_cache);
}

bool Prefs::cache() const
{
  return _cache;
}

void Prefs::cache_size(int in)
{
  if (in == _cache_size)
    return;
  
  DJV_DEBUG("Prefs::cache_size");
  DJV_DEBUG_PRINT("in = " << in);
  
  _cache_size = in;
  
  cache::global()->max(_cache_size);
}

int Prefs::cache_size() const
{
  return _cache_size;
}

void Prefs::cache_type(cache::CACHE in)
{
  if (in == _cache_type)
    return;
  
  DJV_DEBUG("Prefs::cache_type");
  DJV_DEBUG_PRINT("in = " << in);
  
  _cache_type = in;
  
  cache::global()->type(_cache_type);
}

cache::CACHE Prefs::cache_type() const
{
  return _cache_type;
}

void Prefs::cache_display(bool in)
{
  if (in == _cache_display)
    return;
  
  _cache_display = in;
  
  djv_ui::global()->window_update();
}

bool Prefs::cache_display() const
{
  return _cache_display;
}

//------------------------------------------------------------------------------

}
}
}

