/*------------------------------------------------------------------------------
 bin/djv_ls/application.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_LS_APPLICATION_H
#define DJV_LS_APPLICATION_H

#include <djv_image/application.h>
#include <djv_base/file.h>

namespace djv_bin {

//------------------------------------------------------------------------------
//!\namespace djv_bin::ls
//
//! Utility for listing directories.
//------------------------------------------------------------------------------

namespace ls {
using namespace djv_base;

//------------------------------------------------------------------------------
//!\class Application
//
//! The djv_ls application.
//------------------------------------------------------------------------------

class Application : public djv_image::application::Application_Base
{
public:

  Application(int, char **);
  
  ~Application();
  
private:

  void process(List<File> &);

  void print_file(const File &, bool path = false, bool info = true);
  
  bool print_directory(const File &, bool label);
  
  void command_line(String &) throw (Error);
  
  String command_line_help() const;

  List<String> _input;
  
  bool _info;
  bool _file_path;
  file_seq::COMPRESS _seq;
  bool _recurse;
  bool _hidden;
  List<String> _filter;
  int _columns;
  
  file::SORT _sort;
  bool _sort_reverse;
  bool _x_sort_directory;
};

//------------------------------------------------------------------------------

}
}

#endif

