/*------------------------------------------------------------------------------
 bin/djv_view/file_group.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_group.h"

#include "application.h"
#include "file_prefs.h"
#include "image_prefs.h"
#include "shortcut.h"
#include <djv_ui/application_dialog.h>
#include <djv_ui/file_browser.h>
#include <djv_ui/file_browser_prefs.h>
#include <djv_ui/image_io.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/menu.h>
#include <djv_ui/prefs.h>
#include <djv_ui/prefs_dialog.h>
#include <djv_ui/tool_button.h>
#include <djv_image/io.h>
#include <djv_base/directory.h>
#include <djv_base/file.h>
#include <FL/Fl.H>

namespace djv_bin {
namespace view {
namespace file {

//------------------------------------------------------------------------------
// Group::_Group
//------------------------------------------------------------------------------

struct Group::_Group
{
  djv_ui::menu::Menu * menu;
  List<int> menu_recent;
  List<int> menu_layer;
  List<int> menu_proxy;
  
  djv_ui::Tool_Button * open;
  djv_ui::Tool_Button * reload;
  djv_ui::Tool_Button * close;
  
  djv_ui::file_browser::Dialog * open_dialog;
  djv_ui::file_browser::Dialog * save_dialog;
};

//------------------------------------------------------------------------------
// Group
//------------------------------------------------------------------------------

Group::Group(
  djv_ui::menu::Menu * menu,
  djv_ui::Layout_H * toolbar,
  const Group * copy
) :
  open_signal(this),
  reload_signal(this),
  save_signal(this),
  save_frame_signal(this),
  cache_signal(this),
  image_signal(this),
  image_frame_store_signal(this),
  update_signal(this),
  _file(copy ? copy->_file : File()),
  _image(0),
  _seq_auto(copy ? copy->_seq_auto : prefs()->seq_auto()),
  _layer(copy ? copy->_layer : 0),
  _proxy(copy ? copy->_proxy : prefs()->proxy()),
  _cache(copy ? copy->_cache : prefs()->cache()),
  _cache_ref(0),
  _p(new _Group)
{
  using namespace djv_ui;
  
  // Create widgets.
  
  _p->menu = menu;
  
  _p->open =
    new Tool_Button("file_open");
  _p->reload =
    new Tool_Button("file_reload");
  _p->close =
    new Tool_Button("file_close");

  _p->open_dialog =
    new file_browser::Dialog(
      new file_browser::Prefs_Instance(djv_ui::prefs::prefs(), "file/open")
    );
  
  _p->save_dialog =
    new file_browser::Dialog(
      new file_browser::Prefs_Instance(djv_ui::prefs::prefs(), "file/save")
    );
  
  // Layout.
  
  toolbar->add(new Frame_Group_H(List<Frame *>() <<
    _p->open <<
    _p->reload <<
    _p->close));
  
  // Initialize.
  
  if (copy)
    open(_file);
  
  // Callbacks.
  
  _p->open->signal.set(this, open_callback);
  _p->reload->signal.set(this, reload_callback);
  _p->close->signal.set(this, close_callback);

  _p->open_dialog->signal.set(this, open_callback);
  //_p->save_dialog->signal.set(this, save_callback);
  
  prefs()->recent_signal.set(this, recent_callback);
  prefs()->seq_auto_signal.set(this, seq_auto_callback);
  prefs()->proxy_signal.set(this, proxy_callback);
  prefs()->cache_signal.set(this, cache_callback);

  cache::global()->signal.set(this, cache_update_callback);
  
  djv_ui::image_io::global()->signal.set(this, reload_callback);
}

Group::~Group()
{}

void Group::del()
{
  Callback_Base::del();
  
  _p->open_dialog->del();
  _p->save_dialog->del();
  
  // Cleanup.
  
  if (_cache_ref)
    _cache_ref->ref_del();
  
  cache_del();
}

void Group::menu_update(djv_ui::menu::Group * in, bool seq)
{
  using namespace djv_ui::menu;
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
    
  // Menu:
  //
  // * Open
  // * Recent
  //   * ...
  // * Auto Sequence
  // * Reload
  // * Close
  //   ---
  // * Save
  // * Save Frame
  //   ---
  // * Layer
  //   * ...
  // * Layer Previous
  // * Layer Next
  // * Proxy Scale
  //   * ...
  // * Memory Cache
  // * Clear Memory Cache
  //   ---
  // * Messages
  // * Preferences
  //   ---
  // *
  // * Quit
  
  in->add(menu_title, 0, 0, 0, SUB_MENU);
  
  in->add(
    menu_open,
    shortcut[shortcut::FILE_OPEN].value,
    _open_callback,
    this
  );
  
  in->add(menu_recent, 0, 0, 0, SUB_MENU);
  _p->menu_recent = in->add(
    prefs()->recent(),
    List<int>(),
    _recent_callback,
    this
  );
  in->end();

  in->add(
    menu_reload,
    shortcut[shortcut::FILE_RELOAD].value,
    _reload_callback,
    this
  );
  
  in->add(
    menu_close,
    shortcut[shortcut::FILE_CLOSE].value,
    _close_callback,
    this
  );

  in->add(
    menu_save,
    shortcut[shortcut::FILE_SAVE].value,
    _save_callback,
    this,
    ! _load.get() ? INACTIVE : 0
  );
  
  in->add(
    menu_save_frame,
    shortcut[shortcut::FILE_SAVE_FRAME].value,
    _save_frame_callback,
    this,
    ! _load.get() || ! seq ? INACTIVE : 0
  );
  
  in->add(
    menu_seq_auto,
    0,
    _seq_auto_callback,
    this,
    TOGGLE | DIVIDER,
    _seq_auto
  );
  
  in->add(menu_layer, 0, 0, 0, SUB_MENU);
  _p->menu_layer = in->add(
    _layer_list,
    List<int>() <<
      shortcut[shortcut::FILE_LAYER_DEFAULT].value <<
      shortcut[shortcut::FILE_LAYER_1].value <<
      shortcut[shortcut::FILE_LAYER_2].value <<
      shortcut[shortcut::FILE_LAYER_3].value <<
      shortcut[shortcut::FILE_LAYER_4].value <<
      shortcut[shortcut::FILE_LAYER_5].value <<
      shortcut[shortcut::FILE_LAYER_6].value <<
      shortcut[shortcut::FILE_LAYER_7].value <<
      shortcut[shortcut::FILE_LAYER_8].value <<
      shortcut[shortcut::FILE_LAYER_9].value <<
      shortcut[shortcut::FILE_LAYER_10].value,
    _layer_callback,
    this,
    RADIO,
    _layer
  );
  in->end();
  
  in->add(
    menu_layer_prev,
    shortcut[shortcut::FILE_LAYER_PREV].value,
    _layer_prev_callback,
    this
  );
  
  in->add(
    menu_layer_next,
    shortcut[shortcut::FILE_LAYER_NEXT].value,
    _layer_next_callback,
    this
  );

  in->add(menu_proxy, 0, 0, 0, SUB_MENU | DIVIDER);
  _p->menu_proxy = in->add(
    djv_image::data::label_proxy(),
    List<int>() <<
      shortcut[shortcut::FILE_PROXY_NONE].value <<
      shortcut[shortcut::FILE_PROXY_1_2].value <<
      shortcut[shortcut::FILE_PROXY_1_4].value <<
      shortcut[shortcut::FILE_PROXY_1_8].value,
    _proxy_callback,
    this,
    RADIO,
    _proxy
  );
  in->end();

  in->add(
    menu_cache,
    0,
    _cache_callback,
    this,
    TOGGLE,
    _cache
  );

  in->add(
    menu_cache_clear,
    0,
    _cache_clear_callback,
    this,
    DIVIDER
  );

  in->add(djv_ui::menu::menu_messages, 0, _messages_callback, this);
  in->add(djv_ui::menu::menu_prefs, 0, _prefs_callback, this, DIVIDER);

  in->add(
    djv_ui::menu::menu_exit,
    shortcut[shortcut::EXIT].value,
    _exit_callback,
    this
  );
  
  in->end();
}

void Group::toolbar_update()
{
  // Update tooltips.
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
  
  _p->open->tooltip(string::Format(tooltip_open).
    arg(djv_ui::shortcut::label(shortcut[shortcut::FILE_OPEN].value)));

  _p->reload->tooltip(string::Format(tooltip_reload).
    arg(djv_ui::shortcut::label(shortcut[shortcut::FILE_RELOAD].value)));

  _p->close->tooltip(string::Format(tooltip_close).
    arg(djv_ui::shortcut::label(shortcut[shortcut::FILE_CLOSE].value)));
}

void Group::open(const File & in)
{
  DJV_DEBUG("file::Group::open");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("in type = " << in.type());
  
  File file = in;

  // Initialize.

  cache_del();
  
  _file = File();
  _info = djv_image::io::Info();
  _load.reset();
  
  // Match wildcards.
  
  if (file.is_seq_wildcard())
    file = djv_base::file::seq_wildcard_match(
      file,
      Directory(file.path()).list()
    );
  
  DJV_DEBUG_PRINT("file = " << file);
  DJV_DEBUG_PRINT("file type = " << file.type());
  DJV_DEBUG_PRINT("seq valid = " << file.is_seq_valid());
  DJV_DEBUG_PRINT("seq = " << file.seq());
  
  // Automatic sequence loading.
  
  if (
    _seq_auto &&
    file.is_seq_valid() &&
    file.seq().list.size() == 1
  ) {
    file.type(djv_base::file::SEQ);
    
    const List<File> list = Directory(file.path()).list();

    file_seq::Seq seq = file.seq();
    seq.list.clear();
    file.seq(seq);
    
    for (list::size_type i = 0; i < list.size(); ++i)
      file.seq_add(list[i]);

    djv_base::file::seq_sort(file);

    DJV_DEBUG_PRINT("sequence = " << file);
  }

  // Load.
  
  if (! file.get().empty())
  {
    DJV_DEBUG_PRINT("loading...");
    
    try
    {
      _load = std::auto_ptr<djv_image::io::Load>(
        djv_image::io::global()->load(file, &_info)
      );
      
      _file = file;

      prefs()->recent(_file);
    }
    catch (Error error)
    {
      DJV_DEBUG_PRINT("error = " << error);
      
      application::global()->error(error);
    }
  }

  _layer = 0;
  _layer_list.resize(_info.layer_size());
  for (int i = 0; i < _info.layer_size(); ++i)
    _layer_list[i] = _info[i].layer_name;
}

void Group::open_callback(const File & in)
{
  open_signal.emit(in);
}

void Group::open_callback(bool)
{
  _p->open_dialog->show();
}

void Group::_open_callback()
{
  open_callback(true);
}

const djv_image::Image * Group::get(int64_t frame) const
{
  DJV_DEBUG("file::Group::get");
  DJV_DEBUG_PRINT("frame = " << frame);
  
  Group * that = const_cast<Group *>(this);
  
  that->_image = 0;
  
  cache::Ref * prev = _cache_ref;
  
  that->_cache_ref =
    _cache ?
      cache::global()->get(this, frame) :
      0;
  
  if (_cache_ref)
  {
    DJV_DEBUG_PRINT("cached image");
    
    that->_image = _cache_ref->get();
  }
  else
  {
    if (_load.get())
    {
      DJV_DEBUG_PRINT("loading image");

      try
      {
        that->_image = _load->get(
          djv_image::io::Frame_Info(
            _info.time.list.size() ? _info.time.list[frame] : -1,
            _layer,
            _proxy
          )
        );
      }
      catch (Error error)
      {
        DJV_DEBUG_PRINT("error" << error);
        
        application::global()->error(error);
      }
    }
    
    if (_image && _cache)
      that->_cache_ref = cache::global()->create(
        new djv_image::Image(*_image),
        this,
        frame
      );
  }
  
  if (prev)
    prev->ref_del();

  return _image;
}

const File & Group::file() const
{
  return _file;
}

const djv_image::io::Info & Group::info() const
{
  return _info;
}

void Group::recent_callback(bool)
{
  DJV_DEBUG("file::Group::recent_callback");
  
  File file = prefs()->recent()[list::find(_p->menu->item(), _p->menu_recent)];
  if (file.is_seq_valid())
    file.type(djv_base::file::SEQ);
  
  open_signal.emit(file);
}

void Group::_recent_callback()
{
  recent_callback(true);
}

void Group::recent_callback(const List<String> &)
{
  update_signal.emit(true);
}

void Group::reload_callback(bool)
{
  DJV_DEBUG("file::Group::reload_callback");
  
  if (image::prefs()->frame_store_file_reload())
    image_frame_store_signal.emit(true);

  cache_del();

  _load.reset();
  
  if (_file.get().size())
  {
    try
    {
      _load = std::auto_ptr<djv_image::io::Load>(
        djv_image::io::global()->load(_file, &_info)
      );
    }
    catch (Error error)
    {
      application::global()->error(error);
    }
  }
  
  reload_signal.emit(true);
}

void Group::reload_callback(djv_image::io::Base * in)
{
  if (! _load.get())
    return;
  
  DJV_DEBUG("file::Group::reload_callback");
  
  if (! in)
  {
    reload_callback(true);
  }
  else if (in->name() == _load->name())
  {
    cache_del();
    
    const List<String> & option_list = in->option_list();
    
    for (list::size_type i = 0; i < option_list.size(); ++i)
    {
      String tmp = in->option(option_list[i]);
      _load->option(option_list[i], &tmp);
    }
    
    reload_signal.emit(true);
  }
}

void Group::_reload_callback()
{
  reload_callback(true);
}

void Group::close_callback(bool)
{
  DJV_DEBUG("file::Group::close_callback");
  
  open_signal.emit(File());
}

void Group::_close_callback()
{
  close_callback(true);
}

void Group::save_callback(const File & in)
{
  DJV_DEBUG("file::Group::save_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  _file_save = in;
  
  if (djv_base::file::exists(_file_save))
  {
    DJV_DEBUG_PRINT("exists");
    
    djv_ui::dialog::global()->question(
      string::Format(dialog_save_check).arg(_file_save),
      this,
      save_check_callback
    );
  }
  else
  {
    save_signal.emit(_file_save);
  }
}

void Group::save_check_callback(bool)
{
  save_signal.emit(_file_save);
}

void Group::_save_callback()
{
  _p->save_dialog->signal.del(this);
  _p->save_dialog->signal.set(this, save_callback);
  
  _p->save_dialog->show();
}

void Group::save_frame_callback(const File & in)
{
  _file_save = in;

  if (djv_base::file::exists(_file_save))
  {
    djv_ui::dialog::global()->question(
      string::Format(dialog_save_check).arg(_file_save),
      this,
      save_frame_check_callback
    );
  }
  else
  {
    save_frame_signal.emit(_file_save);
  }
}

void Group::save_frame_check_callback(bool)
{
  save_frame_signal.emit(_file_save);
}

void Group::_save_frame_callback()
{
  _p->save_dialog->signal.del(this);
  _p->save_dialog->signal.set(this, save_frame_callback);
  
  _p->save_dialog->show();
}

void Group::seq_auto(bool in)
{
  if (in == _seq_auto)
    return;
  
  _seq_auto = in;
  
  update_signal.emit(true);
}

bool Group::seq_auto() const
{
  return _seq_auto;
}

void Group::seq_auto_callback(bool in)
{
  seq_auto(in);
}

void Group::_seq_auto_callback()
{
  seq_auto(_p->menu->value());
}

void Group::layer(int in)
{
  if (in == _layer)
    return;
  
  DJV_DEBUG("file::Group::layer");
  DJV_DEBUG_PRINT("in = " << in);
  
  const int size = static_cast<int>(_layer_list.size());
  
  DJV_DEBUG_PRINT("layer list = " << size);

  cache_del();
  
  _layer = math::wrap(in, 0, size - 1);
  
  DJV_DEBUG_PRINT("layer = " << _layer);
  
  image_signal.emit(true);
  update_signal.emit(true);
}

int Group::layer() const
{
  return _layer;
}

void Group::layer_callback(int in)
{
  layer(in);
}

void Group::_layer_callback()
{
  layer(list::find(_p->menu->item(), _p->menu_layer));
}

void Group::_layer_prev_callback()
{
  layer(_layer - 1);
}

void Group::_layer_next_callback()
{
  layer(_layer + 1);
}

void Group::proxy(djv_image::data::PROXY in)
{
  if (in == _proxy)
    return;
  
  DJV_DEBUG("file::Group::proxy");
  DJV_DEBUG_PRINT("in = " << in);

  cache_del();
  
  _proxy = in;
  
  image_signal.emit(true);
  update_signal.emit(true);
}

djv_image::data::PROXY Group::proxy() const
{
  return _proxy;
}

void Group::proxy_callback(djv_image::data::PROXY in)
{
  proxy(in);
}

void Group::_proxy_callback()
{
  proxy(static_cast<djv_image::data::PROXY>(
    list::find(_p->menu->item(), _p->menu_proxy)));
}

void Group::cache(bool in)
{
  if (in == _cache)
    return;
  
  DJV_DEBUG("file::Group::cache");
  DJV_DEBUG_PRINT("in = " << in);

  cache_del();
  
  _cache = in;
  
  update_signal.emit(true);
}

bool Group::cache() const
{
  return _cache;
}

void Group::cache_del()
{
  DJV_DEBUG("file::Group::cache_del");
  
  cache::global()->del(this);
}

void Group::cache_callback(bool in)
{
  cache(in);
}

void Group::_cache_callback()
{
  cache(_p->menu->value());
}

void Group::cache_clear_callback(bool)
{
  cache::global()->del();
}

void Group::_cache_clear_callback()
{
  cache_clear_callback(true);
}

void Group::cache_update_callback(bool)
{
  DJV_DEBUG("file::Group::cache_update_callback");
  
  cache_signal.emit(cache::global()->frame_list(this));
}

void Group::_messages_callback()
{
  djv_ui::application_dialog::global()->message()->show();
}

void Group::_prefs_callback()
{
  djv_ui::prefs_dialog::global()->show();
}

void Group::_exit_callback()
{
  application::global()->exit();
}

//------------------------------------------------------------------------------

}
}
}

