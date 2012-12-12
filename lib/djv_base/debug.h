/*------------------------------------------------------------------------------
 include/djv_base/debug.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_DEBUG_H
#define DJV_DEBUG_H

#include <djv_base/debug_def.h>

#include <djv_base/list.h>
#include <djv_base/set.h>
#include <djv_base/string.h>
#include <djv_base/time.h>

namespace djv_base {
namespace debug {

//------------------------------------------------------------------------------
// Assert
//------------------------------------------------------------------------------

#if defined(DJV_ASSERT)

#undef DJV_ASSERT

#define DJV_ASSERT(IN) \
  \
  if (! (IN)) \
    debug::_djv_assert(__FILE__, __LINE__)

#else

#define DJV_ASSERT(IN)

#endif

_DJV_BASE void _djv_assert(const char * file, int line);

//------------------------------------------------------------------------------
// Debugging Messages
//------------------------------------------------------------------------------

enum LINE
{
  LINE_BEGIN,
  LINE_END
};

class _DJV_BASE Debug
{
public:

  Debug(const String &);
  Debug(const String & prefix, const String &);
  
  ~Debug();

  void add(const String &);

  Debug & operator << (LINE);

private:

  void init(const String &);
  
  String _tmp;
  time::Timer _timer;
};

_DJV_BASE Debug & operator << (Debug &, const String &);
_DJV_BASE Debug & operator << (Debug &, const char *);
_DJV_BASE Debug & operator << (Debug &, bool);
_DJV_BASE Debug & operator << (Debug &, int);
_DJV_BASE Debug & operator << (Debug &, int64_t);
_DJV_BASE Debug & operator << (Debug &, uint);
_DJV_BASE Debug & operator << (Debug &, uint64_t);
_DJV_BASE Debug & operator << (Debug &, double);
//_DJV_BASE Debug & operator << (Debug &, list::size_type);
_DJV_BASE Debug & operator << (Debug &, const List<String> &);
_DJV_BASE Debug & operator << (Debug &, const Set<String> &);
template<class T>
inline Debug & operator << (Debug &, const List<T> &);
template<class T>
inline Debug & operator << (Debug &, const Set<T> &);

#if defined(DJV_DEBUG)

#undef DJV_DEBUG

#define DJV_DEBUG(in) \
  \
  djv_base::debug::Debug _debug(__FILE__, in)

#define DJV_DEBUG_PRINT(in) \
  \
  _debug << djv_base::debug::LINE_BEGIN << in << djv_base::debug::LINE_END

#else

#define DJV_DEBUG(in)
#define DJV_DEBUG_PRINT(in)

#endif

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_BASE String bits_u8(uint8_t);
_DJV_BASE String bits_u16(uint16_t);
_DJV_BASE String bits_u32(uint32_t);

//------------------------------------------------------------------------------

}
}

#include <djv_base/debug_inline.h>
#endif

