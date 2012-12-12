/*------------------------------------------------------------------------------
 lib/djv_image/io_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io.h"

#include <djv_base/file_io.h>

namespace djv_image {
namespace io {

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

void throw_error(const String & plugin, const String & error)
  throw (Error)
{
  throw Error(
    string::Format(io::error_plugin).arg(plugin),
    error
  );
}

void throw_error_unrecognized(const String & plugin, const String & file_name)
  throw (Error)
{
  throw Error(
    string::Format(io::error_plugin).arg(plugin),
    string::Format(io::error_unrecognized).arg(file_name)
  );
}

void throw_error_unsupported(const String & plugin, const String & file_name)
  throw (Error)
{
  throw Error(
    string::Format(io::error_plugin).arg(plugin),
    string::Format(io::error_unsupported).arg(file_name)
  );
}

void throw_error_open(const String & plugin, const String & file_name)
  throw (Error)
{
  throw Error(
    string::Format(io::error_plugin).arg(plugin),
    string::Format(io::error_open).arg(file_name)
  );
}

void throw_error_read(const String & plugin, const String & file_name)
  throw (Error)
{
  throw Error(
    string::Format(io::error_plugin).arg(plugin),
    string::Format(io::error_read).arg(file_name)
  );
}

void throw_error_write(const String & plugin, const String & file_name)
  throw (Error)
{
  throw Error(
    string::Format(io::error_plugin).arg(plugin),
    string::Format(io::error_write).arg(file_name)
  );
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Info & a, const Info & b)
{
  if (a.layer_size() != b.layer_size())
    return false;
  
  for (list::size_type i = 0; i < a.layer_size(); ++i)
    if (a[i] != b[i])
      return false;
  
  return
    data::operator == (a, b) &&
    a.tag == b.tag &&
    a.time == b.time;
}

bool operator != (const Info & a, const Info & b)
{
  return ! (a == b);
}

bool operator == (const Frame_Info & a, const Frame_Info & b)
{
  return
    a.frame == b.frame &&
    a.layer == b.layer &&
    a.proxy == b.proxy;
}

bool operator != (const Frame_Info & a, const Frame_Info & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

List<String> label_extension(const List<Base *> & in)
{
  List<String> out;
  
  for (list::size_type i = 0; i < in.size(); ++i)
    out += string::Format("%%").
      arg(in[i]->name()).
      arg(string::join(in[i]->extension_list(), ", "));
  
  return out;
}

extern const String
  error = "Image I/O",
  error_plugin = "Image I/O, %%",
  error_unrecognized = "Unrecognized file: %%",
  error_unsupported = "Unsupported file: %%",
  error_open = file_io::error_open,
  error_read = file_io::error_read,
  error_write = file_io::error_write;

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, const Info & in)
{
  // XXX
  
  return data::operator << (debug, in);
}

Debug & operator << (Debug & debug, const Frame_Info & in)
{
  // XXX
  
  return debug << in.frame;
}

//------------------------------------------------------------------------------

}
}

