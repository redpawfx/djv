/*------------------------------------------------------------------------------
 include/djv_base/system.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_SYSTEM_H
#define DJV_SYSTEM_H

#include <djv_base/base.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::system
//
//! System utilities.
//------------------------------------------------------------------------------

namespace system {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Get system information.

_DJV_BASE String info();

//! Get the search path. The default search path consists of:
//!
//! - The DJV_PATH environment variable
//! - The LD_LIBRARY_PATH environment variable
//! - The application path
//! - The current directory

_DJV_BASE List<String> search_path();

//! Get the width of the terminal.

_DJV_BASE int terminal_width();

//! Print a message to the terminal.

_DJV_BASE void print(const String &, bool newline = true);

//! Execute a command.

_DJV_BASE int exec(const String &);

//! Open an URL in an external web-browser.

_DJV_BASE bool open_url(const String &);

//@}
//------------------------------------------------------------------------------
//!\name Environment Variables
//------------------------------------------------------------------------------
//@{

//! Set an environment variable.

_DJV_BASE bool env(const String &, const String &);

//! Get an environment variable.

_DJV_BASE String env(const String &);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_BASE extern const String
  label_info;

//@}
//------------------------------------------------------------------------------

}
}

#endif

