/*------------------------------------------------------------------------------
 include/djv_base/file_inline.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include <djv_base/math.h>

namespace djv_base {
namespace file {

//------------------------------------------------------------------------------
// File
//------------------------------------------------------------------------------

inline const String & File::path() const
{
  return _path;
}

inline const String & File::base() const
{
  return _base;
}

inline const String & File::number() const 
{
  return _number;
}

inline const String & File::extension() const
{
  return _extension;
}

inline TYPE File::type () const
{
  return _type;
}

inline uint64_t File::size () const
{
  return _size;
}

inline uid_t File::user () const
{
  return _user;
}

inline int File::perm () const
{
  return _perm;
}

inline time_t File::time () const
{
  return _time;
}

inline const file_seq::Seq & File::seq() const
{
  return _seq;
}

inline bool File::is_seq_valid() const
{
  return
    _type != DIRECTORY &&
    _number.size() &&
    _seq.list.size() &&
    _has_seq_extension;
}

inline bool File::is_seq_wildcard() const
{
  return
    _type != DIRECTORY &&
    _number.size() &&
    '#' == _number[0];
}

inline bool File::seq_add(const File & in)
{
  // Compare.
  
  if (_extension != in._extension)
    return false;
  if (_base != in._base)
    return false;
  if (! in.is_seq_valid())
    return false;
  //if (is_seq_valid() && _seq.pad != in._seq.pad)
  //  return false;

  // Update sequence.
  
  _seq.list += in._seq.list;
  _seq.pad = math::max(_seq.pad, in._seq.pad);

  // Update information.
  
  _size += in._size;
  _user = math::max(_user, in._user);
  _time = math::max(_time, in._time);

  return true;
}

inline bool File::has_seq_extension() const
{
  return _has_seq_extension;
}

inline File::operator String() const
{
  return get();
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

inline bool operator == (const File & a, const File & b)
{
  return a.get() == b.get();
}

inline bool operator != (const File & a, const File & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------

}
}

