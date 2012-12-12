/*------------------------------------------------------------------------------
 lib/djv_base/file_seq.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file.h"

#include <algorithm>

namespace djv_base {
namespace file_seq {

//------------------------------------------------------------------------------
// Seq
//------------------------------------------------------------------------------

Seq::Seq() :
  pad(0)
{}

Seq::Seq(const List<int64_t> & in, int pad) :
  list(in),
  pad(pad)
{}

Seq::Seq(int64_t start, int64_t end, int pad) :
  pad(pad)
{
  set(start, end);
}

void Seq::set(int64_t start, int64_t end)
{
  if (start < end)
  {
    const list::size_type size = math::min(end - start + 1, seq_max);
    
    list.resize(size);
    
    for (int64_t i = start, j = 0; i <= end, j < size; ++i, ++j)
      list[j] = i;
  }
  else
  {
    const list::size_type size = math::min(start - end + 1, seq_max);
    
    list.resize(size);
    
    for (int64_t i = start, j = 0; i >= end, j < size; --i, ++j)
      list[j] = i;
  }
}

//------------------------------------------------------------------------------
// sort()
//------------------------------------------------------------------------------

void sort(Seq & in)
{
  std::sort(in.list.begin(), in.list.end());
}

//------------------------------------------------------------------------------
// find_closest()
//------------------------------------------------------------------------------

int64_t find_closest(int64_t in, const List<int64_t> & list)
{
  const list::size_type size = list.size();
  if (! size)
    return -1;
  
  int64_t out = 0;
  
  int64_t min = 0;
  for (list::size_type i = 0; i < size; ++i)
  {
    const int64_t tmp = math::abs(in - list[i]);
    if (tmp < min || ! i)
    {
      out = static_cast<int64_t>(i);
      min = tmp;
    }
  }
  
  return out;
}

//------------------------------------------------------------------------------
// seq_to_string()
//------------------------------------------------------------------------------

namespace {

enum INC
{
  BREAK,
  ASCEND,
  DESCEND
};

inline INC inc(int64_t a, int64_t b)
{
  if (a + 1 == b) return ASCEND;
  else if (a - 1 == b) return DESCEND;
  return BREAK;
}

}

String seq_to_string(const Seq & in)
{
  //DJV_DEBUG("seq_to_string");
  //DJV_DEBUG_PRINT("in = " << in.list);
  
  List<String> out;
  
  List<int64_t> list = in.list;
  const list::size_type size = list.size();
  const int pad = in.pad;
  
  // List end marker.
  
  list += -2;

  // Sequence.
  
  int64_t tmp = list[0];
  INC _inc = BREAK;
  for (list::size_type i = 0; i < size; ++i)
  {
    const INC inc_ = inc(list[i], list[i + 1]);
    
    //DJV_DEBUG_PRINT(list[i] << " = " << inc_);

    if ((inc_ != _inc && _inc != BREAK) || BREAK == inc_)
    {
      if (tmp != list[i])
        out += frame_to_string(tmp, pad) + "-" + frame_to_string(list[i], pad);
      else
        out += frame_to_string(list[i], pad);
      tmp = list[i + 1];
      _inc = inc_;
    }
  }
  
  //DJV_DEBUG_PRINT("out = " << out);
  
  return string::join(out, ',');
}

//------------------------------------------------------------------------------
// string_to_seq()
//------------------------------------------------------------------------------

Seq string_to_seq(const String & in)
{
  //DJV_DEBUG("string_to_seq");
  //DJV_DEBUG_PRINT("in = " << in);

  Seq out;
  
  int pad = 0;
  
  const List<String> tmp = string::split(in, ',');
  
  for (list::size_type i = 0; i < tmp.size(); ++i)
  {
    const List<String> tmp2 = string::split(tmp[i], '-');
    const list::size_type size = tmp2.size();
    
    if (size)
    {
      int _pad = 0;
      const int64_t start = string_to_frame(tmp2[0], &_pad);
      const int64_t end = string_to_frame(tmp2[size - 1]);
      
      if (start != -1 && end != -1)
      {
        if (start < end)
        {
          const list::size_type size = math::min(end - start + 1, seq_max);
    
          for (int64_t j = start, k = 0; j <= end && k < size; ++j, ++k)
            out.list += j;
        }
        else
        {
          const list::size_type size = math::min(start - end + 1, seq_max);
    
          for (int64_t j = start, k = 0; j >= end && k < size; --j, ++k)
            out.list += j;
        }
      }
      
      pad = math::max(_pad, pad);
    }
  }

  out.pad = pad;
  
  //DJV_DEBUG_PRINT("out = " << out);
  //DJV_DEBUG_PRINT("pad = " << out.pad);
  
  return out;
}

//------------------------------------------------------------------------------
// frame_to_string()
//------------------------------------------------------------------------------

String frame_to_string(int64_t in, int pad)
{
  char format [] = "%00d";
  static const int format_max = 9;
  format[2] = char('0' + math::min(pad, format_max));
  
  char tmp [string::cstring_size] = "";
  const int size = SNPRINTF(tmp, string::cstring_size, format, in);
  return String(tmp, size);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

String & operator << (String & out, const Seq & in)
{
  return out << file_seq::seq_to_string(in);
}

String & operator >> (String & in, Seq & out) throw (String)
{
  String tmp;
  in >> tmp;
  out = string_to_seq(tmp);
  return in;
}

_DJV_STRING_OPERATOR_LABEL(COMPRESS, label_compress())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_compress()
{
  static const List<String> data = List<String>() <<
    "Off" <<
    "Sparse" <<
    "Range";
  
  DJV_ASSERT(data.size() == _COMPRESS_SIZE);
  
  return data;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, const Seq & in)
{
  return debug << string::label(in);
}

//------------------------------------------------------------------------------

}
}

