/*------------------------------------------------------------------------------
 bin/djv_info/application.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_INFO_APPLICATION_H
#define DJV_INFO_APPLICATION_H

#include <djv_image/application.h>
#include <djv_base/file.h>

namespace djv_bin {

//------------------------------------------------------------------------------
//!\namespace djv_bin::info
//
//! Utility for displaying image information.
//------------------------------------------------------------------------------

namespace info {
using namespace djv_base;

//------------------------------------------------------------------------------
//!\class Application
//
//! The djv_info application.
//------------------------------------------------------------------------------

class Application : public djv_image::application::Application_Base
{
public:

  Application(int, char **);
  
private:

  void print_file(const File &, bool path = false, bool info = true)
    throw (Error);
  
  void print_directory(const File &, bool label) throw (Error);

  void command_line(String &) throw (Error);
  
  String command_line_help() const;

  List<String> _input;
  
  bool _info;
  bool _verbose;
  bool _file_path;
  file_seq::COMPRESS _seq;
  bool _recurse;
  int _columns;
};

//------------------------------------------------------------------------------

}
}

#endif

