/*------------------------------------------------------------------------------
 include/djv_base/file_io_inline.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace file_io {

//------------------------------------------------------------------------------
// File_Io
//------------------------------------------------------------------------------

inline size_t File_Io::size() const
{
  return _size;
}

inline void File_Io::set_8(const int8_t & in) throw (Error)
{
  set_8(&in, 1);
}

inline void File_Io::set_u8(const uint8_t & in) throw (Error)
{
  set_u8(&in, 1);
}

inline void File_Io::set_16(const int16_t & in) throw (Error)
{
  set_16(&in, 1);
}

inline void File_Io::set_u16(const uint16_t & in) throw (Error)
{
  set_u16(&in, 1);
}

inline void File_Io::set_32(const int32_t & in) throw (Error)
{
  set_32(&in, 1);
}

inline void File_Io::set_u32(const uint32_t & in) throw (Error)
{
  set_u32(&in, 1);
}

inline void File_Io::set_f32(const float & in) throw (Error)
{
  set_f32(&in, 1);
}

inline const uint8_t * File_Io::mmap_p() const
{
  return _mmap_p;
}

inline const uint8_t * File_Io::mmap_end() const
{
  return _mmap_end;
}

inline bool File_Io::endian() const
{
  return _endian;
}

//------------------------------------------------------------------------------

}
}

