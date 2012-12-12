/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_prefs.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_browser_private.h"

#include <djv_ui/global.h>
#include <djv_ui/file.h>
#include <djv_ui/layout_row.h>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs() :
  _navigate(NAVIGATE_SINGLE_CLICK)
{
  DJV_DEBUG("Prefs::Prefs");

  using djv_ui::shortcut::Shortcut;
  
  djv_ui::prefs::Prefs prefs(prefs::prefs_global(), "file_browser");

  prefs::get(&prefs, "recent", &_recent);
  prefs::get(&prefs, "bookmark", &_bookmark);
  prefs::get(&prefs, "navigate", &_navigate);

  const List<String> & label_shortcut = file_browser::label_shortcut();
  _shortcut += Shortcut(label_shortcut[UP], FL_COMMAND + '\\');
  _shortcut += Shortcut(label_shortcut[PREV], FL_COMMAND + FL_BackSpace);
  _shortcut += Shortcut(label_shortcut[CURRENT], FL_COMMAND + '.');
  _shortcut += Shortcut(label_shortcut[HOME], FL_COMMAND + ',');
  _shortcut += Shortcut(label_shortcut[DESKTOP], FL_COMMAND + 'k');
  _shortcut += Shortcut(label_shortcut[ROOT], FL_COMMAND + '/');
  _shortcut += Shortcut(label_shortcut[RELOAD], FL_COMMAND + 'r');
  _shortcut += Shortcut(label_shortcut[SEQ_OFF], FL_COMMAND + 'q');
  _shortcut += Shortcut(label_shortcut[SEQ_SPARSE], FL_COMMAND + 'a');
  _shortcut += Shortcut(label_shortcut[SEQ_RANGE], FL_COMMAND + 'z');
  _shortcut += Shortcut(label_shortcut[HIDDEN], FL_COMMAND + 'n');
  _shortcut += Shortcut(label_shortcut[SORT_NAME], FL_COMMAND + '1');
  _shortcut += Shortcut(label_shortcut[SORT_SIZE], FL_COMMAND + '2');
#if ! defined(DJV_WINDOWS)
  _shortcut += Shortcut(label_shortcut[SORT_USER], FL_COMMAND + '3');
  _shortcut += Shortcut(label_shortcut[SORT_TIME], FL_COMMAND + '4');
#else
  _shortcut += Shortcut(label_shortcut[SORT_TIME], FL_COMMAND + '3');
#endif
  _shortcut += Shortcut(label_shortcut[SORT_REVERSE], FL_COMMAND + '`');
  _shortcut += Shortcut(label_shortcut[SORT_DIRECTORY], FL_COMMAND + 'd');
  _shortcut += Shortcut(label_shortcut[BOOKMARK_ADD], FL_COMMAND + 'b');
  _shortcut += Shortcut(label_shortcut[BOOKMARK_1], FL_F + 1);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_2], FL_F + 2);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_3], FL_F + 3);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_4], FL_F + 4);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_5], FL_F + 5);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_6], FL_F + 6);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_7], FL_F + 7);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_8], FL_F + 8);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_9], FL_F + 9);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_10], FL_F + 10);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_11], FL_F + 11);
  _shortcut += Shortcut(label_shortcut[BOOKMARK_12], FL_F + 12);
  
  djv_ui::prefs::Prefs prefs_shortcut(prefs, "shortcut");
  String tmp;
  for (list::size_type i = 0; i < _shortcut.size(); ++i)
    if (prefs_shortcut.get(_shortcut[i].name, &tmp))
      shortcut::serialize(&tmp, &_shortcut[i].value);
}

Prefs::~Prefs()
{
  DJV_DEBUG("Prefs::~Prefs");

  djv_ui::prefs::Prefs prefs(prefs::prefs_global(), "file_browser");
  
  prefs::set(&prefs, "recent", _recent);
  prefs::set(&prefs, "bookmark", _bookmark);
  prefs::set(&prefs, "navigate", _navigate);
  
  djv_ui::prefs::Prefs prefs_shortcut(prefs, "shortcut");
  for (list::size_type i = 0; i < _shortcut.size(); ++i)
    prefs_shortcut.set(
      _shortcut[i].name,
      shortcut::serialize(_shortcut[i].value)
    );
}

