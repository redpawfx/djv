/*------------------------------------------------------------------------------
 bin/djv_view/win_main.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "application.h"

//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
  int r = 1;
  
  try
  {
    r = djv_bin::view::application::Application(argc, argv).exec();
  }
  catch (djv_base::Error in)
  {
    djv_base::error::print(in);
  }
  
  return r;
}

//------------------------------------------------------------------------------
// WinMain()
//------------------------------------------------------------------------------

#if defined(DJV_WINDOWS)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
  return main(__argc, __argv);
}

#endif

//------------------------------------------------------------------------------

