/*------------------------------------------------------------------------------
 lib/djv_base/file_io.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG
//#undef DJV_MMAP

#include "file_io.h"

#include <djv_base/file.h>
#if defined(DJV_MMAP)
#if ! defined(DJV_WINDOWS)
#include <sys/mman.h>
#endif // ! DJV_WINDOWS
#endif // DJV_MMAP
#if defined(DJV_WINDOWS)
#include <io.h>
#else // DJV_WINDOWS
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#endif // DJV_WINDOWS
#include <errno.h>
#include <stdio.h>
#include <string.h>

namespace djv_base {
namespace file_io {

//------------------------------------------------------------------------------
// File_Io
//------------------------------------------------------------------------------

File_Io::File_Io() :

#if defined(DJV_WINDOWS)

  _f(INVALID_HANDLE_VALUE),
  _mode(MODE(0)),
  _size(0),
  _endian(false),
  _mmap(0), _mmap_start(0), _mmap_end(0), _mmap_p(0)

#else // DJV_WINDOWS

  _f(-1),
  _mode(MODE(0)),
  _size(0),
  _endian(false),
  _mmap((void *)-1), _mmap_start(0), _mmap_end(0), _mmap_p(0)

#endif // DJV_WINDOWS

{}

File_Io::~File_Io()
{
  close();
}

void File_Io::open(const String & file_name, MODE mode) throw (Error)
{
  DJV_DEBUG("File_Io::open");
  DJV_DEBUG_PRINT("file name = " << file_name);
  DJV_DEBUG_PRINT("mode = " << mode);

  close();

  // Open file.
  
#if defined(DJV_WINDOWS)

  _f = ::CreateFile(
    file_name.c_str(),
    (WRITE == mode) ? GENERIC_WRITE : GENERIC_READ,
    (WRITE == mode) ? 0 : FILE_SHARE_READ,
    0,
    (WRITE == mode) ? CREATE_ALWAYS : OPEN_EXISTING,
    //FILE_ATTRIBUTE_NORMAL,
    //FILE_FLAG_NO_BUFFERING |
    FILE_FLAG_SEQUENTIAL_SCAN,
    0
  );

  if (INVALID_HANDLE_VALUE == _f)
    throw Error(error, string::Format(error_open).arg(file_name));
  
#else // DJV_WINDOWS

  int read_flag = 0;
#if defined(DJV_LINUX)
  //read_flag = O_DIRECT;
#endif
  
  _f = ::open(
    file_name.c_str(),
    (WRITE == mode) ? (O_WRONLY | O_CREAT | O_TRUNC) : (O_RDONLY | read_flag),
    (WRITE == mode) ? (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) : (0)
  );

  if (-1 == _f)
    throw Error(error, string::Format(error_open).arg(file_name));

#endif // DJV_WINDOWS

  _file_name = file_name;

  // File size.
  
#if defined(DJV_WINDOWS)
  _size = ::GetFileSize(_f, 0);
#else
  _size = File(file_name).size();
#endif
  DJV_DEBUG_PRINT("size = " << _size);

  // I/O mode.
  
  _mode = mode;
  
  // Memory maping.
  
#if defined(DJV_MMAP)

  if (READ == _mode)
  {
    DJV_DEBUG_PRINT("mmap");
  
#if defined(DJV_WINDOWS)

    if (! (_mmap = ::CreateFileMapping(_f, 0, PAGE_READONLY, 0, 0, 0)))
      throw Error(error, string::Format(error_mmap).arg(_file_name));
    
    _mmap_start = reinterpret_cast<const uint8_t *>(
      ::MapViewOfFile(_mmap, FILE_MAP_READ, 0, 0, 0)
    );
    
    if (! _mmap_start)
      throw Error(error, string::Format(error_mmap).arg(_file_name));

    _mmap_end = _mmap_start + _size;
    _mmap_p = _mmap_start;

#else // DJV_WINDOWS

    if (
      ((_mmap = ::mmap(0, _size, PROT_READ, MAP_SHARED, _f, 0)) == (void *)-1)
    )
      throw Error(error, string::Format(error_mmap).arg(_file_name));

    _mmap_start = reinterpret_cast<const uint8_t *>(_mmap);
    _mmap_end = _mmap_start + _size;
    _mmap_p = _mmap_start;

#endif // DJV_WINDOWS
  }
#endif // DJV_MMAP
}

void File_Io::close()
{
  DJV_DEBUG("File_Io::close");

#if defined(DJV_MMAP)
#if defined(DJV_WINDOWS)

  if (_mmap_start != 0)
  {
    ::UnmapViewOfFile((void *)_mmap_start);
    _mmap_start = 0;
  }
  
  if (_mmap != 0)
  {
    ::CloseHandle(_mmap);
    _mmap = 0;
  }

#else // DJV_WINDOWS

  _mmap_start = 0;
  
  if (_mmap != (void *)-1)
  {
    DJV_DEBUG_PRINT("munmap");
    
    // XXX Solaris wants (char *)?
    
    int r = ::munmap((char *)_mmap, _size);
    
    if (-1 == r)
    {
      const String err(::strerror(errno));
      DJV_DEBUG_PRINT("errno = " << err);
    }
    
    _mmap = (void *)-1;
  }

#endif // DJV_WINDOWS

  _mmap_end = 0;
  _mmap_p = 0;

#endif // DJV_MMAP

  _file_name.clear();

#if defined(DJV_WINDOWS)

  if (_f != INVALID_HANDLE_VALUE)
  {
    ::CloseHandle(_f);
    _f = INVALID_HANDLE_VALUE;
  }
  
#else // DJV_WINDOWS

  if (_f != -1)
  {
    DJV_DEBUG_PRINT("close");
    
    int r = ::close(_f);
    
    if (-1 == r)
    {
      const String err(::strerror(errno));
      DJV_DEBUG_PRINT("errno = " << err);
    }
    _f = -1;
  }
  
#endif // DJV_WINDOWS

  _size = 0;
  _mode = MODE(0);
}

const String & File_Io::file_name() const
{
  return _file_name;
}

bool File_Io::is_valid() const
{
  return
#if defined(DJV_WINDOWS)
    _f != INVALID_HANDLE_VALUE &&
#else
    _f != -1 &&
#endif
    position() < _size;
}

void File_Io::set(const void * in, size_t size, size_t word_size) throw (Error)
{
  DJV_DEBUG("File_Io::set");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("word size = " << word_size);
  DJV_DEBUG_PRINT("endian = " << _endian);
  
  uint8_t * p = (uint8_t *)in;
  
  memory::Buffer<uint8_t> tmp;
  
  if (_endian && word_size > 1)
  {
    tmp.size(size * word_size);
    p = tmp();
    memory::endian(in, p, size, word_size);
  }

#if defined(DJV_WINDOWS)
  DWORD n;
  if (! ::WriteFile(_f, p, static_cast<DWORD>(size * word_size), &n, 0))
    throw Error(error, string::Format(error_write).arg(_file_name));
#else
  if (::write(_f, p, size * word_size) == -1)
    throw Error(error, string::Format(error_write).arg(_file_name));
#endif
}

void File_Io::set_8(const int8_t * in, size_t size) throw (Error)
{
  return set(in, size, 1);
}

void File_Io::set_u8(const uint8_t * in, size_t size) throw (Error)
{
  return set(in, size, 1);
}

void File_Io::set_16(const int16_t * in, size_t size) throw (Error)
{
  return set(in, size, 2);
}

void File_Io::set_u16(const uint16_t * in, size_t size) throw (Error)
{
  return set(in, size, 2);
}

void File_Io::set_32(const int32_t * in, size_t size) throw (Error)
{
  return set(in, size, 4);
}

void File_Io::set_u32(const uint32_t * in, size_t size) throw (Error)
{
  return set(in, size, 4);
}

void File_Io::set_f32(const float * in, size_t size) throw (Error)
{
  return set(in, size, 4);
}

void File_Io::get(void * in, size_t size, size_t word_size) throw (Error)
{
  DJV_DEBUG("File_Io::get");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("word size = " << word_size);

#if defined(DJV_MMAP)

  const uint8_t * p = _mmap_p + size * word_size;
  if (p > _mmap_end)
    throw Error(error, string::Format(error_read).arg(_file_name));
  
  if (_endian && word_size > 1)
    memory::endian(_mmap_p, in, size, word_size);
  else
    memory::copy(_mmap_p, in, size * word_size);

  _mmap_p = p;

#else // DJV_MMAP

#if defined(DJV_WINDOWS)

  DWORD n;
  if (! ::ReadFile(_f, in, size * word_size, &n, 0))
    throw Error(error, string::Format(error_read).arg(_file_name));

#else // DJV_WINDOWS

  //if (-1 == ::read(_f, in, size * word_size) == (size * word_size))
  //  throw Error(error, string::Format(error_read).arg(_file_name));
  ::read(_f, in, size * word_size);
  
  if (_endian && word_size > 1)
    memory::endian(in, size, word_size);

#endif // DJV_WINDOWS

#endif // DJV_MMAP
}

void File_Io::get_8(int8_t * in, size_t size) throw (Error)
{
  return get(in, size, 1);
}

void File_Io::get_u8(uint8_t * in, size_t size) throw (Error)
{
  return get(in, size, 1);
}

void File_Io::get_16(int16_t * in, size_t size) throw (Error)
{
  return get(in, size, 2);
}

void File_Io::get_u16(uint16_t * in, size_t size) throw (Error)
{
  return get(in, size, 2);
}

void File_Io::get_32(int32_t * in, size_t size) throw (Error)
{
  return get(in, size, 4);
}

void File_Io::get_u32(uint32_t * in, size_t size) throw (Error)
{
  return get(in, size, 4);
}

void File_Io::get_f32(float * in, size_t size) throw (Error)
{
  return get(in, size, 4);
}

void File_Io::read_ahead()
{
#if defined(DJV_MMAP)

#if defined(DJV_LINUX)

  ::madvise((void *)_mmap_start, _size, MADV_WILLNEED);

#elif defined(DJV_SOLARIS)

  ::madvise((caddr_t)_mmap_start, _size, MADV_WILLNEED);

#else

  // XXX Force read-ahead?

  for (const uint8_t * p = _mmap_start; p < _mmap_end; p += memory::align)
    _mmap_read_ahead = *p;

#endif

#else // DJV_MMAP

#if defined(DJV_LINUX)
  ::posix_fadvise(_f, 0, _size, POSIX_FADV_NOREUSE);
  ::posix_fadvise(_f, 0, _size, POSIX_FADV_WILLNEED);
#endif

#endif // DJV_MMAP
}

void File_Io::position(size_t in) throw (Error)
{
  position(in, false);
}

void File_Io::seek(size_t in) throw (Error)
{
  position(in, true);
}

void File_Io::position(size_t in, bool seek) throw (Error)
{
  DJV_DEBUG("File_Io::position");
  DJV_DEBUG_PRINT("in = " << static_cast<int>(in));
  DJV_DEBUG_PRINT("seek = " << seek);
  
  switch (_mode)
  {
    case READ:
#if defined(DJV_MMAP)
      if (! seek)
        _mmap_p = reinterpret_cast<const uint8_t *>(_mmap_start) + in;
      else
        _mmap_p += in;
      
      if (_mmap_p > _mmap_end)
        throw Error(error, string::Format(error_position).arg(_file_name));
      break;
#else 
      // Fall through...
#endif

    case WRITE:
    {
#if defined(DJV_WINDOWS)

      if (
        ! ::SetFilePointer(
          _f, static_cast<LONG>(in),
          0,
          ! seek ? FILE_BEGIN : FILE_CURRENT
        )
      )
        throw Error(error, string::Format(error_position).arg(_file_name));

#else // DJV_WINDOWS

      if (::lseek(_f, in, ! seek ? SEEK_SET : SEEK_CUR) == (off_t)-1)
        throw Error(error, string::Format(error_position).arg(_file_name));

#endif // DJV_WINDOWS
    }
    break;
  }
}

size_t File_Io::position() const
{
  size_t out = 0;
  
  switch (_mode)
  {
    case READ:
#if defined(DJV_MMAP)
      out = static_cast<size_t>(
        _mmap_p - reinterpret_cast<const uint8_t *>(_mmap_start));
      break;
#else
    // Fall through...
#endif

    case WRITE:
#if defined(DJV_WINDOWS)
      out = ::SetFilePointer(_f, 0, 0, FILE_CURRENT);
#else
      out = ::lseek(_f, 0, SEEK_CUR);
#endif
      break;
  }

  return out;
}

void File_Io::endian(bool in)
{
  _endian = in;
}

//------------------------------------------------------------------------------

}
}

