/*------------------------------------------------------------------------------
 include/djv_base/file_seq_inline.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace file_seq {

//------------------------------------------------------------------------------
// Seq
//------------------------------------------------------------------------------

inline int64_t Seq::start() const
{
  return list.size() ? list[0] : 0;
}

inline int64_t Seq::end() const
{
  return list.size() ? list[list.size() - 1] : 0;
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

inline bool operator == (const Seq & a, const Seq & b)
{
  return a.list == b.list && a.pad == b.pad;
}

inline bool operator != (const Seq & a, const Seq & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------

}
}

