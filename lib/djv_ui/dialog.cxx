/*------------------------------------------------------------------------------
 lib/djv_ui/dialog.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "dialog.h"

#include <djv_ui/application.h>

namespace djv_ui {
namespace dialog {

//------------------------------------------------------------------------------
// Base
//------------------------------------------------------------------------------

Base::Base(const String & title)
{
  this->title(title);
}

Base::~Base()
{}

void Base::title(const String & in)
{
  // Add the application name to the dialog title.
  
  const String & application_name = application::global()->name();
  
  Window::title(
    ! application_name.empty() ?
    String(string::Format("%% - %%").arg(application_name).arg(in)) :
    in
  );
}

//------------------------------------------------------------------------------

}
}
