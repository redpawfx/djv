/*------------------------------------------------------------------------------
 lib/djv_base/file_fnc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file.h"

#include <djv_base/file_io.h>
#include <djv_base/math.h>

namespace djv_base {
namespace file {

//------------------------------------------------------------------------------
// exists()
//------------------------------------------------------------------------------

bool exists(const File & in)
{
  DJV_DEBUG("exists");
  DJV_DEBUG_PRINT("in = " << in);
  
  List<String> list;
  if (SEQ == in.type())
    list = seq_expand(in);
  else
    list += in;
  
  DJV_DEBUG_PRINT("list = " << list);
  
  for (list::size_type i = 0; i < list.size(); ++i)
    try
    {
      File_Io().open(list[i], file_io::READ);
      
      return true;
    }
    catch (Error)
    {}
  
  return false;;
}

//------------------------------------------------------------------------------
// seq_sort()
//------------------------------------------------------------------------------

void seq_sort(File & in)
{
  file_seq::sort(in._seq);
}

//------------------------------------------------------------------------------
// seq_wildcard_match()
//------------------------------------------------------------------------------

const File & seq_wildcard_match(const File & in, const List<File> & list)
{
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    const File & tmp = list[i];
    if (
      in.base() == tmp.base() &&
      in.extension() == tmp.extension() //&&
      //in.seq().pad == tmp.seq().pad
    )
      return tmp;
  }
  return in;
}

//------------------------------------------------------------------------------
// seq_compress()
//------------------------------------------------------------------------------

void seq_compress(List<File> & in, file_seq::COMPRESS compress)
{
  if (file_seq::COMPRESS_OFF == compress)
    return;

  DJV_DEBUG("seq_compress");
  DJV_DEBUG_PRINT("size = " << in.size());
  
  list::size_type cache = 0;
  bool cache_valid = false;
  list::size_type i = 0;
  
  for (list::size_type j = 0; j < in.size(); ++j)
  {
    DJV_DEBUG_PRINT("in = " << in[j]);
    DJV_DEBUG_PRINT("in seq = " << in[j].seq());
    
    const bool seq = in[j].is_seq_valid();
    
    DJV_DEBUG_PRINT("seq = " << seq);
    
    if (seq)
    {
      if (cache_valid)
        if (in[cache].seq_add(in[j]))
          continue;
      
      cache_valid = false;
      for (list::size_type k = 0; k < i; ++k)
      {
        if (SEQ == in[k].type())
          if (in[k].seq_add(in[j]))
          {
            cache = k;
            cache_valid = true;
            continue;
          }
      }
    }
    
    if (! seq || ! cache_valid)
    {
      in[i] = in[j];
      if (seq)
        in[i].type(SEQ);
      ++i;
    }
  }
  
  DJV_DEBUG_PRINT("size = " << i);
  
  in.resize(i);
  
  for (list::size_type i = 0; i < in.size(); ++i)
    sort(in[i]._seq);
  
  if (file_seq::COMPRESS_RANGE == compress)
    for (list::size_type i = 0; i < in.size(); ++i)
      if (in[i]._seq.list.size())
        in[i]._seq.set(in[i]._seq.start(), in[i]._seq.end());
}

//------------------------------------------------------------------------------
// seq_expand()
//------------------------------------------------------------------------------

List<String> seq_expand(const File & in)
{
  List<String> out;

  const file_seq::Seq & seq = in.seq();
  const list::size_type size = seq.list.size();
  
  if (SEQ == in.type() && size)
  {
    out.resize(size);
    
    for (list::size_type i = 0; i < size; ++i)
      out[i] = in.get(seq.list[i]);
  }
  else
  {
    out += in;
  }
  
  return out;
}

//------------------------------------------------------------------------------

}
}