void Prefs::recent(const List<String> & in)
{
  _recent = in;
}

void Prefs::recent_add(const String & in)
{
  djv_ui::file::recent(in, _recent);
}

const List<String> & Prefs::recent() const
{
  return _recent;
}

void Prefs::bookmark(const List<String> & in)
{
  _bookmark = in;
}

void Prefs::bookmark_add(const String & in)
{
  _bookmark += in;
}

const List<String> & Prefs::bookmark() const
{
  return _bookmark;
}

void Prefs::navigate(NAVIGATE in)
{
  if (in == _navigate)
    return;
  
  _navigate = in;
}

NAVIGATE Prefs::navigate() const
{
  return _navigate;
}

void Prefs::shortcut(const List<shortcut::Shortcut> & in)
{
  if (in == _shortcut)
    return;
  
  _shortcut = in;
  
  djv_ui::global()->window_update();
}

const List<shortcut::Shortcut> & Prefs::shortcut() const
{
  return _shortcut;
}

//------------------------------------------------------------------------------
// Prefs_Instance
//------------------------------------------------------------------------------

Prefs_Instance::Prefs_Instance(
  prefs::Prefs * prefs,
  const String & prefs_name
) :
  _prefs(prefs),
  _prefs_name(prefs_name),
  _image(IMAGE_HIGH),
  _seq(file_seq::COMPRESS_RANGE),
  _hidden(false),
  _sort(file_browser::SORT_NAME),
  _sort_reverse(false),
  _sort_directory(true),
  _type(-1),
  _pin(false)
{
  DJV_DEBUG("Prefs_Instance::Prefs_Instance");

  using djv_ui::shortcut::Shortcut;
  
  djv_ui::prefs::Prefs prefs_(_prefs, _prefs_name);

  prefs::get(&prefs_, "image", &_image);
  prefs::get(&prefs_, "seq", &_seq);
  prefs::get(&prefs_, "hidden", &_hidden);
  prefs::get(&prefs_, "sort", &_sort);
  prefs::get(&prefs_, "sort_reverse", &_sort_reverse);
  prefs::get(&prefs_, "sort_directory", &_sort_directory);
  prefs::get(&prefs_, "type", &_type);
  prefs::get(&prefs_, "pin", &_pin);
}

Prefs_Instance::~Prefs_Instance()
{
  DJV_DEBUG("Prefs_Instance::~Prefs_Instance");

  djv_ui::prefs::Prefs prefs(_prefs, _prefs_name);
  
  prefs::set(&prefs, "image", _image);
  prefs::set(&prefs, "seq", _seq);
  prefs::set(&prefs, "hidden", _hidden);
  prefs::set(&prefs, "sort", _sort);
  prefs::set(&prefs, "sort_reverse", _sort_reverse);
  prefs::set(&prefs, "sort_directory", _sort_directory);
  prefs::set(&prefs, "type", _type);
  prefs::set(&prefs, "pin", _pin);
}

void Prefs_Instance::image(IMAGE in)
{
  _image = in;
}

IMAGE Prefs_Instance::image() const
{
  return _image;
}

void Prefs_Instance::seq(file_seq::COMPRESS in)
{
  _seq = in;
}

file_seq::COMPRESS Prefs_Instance::seq() const
{
  return _seq;
}

void Prefs_Instance::hidden(bool in)
{
  _hidden = in;
}

bool Prefs_Instance::hidden() const
{
  return _hidden;
}
  
void Prefs_Instance::sort(SORT in)
{
  _sort = in;
}

SORT Prefs_Instance::sort() const
{
  return _sort;
}

void Prefs_Instance::sort_reverse(bool in)
{
  _sort_reverse = in;
}

bool Prefs_Instance::sort_reverse() const
{
  return _sort_reverse;
}

void Prefs_Instance::sort_directory(bool in)
{
  _sort_directory = in;
}

bool Prefs_Instance::sort_directory() const
{
  return _sort_directory;
}

void Prefs_Instance::type(int in)
{
  _type = in;
}

int Prefs_Instance::type() const
{
  return _type;
}

void Prefs_Instance::pin(bool in)
{
  _pin = in;
}

bool Prefs_Instance::pin() const
{
  return _pin;
}

//------------------------------------------------------------------------------

}
}

