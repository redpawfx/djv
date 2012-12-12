/*------------------------------------------------------------------------------
 bin/djv_view/file_group.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_FILE_GROUP_H
#define DJV_VIEW_FILE_GROUP_H

#include "cache.h"
#include <djv_ui/layout_row_def.h>
#include <djv_ui/menu_def.h>
#include <djv_image/io.h>
#include <djv_base/file.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::file
//
//! Files.
//------------------------------------------------------------------------------

namespace file {

//------------------------------------------------------------------------------
//!\class Group
//
//! File group.
//------------------------------------------------------------------------------

class Group : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Group(
    djv_ui::menu::Menu *,
    djv_ui::Layout_H * toolbar,
    const Group * copy = 0
  );
  
  ~Group();
  
  void del();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void open(const File &);

  const djv_image::Image * get(int64_t frame) const;
  
  const File & file() const;
  
  const djv_image::io::Info & info() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Options
  //@{
  
  void seq_auto(bool);

  bool seq_auto() const;

  void layer(int);

  int layer() const;

  void proxy(djv_image::data::PROXY);

  djv_image::data::PROXY proxy() const;

  void cache(bool);
  
  bool cache() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Signals
  //@{
  
  djv_ui::callback::Signal<const File &> open_signal;
  djv_ui::callback::Signal<bool> reload_signal;
  djv_ui::callback::Signal<const File &> save_signal;
  djv_ui::callback::Signal<const File &> save_frame_signal;
  djv_ui::callback::Signal<const List<int64_t> &> cache_signal;
  djv_ui::callback::Signal<bool> image_signal;
  djv_ui::callback::Signal<bool> image_frame_store_signal;
  djv_ui::callback::Signal<bool> update_signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  void menu_update(djv_ui::menu::Group *, bool seq);
  
  void toolbar_update();

  //@}

private:

  void cache_del();

  // Callbacks.
  
  DJV_CALLBACK(Group, open_callback, const File &);
  DJV_CALLBACK(Group, open_callback, bool);
  DJV_CALLBACK(Group, recent_callback, bool);
  DJV_CALLBACK(Group, recent_callback, const List<String> &);
  DJV_CALLBACK(Group, reload_callback, bool);
  DJV_CALLBACK(Group, reload_callback, djv_image::io::Base *);
  DJV_CALLBACK(Group, close_callback, bool);
  DJV_CALLBACK(Group, save_callback, const File &);
  DJV_CALLBACK(Group, save_check_callback, bool);
  DJV_CALLBACK(Group, save_frame_callback, const File &);
  DJV_CALLBACK(Group, save_frame_check_callback, bool);
  DJV_CALLBACK(Group, seq_auto_callback, bool);
  DJV_CALLBACK(Group, layer_callback, int);
  DJV_CALLBACK(Group, proxy_callback, djv_image::data::PROXY);
  DJV_CALLBACK(Group, cache_callback, bool);
  DJV_CALLBACK(Group, cache_clear_callback, bool);
  DJV_CALLBACK(Group, cache_update_callback, bool);
  
  DJV_FL_WIDGET_CALLBACK(Group, _open_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _recent_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _reload_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _close_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _save_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _save_frame_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _seq_auto_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _layer_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _layer_prev_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _layer_next_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _proxy_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _cache_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _cache_clear_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _messages_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _prefs_callback);
  DJV_FL_WIDGET_CALLBACK(Group, _exit_callback);

  // Variables.
  
  File _file;
  File _file_save;
  djv_image::io::Info _info;
  std::auto_ptr<djv_image::io::Load> _load;
  const djv_image::Image * _image;
  bool _seq_auto;
  int _layer;
  List<String> _layer_list;
  djv_image::data::PROXY _proxy;
  bool _cache;
  cache::Ref * _cache_ref;
  
  struct _Group;
  std::auto_ptr<_Group> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  menu_title,
  menu_open,
  menu_recent,
  menu_reload,
  menu_close,
  menu_save,
  menu_save_frame,
  menu_seq_auto,
  menu_layer,
  menu_layer_prev,
  menu_layer_next,
  menu_proxy,
  menu_cache,
  menu_cache_clear;

extern const String
  tooltip_open,
  tooltip_reload,
  tooltip_close;

extern const String
  dialog_save_check;

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

