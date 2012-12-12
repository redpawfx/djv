/*------------------------------------------------------------------------------
 include/djv_base/string.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_STRING_H
#define DJV_STRING_H

#include <djv_base/string_def.h>

#include <djv_base/list.h>
#include <string>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::string
//
//! Strings.
//------------------------------------------------------------------------------

namespace string {

//@}
//------------------------------------------------------------------------------
//!\class String
//
//! Inherited from std::string.
//------------------------------------------------------------------------------

class _DJV_BASE String : public std::string
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  inline String();
  inline String(const String &, size_type position = 0, size_type size = npos);
  inline String(const std::string &);
  inline String(const char *);
  inline String(const char *, size_type size);
  inline String(char, size_type size = 1);
  
  //@}
};

//! Default C-string size.

static const String::size_type cstring_size = 256;

//------------------------------------------------------------------------------
//!\class Format
//
//! String formatting.
//!
//! Example:
//!
//! String s = string::Format("string = %%, int = %%, double = %%").
//!   arg("abc").
//!   arg(123).
//!   arg(456.0);
//------------------------------------------------------------------------------

class _DJV_BASE Format
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Format(const String &);

  //@}
  //----------------------------------------------------------------------------
  //!\name Arguments
  //@{
  
  //! String argument.
  
  Format arg(
    const String &,
    int width = 0,
    char fill = ' '
  ) const;
  
  //! String list argument.
  
  Format arg(
    const List<String> &,
    const String & separator = " ",
    int width = 0,
    char fill = ' '
  ) const;
  
  //! Integer arguments.
  
  Format arg(
    int,
    int width = 0,
    char fill = ' '
  ) const;
  
  Format arg(
    int64_t,
    int width = 0,
    char fill = ' '
  ) const;
  
  Format arg(
    uint,
    int width = 0,
    char fill = ' '
  ) const;
  
  Format arg(
    uint64_t,
    int width = 0,
    char fill = ' '
  ) const;
  
  //! Floating-point argument.
  
  Format arg(
    double,
    int width = 0,
    int precision = 0,
    char fill = ' '
  ) const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Conversion
  //@{
  
  operator String () const;
  
  //@}
  
private:

  String _value;
};

//------------------------------------------------------------------------------

}
}

#include <djv_base/string_fnc.h>
#include <djv_base/string_fnc_inline.h>
#include <djv_base/string_inline.h>
#endif

