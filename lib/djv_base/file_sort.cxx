/*------------------------------------------------------------------------------
 lib/djv_base/file_sort.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file.h"

#include <algorithm>

namespace djv_base {
namespace file {

//------------------------------------------------------------------------------
// sort()
//------------------------------------------------------------------------------

namespace {

typedef bool (Compare)(const File &, const File &);

inline bool name(const File & a, const File & b) { return a.get() < b.get(); }
inline bool type(const File & a, const File & b) { return a.type() < b.type(); }
inline bool size(const File & a, const File & b) { return a.size() < b.size(); }
inline bool user(const File & a, const File & b) { return a.user() < b.user(); }
inline bool perm(const File & a, const File & b) { return a.perm() < b.perm(); }
inline bool time(const File & a, const File & b) { return a.time() < b.time(); }

inline bool name_r(const File & a, const File & b) { return name(b, a); }
inline bool type_r(const File & a, const File & b) { return type(b, a); }
inline bool size_r(const File & a, const File & b) { return size(b, a); }
inline bool user_r(const File & a, const File & b) { return user(b, a); }
inline bool perm_r(const File & a, const File & b) { return perm(b, a); }
inline bool time_r(const File & a, const File & b) { return time(b, a); }

}

void sort(
  List<File> & in,
  SORT sort,
  bool reverse
) {
  DJV_DEBUG("sort");
  DJV_DEBUG_PRINT("in = " << in.size());
  DJV_DEBUG_PRINT("sort = " << sort);
  DJV_DEBUG_PRINT("reverse = " << reverse);
  
  Compare * compare = 0;
  switch (sort)
  {
    case SORT_NAME: compare = reverse ? name_r : name; break;
    case SORT_TYPE: compare = reverse ? type_r : type; break;
    case SORT_SIZE: compare = reverse ? size_r : size; break;
    case SORT_USER: compare = reverse ? user_r : user; break;
    case SORT_PERM: compare = reverse ? perm_r : perm; break;
    case SORT_TIME: compare = reverse ? time_r : time; break;
  }
  
  std::sort(in.begin(), in.end(), compare);
}

//------------------------------------------------------------------------------
// sort_directories_first()
//------------------------------------------------------------------------------

namespace {

inline bool directories_first(const File & a, const File & b)
{
  return DIRECTORY == a.type() && DIRECTORY != b.type();
}

}

void sort_directories_first(List<File> & in)
{
  DJV_DEBUG("sort_directories_first");
  
  std::stable_sort(in.begin(), in.end(), directories_first);
}

//------------------------------------------------------------------------------

}
}

