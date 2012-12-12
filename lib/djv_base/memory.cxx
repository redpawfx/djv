/*------------------------------------------------------------------------------
 lib/djv_base/memory.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "memory.h"

#include <string.h>
#if ! (defined(DJV_FREEBSD) || defined(DJV_MACOSX))
#include <malloc.h>
#include <stdlib.h>
#endif

namespace djv_base {
namespace memory {

//------------------------------------------------------------------------------
// get()
//------------------------------------------------------------------------------

void * get(size_t in)
{
  // XXX

#if ! (defined(DJV_WINDOWS) || defined(DJV_FREEBSD) || defined(DJV_MACOSX))
  return ::memalign(memory::align, in);
#else
  return ::malloc(in);
#endif
}

//------------------------------------------------------------------------------
// del()
//------------------------------------------------------------------------------

void del(void * in)
{
  ::free(in);
}

//------------------------------------------------------------------------------
// copy()
//------------------------------------------------------------------------------

void copy(const void * in, void * out, size_t size)
{
  ::memcpy(out, in, size);
}

//------------------------------------------------------------------------------
// zero()
//------------------------------------------------------------------------------

void zero(void * out, size_t size)
{
  ::memset(out, 0, size);
}

//------------------------------------------------------------------------------
// compare()
//------------------------------------------------------------------------------

int compare(const void * a, const void * b, size_t size)
{
  return ::memcmp(a, b, size);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(ENDIAN, label_endian())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_endian()
{
  static const List<String> data = List<String>() <<
    "MSB" <<
    "LSB";
  
  // XXX Pre-processor clash with DJV_ASSERT? (Ubuntu 8.04)
  //
  //DJV_ASSERT(data.size() == _ENDIAN_SIZE);
  
  return data;
}

const List<String> & label_endian_text()
{
  static const List<String> data = List<String>() <<
    "Most significant bit first (e.g., MIPS, SPARC, PowerPC)" <<
    "Least significant bit first (e.g., Intel)";
  
  //DJV_ASSERT(data.size() == _ENDIAN_SIZE);
  
  return data;
}

//------------------------------------------------------------------------------

}
}

