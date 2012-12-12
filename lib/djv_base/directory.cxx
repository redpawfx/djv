/*------------------------------------------------------------------------------
 lib/djv_base/directory.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "directory.h"

#include <djv_base/file.h>
#include <FL/filename.H>
#include <stdlib.h>

namespace djv_base {
namespace directory {

//------------------------------------------------------------------------------
// Directory
//------------------------------------------------------------------------------

Directory::Directory() :
  _is_valid(false)
{}

Directory::Directory(const String & in) :
  _is_valid(false)
{
  set(in);
}

void Directory::set(const String & in)
{
  DJV_DEBUG("Directory::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  _path = file::path_fix(in);
  
  DJV_DEBUG_PRINT("path = " << _path);
  
  update();
}

const String & Directory::get() const
{
  return _path;
}

List<File> Directory::list() const
{
  DJV_DEBUG("Directory::list");
  DJV_DEBUG_PRINT("path = " << _path);

  List<File> out;
  
  // Read directory entries.

  struct Dirent_List
  {
    Dirent_List() :
      p(0),
      size(0)
    {}
    
    ~Dirent_List()
    {
      if (p)
      {
        for (int i = 0; i < size; ++i)
          free((void *)p[i]);
        free((void *)p);
      }
    }
    
    dirent ** p;
    int size;
  };
  
  Dirent_List list;
  list.size = fl_filename_list(_path.c_str(), &list.p, fl_alphasort);
  DJV_DEBUG_PRINT("size = " << list.size);
  if (-1 == list.size)
    return out;
  
  // Convert to our format.
  
  out.resize(list.size);

  int j = 0;
  for (int i = 0; i < list.size; ++i)
  {
    const char * name = list.p[i]->d_name;
    
    // Go to the end of the string.
    
    const char * p = name;
    for (; *p; ++p)
      ;
    
    // Move back one if there is a trailing path separator.
    
    if (p - name)
      if ('/' == *(p - 1) || '\\' == *(p - 1))
        --p;
    
    // Ignore the "." and ".." directories.
    
    if (1 == (p - name))
    {
      if ('.' == name[0])
        name = 0;
    }
    else if (2 == (p - name))
    {
      if ('.' == name[0] && '.' == name[1])
        name = 0;
    }
    
    // Convert.
    
    if (name)
    {
      DJV_DEBUG_PRINT("file = " << name);
      
      out[j++] = File(_path + String(name, p - name));
    }
  }
  
  if (j < list.size)
    out.resize(j);

  DJV_DEBUG_PRINT("size = " << out.size());
  
  return out;
}

void Directory::cd(const String & in)
{
  DJV_DEBUG("Directory::cd");
  DJV_DEBUG_PRINT("in = " << in);

  _path = file::path_fix(_path + in);
  
  update();
}

void Directory::up()
{
  cd("..");
}

bool Directory::is_valid() const
{
  return _is_valid;
}

void Directory::update()
{
  dirent ** dirent_list = 0;

  const int size = fl_filename_list(get().c_str(), &dirent_list, fl_alphasort);
  
  _is_valid = size >= 0;

  for (int i = 0; i < size; ++i)
    ::free((void *)dirent_list[i]);
  if (dirent_list)
    ::free((void *)dirent_list);
}

//------------------------------------------------------------------------------

}
}

