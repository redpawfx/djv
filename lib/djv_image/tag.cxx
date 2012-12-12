/*------------------------------------------------------------------------------
 lib/djv_image/tag.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "tag.h"

namespace djv_image {
namespace tag {

//------------------------------------------------------------------------------
// Tag
//------------------------------------------------------------------------------
  
void Tag::add(const Tag & in)
{
  for (list::size_type i = 0; i < in._list.size(); ++i)
    add(in._list[i].first, in._list[i].second);
}

void Tag::add(const String & key, const String & value)
{
  for (list::size_type i = 0; i < _list.size(); ++i)
    if (key == _list[i].first)
    {
      _list[i].second = value;
      return;
    }
  
  _list += Pair(key, value);
}

String Tag::get(const String & key) const
{
  for (list::size_type i = 0; i < _list.size(); ++i)
    if (key == _list[i].first)
      return _list[i].second;
  
  return String();
}

List<String> Tag::key_list() const
{
  List<String> out(_list.size());
  for (list::size_type i = 0; i < _list.size(); ++i)
    out[i] = _list[i].first;

  return out;
}

List<String> Tag::value_list() const
{
  List<String> out(_list.size());
  for (list::size_type i = 0; i < _list.size(); ++i)
    out[i] = _list[i].second;

  return out;
}

int Tag::size() const
{
  return static_cast<int>(_list.size());
}

bool Tag::is_valid(const String & key)
{
  bool r = false;
  
  for (list::size_type i = 0; i < _list.size(); ++i)
    if (key == _list[i].first)
    {
      r = true;
      break;
    }

  return r;
}

void Tag::clear()
{
  _list.clear();
}

String & Tag::operator [] (const String & key)
{
  for (list::size_type i = 0; i < _list.size(); ++i)
    if (key == _list[i].first)
      return _list[i].second;

  _list += Pair(key, String());
  return _list[_list.size() - 1].second;
}

String Tag::operator [] (const String & key) const
{
  return get(key);
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Tag & a, const Tag & b)
{
  if (a.key_list() != b.key_list())
    return false;
  if (a.value_list() != b.value_list())
    return false;
  return true;
}

bool operator != (const Tag & a, const Tag & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_tag()
{
  static const List<String> data = List<String>() <<
    "Project" <<
    "Creator" <<
    "Description" <<
    "Copyright" <<
    "Time" <<
    "UTC offset" <<
    "Keycode" <<
    "Timecode";

  DJV_ASSERT(data.size() == _TAG_SIZE);

  return data;
}

//------------------------------------------------------------------------------

}
}
