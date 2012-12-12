/*------------------------------------------------------------------------------
 lib/djv_base/file.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file.h"

#include <djv_base/math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

namespace djv_base {
namespace file {

//------------------------------------------------------------------------------
// File
//------------------------------------------------------------------------------

void File::init()
{
  _type = FILE;
  _size = 0;
  _user = 0;
  _perm = 0;
  _time = 0;
  _has_seq_extension = false;
}

File::File()
{
  init();
}

File::File(const String & in, bool stat)
{
  init();
  set(in, stat);
}

void File::set(const String & in, bool stat)
{
  DJV_DEBUG("File::set");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("stat = " << stat);

  init();
  
  // Split file name into pieces.
  
  split(in, _path, _base, _number, _extension);
  
  DJV_DEBUG_PRINT("path = " << _path);
  DJV_DEBUG_PRINT("base = " << _base);
  DJV_DEBUG_PRINT("number = " << _number);
  DJV_DEBUG_PRINT("extension = " << _extension);

  // Information.
  
  if (stat)
    this->stat();
  
  // Sequence.
  
  _seq = file_seq::string_to_seq(_number);

  update_has_seq_extension();
  
  DJV_DEBUG_PRINT("seq = " << _seq);
  DJV_DEBUG_PRINT("has seq extension = " << _has_seq_extension);
}

String File::get(int64_t frame, bool path) const
{
  String out;
  
  // Reconstruct pieces.

  if (path)
    out += _path;
  out += _base;
  
  if (SEQ == _type && frame != -1)
    out += file_seq::frame_to_string(frame, _seq.pad);
  else if (SEQ == _type && _seq.list.size())
    out += file_seq::seq_to_string(_seq);
  else
    out += _number;
  
  out += _extension;

  return out;
}

String File::name() const
{
  return _base + _number + _extension;
}

void File::path(const String & in)
{
  _path = in;
}

void File::base(const String & in)
{
  _base = in;
}

void File::number(const String & in)
{
  _number = in;
}

void File::extension(const String & in)
{
  _extension = in;
  
  update_has_seq_extension();
}

void File::type(TYPE in)
{
  _type = in;
}

void File::size(uint64_t in)
{
  _size = in;
}

void File::user(uid_t in)
{
  _user = in;
}

void File::perm(int in)
{
  _perm = in;
}

void File::time(time_t in)
{
  _time = in;
}

#if defined(DJV_WINDOWS)

#define _STAT struct ::_stati64
#define _STAT_FNC ::_stati64

#elif (defined(DJV_FREEBSD) || defined(DJV_MACOSX))

// XXX No stat64?

#define _STAT struct ::stat
#define _STAT_FNC ::stat

#else

#define _STAT struct ::stat64
#define _STAT_FNC ::stat64

#endif // DJV_WINDOWS

bool File::stat(const String & path)
{
  DJV_DEBUG("File::stat");
  DJV_DEBUG_PRINT("path = " << path);
  
  _STAT info;
  memory::set<uint8_t>(0, &info, sizeof(_STAT));

  const String file = path_fix(path.size() ? path : _path) + get(-1, false);
  DJV_DEBUG_PRINT("file = " << file);

  // XXX Windows stat.

  if (0 == _STAT_FNC(file.c_str(), &info))
    ;
  else if (0 == _STAT_FNC(String(file + path_separator).c_str(), &info))
    ;
  else if (
    0 == _STAT_FNC(
      String(file, static_cast<int>(file.size()) - 1).c_str(), &info
    )
  )
    ;
  else
  {
    String err;

#if defined(DJV_WINDOWS)

    char tmp[string::cstring_size] = "";
    ::strerror_s(tmp, string::cstring_size, errno);
    err = tmp;

#else // DJV_WINDOWS

    err = ::strerror(errno);

#endif // DJV_WINDOWS

    DJV_DEBUG_PRINT("errno = " << err);

    return false;
  }

  _size = info.st_size;
  _user = info.st_uid;
  _perm = 0;
  _time = info.st_mtime;
  
#if defined(DJV_WINDOWS)

  if (info.st_mode & _S_IFDIR)
    _type = DIRECTORY;

  _perm |= (info.st_mode & _S_IREAD) ? READ : 0;
  _perm |= (info.st_mode & _S_IWRITE) ? WRITE : 0;
  _perm |= (info.st_mode & _S_IEXEC) ? EXEC : 0;

#else // DJV_WINDOWS

  if (S_ISDIR(info.st_mode))
    _type = DIRECTORY;

  _perm |= (info.st_mode & S_IRUSR) ? READ : 0;
  _perm |= (info.st_mode & S_IWUSR) ? WRITE : 0;
  _perm |= (info.st_mode & S_IXUSR) ? EXEC : 0;

#endif // DJV_WINDOWS
  
  return true;
}

void File::seq(const file_seq::Seq & in)
{
  _seq = in;
}
  
List<String> File::_seq_extension_list;

void File::seq_extension_list(const List<String> & in)
{
  _seq_extension_list = in;
}
  
const List<String> & File::seq_extension_list()
{
  return _seq_extension_list;
}

void File::update_has_seq_extension()
{
  _has_seq_extension = find_no_case(_extension, _seq_extension_list);
}

//------------------------------------------------------------------------------

}
}

