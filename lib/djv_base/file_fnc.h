/*------------------------------------------------------------------------------
 include/djv_base/file_fnc.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace file {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Split a file name into pieces.

_DJV_BASE void split(
  const String &,
  String & path,
  String & base,
  String & number,
  String & extension
);

//! Check if a file exists.

_DJV_BASE bool exists(const File &);

//! Sort frame numbers in a sequence.

_DJV_BASE void seq_sort(File &);

//! Find a match for a sequence wildcard. If nothing is found the input is
//! returned.
//!
//! Example wildcard: render.#.exr

_DJV_BASE const File & seq_wildcard_match(const File &, const List<File> &);

//! Compress a list of files into a sequence.

_DJV_BASE void seq_compress(
  List<File> &,
  file_seq::COMPRESS = file_seq::COMPRESS_RANGE
);

//! Expand a sequence into a list of file names.

_DJV_BASE List<String> seq_expand(const File &);

//@}
//------------------------------------------------------------------------------
//!\name Filtering
//------------------------------------------------------------------------------
//@{

//! File filtering.

enum FILTER
{
  FILTER_NONE = 0,
  FILTER_FILE = 1,       //!< Filter files
  FILTER_DIRECTORY = 2,  //!< Filter directories
  FILTER_HIDDEN = 4      //!< Filter hidden items
};

//! Filter a list of files.

_DJV_BASE void filter(
  List<File> &,
  int filter,
  const List<String> & glob = List<String>(),
  bool glob_files_only = false
);

//@}
//------------------------------------------------------------------------------
//!\name Sorting
//------------------------------------------------------------------------------
//@{

//! File sorting.

enum SORT
{
  SORT_NAME,  //!< Sort by name
  SORT_TYPE,  //!< Sort by file type
  SORT_SIZE,  //!< Sort by file size
  SORT_USER,  //!< Sort by user name
  SORT_PERM,  //!< Sort by file permissions
  SORT_TIME,  //!< Sort by the last modified time
  
  _SORT_SIZE
};

//! Sort a list of files.

_DJV_BASE void sort(
  List<File> &,
  SORT,
  bool reverse = false
);

//! Sort list so directories are first.

_DJV_BASE void sort_directories_first(
  List<File> &
);

//@}
//------------------------------------------------------------------------------
//!\name File Paths
//------------------------------------------------------------------------------
//@{

//! Convert a path to an absolute path.

_DJV_BASE String path_absolute(const String &);

//! Is a path absolute?

_DJV_BASE bool path_is_absolute(const String &);

//! Fix a path.

_DJV_BASE String path_fix(const String &);

//_DJV_BASE String path_posix(const String &);

//! Path separator.

static List<char> path_separator_list = List<char>() << '/' << '\\';
#if defined(DJV_WINDOWS)
static char & path_separator = path_separator_list[1];
#else
static char & path_separator = path_separator_list[0];
#endif

//! Path list separator.

static List<char> list_separator_list = List<char>() << ':' << ';';
#if defined(DJV_WINDOWS)
static char & list_separator = list_separator_list[1];
#else
static char & list_separator = list_separator_list[0];
#endif

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_BASE String & operator >> (String &, SORT &) throw (String);

_DJV_BASE String & operator << (String &, SORT);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_BASE const List<String> & label_sort();

//@}
//------------------------------------------------------------------------------

}
}

