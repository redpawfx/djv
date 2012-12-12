/*------------------------------------------------------------------------------
 lib/djv_image/io_ifl_load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_ifl.h"

#include <djv_base/file.h>
#include <djv_base/file_io.h>

namespace djv_image {
namespace io_ifl {

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return io_ifl::name;
}

List<String> Load::extension_list() const
{
  return io_ifl::extension_list;
}

//------------------------------------------------------------------------------
// Load::open()
//------------------------------------------------------------------------------

void Load::open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::open");
  DJV_DEBUG_PRINT("in = " << in);
  
  _list.clear();

  List<String> tmp;
  try
  {
    tmp = file_io::lines(in);
    DJV_DEBUG_PRINT("list = " << tmp);
  }
  catch (Error error)
  {
    io::throw_error_read(name(), in);
  }
  
  for (list::size_type i = 0; i < tmp.size(); ++i)
  {
    if (tmp[i].empty())
      continue;
    
    File file(tmp[i]);
    
    if (file.path().empty())
      file.path(in.path());
    
    if (file.is_seq_valid())
    {
      file.type(file::SEQ);
      
      _list += file::seq_expand(file);
    }
    else
    {
      _list += tmp[i];
    }
  }
  
  DJV_DEBUG_PRINT("list = " << _list);

  _load = std::auto_ptr<io::Load>(
    io::global()->load(_list.size() ? _list[0] : String(), &info)
  );
  
  info.time.list.resize(_list.size());
  for (list::size_type i = 0; i < _list.size(); ++i)
    info.time.list[i] = static_cast<int>(i);
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

const Image * Load::get(const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Load::get");
  DJV_DEBUG_PRINT("frame = " << frame);
  
  String file_name;
  
  if (_list.size())
  {
    if (-1 == frame.frame)
      file_name = _list[0];
    else if (frame.frame >= 0 && frame.frame < static_cast<int>(_list.size()))
      file_name = _list[frame.frame];
  }
  
  DJV_DEBUG_PRINT("file name = " << file_name);

  io::Info info;
  
  _load = std::auto_ptr<io::Load>(io::global()->load(file_name, &info));
  
  return _load->get(io::Frame_Info(-1, frame.layer, frame.proxy));
}

//------------------------------------------------------------------------------

}
}
