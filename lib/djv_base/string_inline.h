/*------------------------------------------------------------------------------
 include/djv_base/string_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace string {

//------------------------------------------------------------------------------
// String
//------------------------------------------------------------------------------

inline String::String()
{}

inline String::String(const String & in, size_type position, size_type size) :
  std::string(in, position, size)
{}

inline String::String(const std::string & in) :
  std::string(in)
{}

inline String::String(const char * in) :
  std::string(in)
{}

inline String::String(const char * in, size_type size) :
  std::string(in, size)
{}

inline String::String(char in, size_type size) :
  std::string(size, in)
{}

//------------------------------------------------------------------------------

}
}

