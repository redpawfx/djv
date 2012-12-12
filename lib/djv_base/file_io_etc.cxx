/*------------------------------------------------------------------------------
 lib/djv_base/file_io_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_io.h"

namespace djv_base {
namespace file_io {

//------------------------------------------------------------------------------
// word()
//------------------------------------------------------------------------------

void word(File_Io & in, char * out, int max_len) throw (Error)
{
  DJV_ASSERT(max_len > 0);
  
  DJV_DEBUG("word");
  
  out[0] = 0;
  
  enum
  {
    END,
    WORD,
    COMMENT
  } parse = WORD;

  int i = 0;
  
  while (parse)
  {
    DJV_DEBUG_PRINT("parse = " << parse);
    DJV_DEBUG_PRINT("i = " << i);

    // Get next character.

    uint8_t c;
    in.get(&c, 1);
    
    DJV_DEBUG_PRINT("c = \'" << c << "\'");
    
    switch (c)
    {
      // Start comment.

      case '#': parse = COMMENT; break;

      // End comment or word.

      case '\0':
      case '\n':
      case '\r': parse = WORD;
      case ' ':
      case '\t': if (out[0]) parse = END; break;

      // Add character to word.

      default:
        if (WORD == parse && i < (max_len - 1))
        {
          out[i++] = c;
          //out[i] = 0;
          //DJV_DEBUG_PRINT("out = \"" << out << "\"");
        }
        break;
    }
  }
  
  out[i] = 0;
  
  DJV_DEBUG_PRINT("out = \"" << out << "\"");
}

//------------------------------------------------------------------------------
// line()
//------------------------------------------------------------------------------

void line(File_Io & in, char * out, int max_len) throw (Error)
{
  DJV_ASSERT(max_len > 0);
  
  DJV_DEBUG("line");
  
  int i = 0;
  char c = 0;
  
  do
  {
    in.get(&c, 1);

    if (
      c != '\n' &&
      c != '\r'
    ) {
      out[i++] = c;
      DJV_DEBUG_PRINT(String(out, i));
    }
  }
  while (
    c != '\n' &&
    c != '\r' &&
    in.is_valid() &&
    i < (max_len - 1)
  );

  out[i] = 0;
  
  DJV_DEBUG_PRINT("out = \"" << out << "\"");
}

//------------------------------------------------------------------------------
// lines()
//------------------------------------------------------------------------------

List<String> lines(const String & file_name) throw (Error)
{
  List<String> out;

  File_Io io;
  io.open(file_name, READ);
  
  while (io.is_valid())
  {
    char buf [string::cstring_size] = "";
    line(io, buf, string::cstring_size);
    out += buf;
  }

  return out;
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  error = "I/O",
  error_open = "Cannot open file: %%",
  error_mmap = "Cannot memory-map file: %%",
  error_read = "Error reading file: %%",
  error_write = "Error writing file: %%",
  error_position = "Cannot set file position: %%";

//------------------------------------------------------------------------------

}
}

