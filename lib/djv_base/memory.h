/*------------------------------------------------------------------------------
 include/djv_base/memory.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_MEMORY_H
#define DJV_MEMORY_H

#include <djv_base/memory_def.h>

#include <djv_base/string.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::memory
//
//! Memory.
//------------------------------------------------------------------------------

namespace memory {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

static const uint64_t kilobyte = 1024;
static const uint64_t megabyte = kilobyte * 1024;
static const uint64_t gigabyte = megabyte * 1024;
static const uint64_t terabyte = gigabyte * 1024;

static const size_t align = 4096;

//! Machine endian

enum ENDIAN
{
  MSB,  //!< Most siginificant byte first
  LSB,  //!< Least significant byte first
  
  _ENDIAN_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Buffer
//
//! A memory buffer.
//------------------------------------------------------------------------------

template<typename T>
class Buffer
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline Buffer();
  inline Buffer(const Buffer &);
  inline Buffer(size_t);
  
  inline ~Buffer();
  
  inline Buffer & operator = (const Buffer &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Size
  //@{
  
  inline void size(size_t);
  
  inline size_t size() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Data
  //@{
  
  inline T * data();

  inline const T * data() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  inline void zero();

  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{
  
  inline T * operator () ();

  inline const T * operator () () const;
  
  //@}
  
private:

  inline void del();

  T * _data;
  size_t _size;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Allocate memory.

_DJV_BASE void * get(size_t);

//! De-allocate memory.

_DJV_BASE void del(void *);

//! Copy memory.

_DJV_BASE void copy(const void *, void *, size_t size);

//! Fill memory with a value.

template<typename T>
inline void set(T, void *, size_t size);

//! Fill memory with zeroes.

_DJV_BASE void zero(void *, size_t size);

//! Get the current machine's endian.

inline ENDIAN endian();

//! Get the opposite of the given endian.

inline ENDIAN endian_opposite(ENDIAN);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_BASE int compare(const void *, const void *, size_t size);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

inline void endian(
  void *,
  size_t size,
  size_t word_size
);

inline void endian(
  const void *,
  void *,
  size_t size,
  size_t word_size
);

_DJV_BASE String & operator >> (String &, ENDIAN &) throw (String);

_DJV_BASE String & operator << (String &, ENDIAN);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_BASE String label_size(uint64_t);

_DJV_BASE const List<String> & label_endian();
_DJV_BASE const List<String> & label_endian_text();

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_base/memory_inline.h>
#endif

