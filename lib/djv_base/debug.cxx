/*------------------------------------------------------------------------------
 lib/djv_base/debug.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "debug.h"

#include <djv_base/string.h>
#include <stdio.h>
#include <stdlib.h>

namespace djv_base {
namespace debug {

//------------------------------------------------------------------------------
// _djv_assert()
//------------------------------------------------------------------------------

void _djv_assert(const char * file, int line)
{
  ::printf("ASSERT: File \"%s\", line %d.\n", file, line);
  ::fflush(stdout);

  ::abort();
}

//------------------------------------------------------------------------------
// Debug
//------------------------------------------------------------------------------

namespace {

// XXX Global variable.

int indent = 0;

}

void Debug::init(const String & in)
{
  *this << LINE_BEGIN << in << LINE_END;
  *this << LINE_BEGIN << "{" << LINE_END;

  indent += 2;
}

Debug::Debug(const String & in)
{
  init(in);
}

Debug::Debug(const String & prefix, const String & in)
{
  init(prefix + ": " + in);
}

Debug::~Debug()
{
  _timer.check();

  indent -= 2;
  
  *this <<
    LINE_BEGIN <<
    "} (" <<
    _timer.seconds() <<
    " seconds, " <<
    _timer.fps() <<
    " fps)" <<
    LINE_END;
}

Debug & Debug::operator << (LINE in)
{
  switch (in)
  {
    case LINE_BEGIN:
      _tmp = "debug " + String(' ', indent);
      break;

    case LINE_END:
      ::printf("%s\n", _tmp.c_str());
      ::fflush(stdout);
      break;
  }
  
  return *this;
}

void Debug::add(const String & in)
{
  _tmp += in;
}

Debug & operator << (Debug & debug, const String & in)
{
  debug.add(in);
  
  return debug;
}

Debug & operator << (Debug & debug, const char * in)
{
  debug.add(in);
  
  return debug;
}

Debug & operator << (Debug & debug, bool in)
{
  static const List<String> label = string::label_bool();
  
  return debug << label[in];
}

Debug & operator << (Debug & debug, int in)
{
  return debug << string::int_to_string(in);
}

Debug & operator << (Debug & debug, int64_t in)
{
  return debug << string::int_to_string(in);
}

Debug & operator << (Debug & debug, uint in)
{
  return debug << string::uint_to_string(in);
}

Debug & operator << (Debug & debug, uint64_t in)
{
  return debug << string::uint_to_string(in);
}

Debug & operator << (Debug & debug, double in)
{
  return debug << string::float_to_string(in);
}

/*Debug & operator << (Debug & debug, list::size_type in)
{
  return debug << string::uint_to_string(in);
}*/

Debug & operator << (Debug & debug, const List<String> & in)
{
  return debug << string::join(in, " ");
}

Debug & operator << (Debug & debug, const Set<String> & in)
{
  return debug << string::join(in, " ");
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

namespace {

String bits(uint32_t in, int size)
{
  String out;
  for (int i = 0; i < size; ++i)
    out += ((in >> i) & 1) ? '1' : '0';
  return out;
}

}

String bits_u8(uint8_t  in)
{
  return bits(in, 8); 
}

String bits_u16(uint16_t in)
{
  return bits(in, 16);
}

String bits_u32(uint32_t in)
{
  return bits(in, 32);
}

//------------------------------------------------------------------------------

}
}

