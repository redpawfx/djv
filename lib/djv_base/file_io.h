/*------------------------------------------------------------------------------
 include/djv_base/file_io.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_FILE_IO_H
#define DJV_FILE_IO_H

#include <djv_base/file_io_def.h>

#include <djv_base/base.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::file_io
//
//! File I/O.
//------------------------------------------------------------------------------

namespace file_io {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

enum MODE
{
  READ,
  WRITE
};

//@}
//------------------------------------------------------------------------------
//!\class File_Io
//
//! Read and write files.
//------------------------------------------------------------------------------

class _DJV_BASE File_Io
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  File_Io();
  
  ~File_Io();

  //@}
  //----------------------------------------------------------------------------
  //!\name File
  //@{
  
  void open(const String & file_name, MODE) throw (Error);
  
  void close();
  
  const String & file_name() const;
  
  bool is_valid() const;

  inline size_t size() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Write
  //@{

  void set(const void *, size_t, size_t word_size = 1) throw (Error);

  void set_8(const int8_t *, size_t) throw (Error);
  void set_u8(const uint8_t *, size_t) throw (Error);
  void set_16(const int16_t *, size_t) throw (Error);
  void set_u16(const uint16_t *, size_t) throw (Error);
  void set_32(const int32_t *, size_t) throw (Error);
  void set_u32(const uint32_t *, size_t) throw (Error);
  void set_f32(const float *, size_t) throw (Error);

  inline void set_8(const int8_t & in) throw (Error);
  inline void set_u8(const uint8_t & in) throw (Error);
  inline void set_16(const int16_t & in) throw (Error);
  inline void set_u16(const uint16_t & in) throw (Error);
  inline void set_32(const int32_t & in) throw (Error);
  inline void set_u32(const uint32_t & in) throw (Error);
  inline void set_f32(const float & in) throw (Error);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Read
  //@{

  void get(void *, size_t, size_t word_size = 1) throw (Error);
  
  void get_8(int8_t *, size_t = 1) throw (Error);
  void get_u8(uint8_t *, size_t = 1) throw (Error);
  void get_16(int16_t *, size_t = 1) throw (Error);
  void get_u16(uint16_t *, size_t = 1) throw (Error);
  void get_32(int32_t *, size_t = 1) throw (Error);
  void get_u32(uint32_t *, size_t = 1) throw (Error);
  void get_f32(float *, size_t = 1) throw (Error);

  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  //! Start an asynchronous read-ahead. This allows the operating system to
  //! cache the file by the time we need it.
  
  void read_ahead();

  //@}
  //----------------------------------------------------------------------------
  //!\name Memory-Mapping
  //!
  //! XXX Ill but fast.
  //@{
  
  //! Get the current memory-map position.
  
  inline const uint8_t * mmap_p() const;
  
  //! Get a pointer to the end of the memory-map.
  
  inline const uint8_t * mmap_end() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name File Position
  //@{

  void position(size_t) throw (Error);
  
  size_t position() const;

  void seek(size_t) throw (Error);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Endian Conversion
  //!
  //! Perform endian conversion when using the data functions.
  //@{

  void endian(bool);
  
  inline bool endian() const;

  //@}
  
private:

  void position(size_t, bool seek) throw (Error);

#if defined(DJV_WINDOWS)
  HANDLE _f;
#else
  int _f;
#endif
  String _file_name;
  MODE _mode;
  size_t _size;
  bool _endian;
  void * _mmap;
  const uint8_t * _mmap_start, * _mmap_end, * _mmap_p;
  int _mmap_read_ahead;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Read a word from a file.

_DJV_BASE void word(
  File_Io &,
  char *,
  int max_len = string::cstring_size
) throw (Error);

//! Read a line from a file.

_DJV_BASE void line(
  File_Io &,
  char *,
  int max_len = string::cstring_size
) throw (Error);

//! Read all the lines from a file.

_DJV_BASE List<String> lines(const String & file_name) throw (Error);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_BASE extern const String
  error,
  error_open,
  error_mmap,
  error_read,
  error_write,
  error_position;

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_base/file_io_inline.h>
#endif

