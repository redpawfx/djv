/*------------------------------------------------------------------------------
 include/djv_base/error.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_ERROR_H
#define DJV_ERROR_H

#include <djv_base/error_def.h>

#include <djv_base/list.h>
#include <djv_base/string.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::error
//
//! Errors.
//------------------------------------------------------------------------------

namespace error {

//------------------------------------------------------------------------------
//!\class Error
//
//! Exception base class.
//------------------------------------------------------------------------------

class _DJV_BASE Error
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Error();
  Error(const String &);
  Error(const String & prefix, const String &);
  
  //@}
  
  //----------------------------------------------------------------------------
  //!\name Conversion
  //@{
  
  const String & string() const;

  operator String () const;

  //@}
  
private:

  String _value;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Convert an error into a printable format.

_DJV_BASE String format(const Error &);

//! Print an error to the console.

_DJV_BASE void print(const Error &);

//! Return the last system error (Windows specific).

_DJV_BASE String last_error();

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_BASE extern const String
  error_format;

//@}
//------------------------------------------------------------------------------

}
}

#endif

