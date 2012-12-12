/*------------------------------------------------------------------------------
 include/djv_base/directory_fnc.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace directory {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Get the user's home directory.

_DJV_BASE String home();

//! Get the user's desktop directory.

_DJV_BASE String desktop();

//! Get a list of drives (Windows specific).

_DJV_BASE List<String> drive_list();

//! Get the root directory.

_DJV_BASE String root();

//@}
//------------------------------------------------------------------------------

}
}

