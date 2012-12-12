/*------------------------------------------------------------------------------
 bin/djv_view/file_prefs.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_FILE_PREFS_H
#define DJV_VIEW_FILE_PREFS_H

#include "cache.h"
#include <djv_ui/widget.h>
#include <djv_image/data.h>

namespace djv_bin {
namespace view {
namespace file {

//------------------------------------------------------------------------------
//!\class Prefs
//
//! File preferences.
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
  //!\name Recent Files
  //@{
  
  void recent(const String &);
  
  const List<String> & recent() const;
  
  djv_ui::callback::Signal<const List<String> &> recent_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name File Sequences
  //@{

  void seq_auto(bool);
  
  bool seq_auto() const;
  
  djv_ui::callback::Signal<bool> seq_auto_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Command Line
  //@{
  
  void command_line_combine(bool);
  
  bool command_line_combine() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Proxy Scale
  //@{
  
  void proxy(djv_image::data::PROXY);
  
  djv_image::data::PROXY proxy() const;
  
  djv_ui::callback::Signal<djv_image::data::PROXY> proxy_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Cache
  //@{
  
  void cache(bool);
  void cache_size(int);
  void cache_type(cache::CACHE);
  void cache_display(bool);
  
  bool cache() const;
  int cache_size() const;
  cache::CACHE cache_type() const;
  bool cache_display() const;
  
  djv_ui::callback::Signal<bool> cache_signal;
  
  //@}

private:

  List<String> _recent;
  bool _seq_auto;
  bool _command_line_combine;
  djv_image::data::PROXY _proxy;
  bool _cache;
  int _cache_size;
  cache::CACHE _cache_type;
  bool _cache_display;
};

//------------------------------------------------------------------------------
//!\class Prefs_Widget
//
//! File preferences widget.
//------------------------------------------------------------------------------

class Prefs_Widget : public djv_ui::Widget
{
public:

  Prefs_Widget();
  
private:

  DJV_CALLBACK(Prefs_Widget, seq_auto_callback, bool);
  DJV_CALLBACK(Prefs_Widget, command_line_combine_callback, bool);
  DJV_CALLBACK(Prefs_Widget, proxy_callback, int);
  DJV_CALLBACK(Prefs_Widget, cache_callback, bool);
  DJV_CALLBACK(Prefs_Widget, cache_size_callback, int);
  DJV_CALLBACK(Prefs_Widget, cache_type_callback, int);
  DJV_CALLBACK(Prefs_Widget, cache_display_callback, bool);
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  prefs_title,
  prefs_group,
  prefs_seq_auto,
  prefs_command_line_combine,
  prefs_proxy_group,
  prefs_proxy_text,
  prefs_cache_group,
  prefs_cache,
  prefs_cache_text,
  prefs_cache_size,
  prefs_cache_type,
  prefs_cache_display;

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

