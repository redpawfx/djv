//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_file.h

#ifndef DJV_FILE_H
#define DJV_FILE_H

#include <djv_debug.h>
#include <djv_seq.h>
#include <djv_set.h>

namespace djv
{

struct File_Util;

//------------------------------------------------------------------------------
//! \class File
//!
//! This class provides information about files.
//!
//! Example: /tmp/render.1-100.exr
//!
//! - path      - /tmp/
//! - base      - render.
//! - number    - 1-100
//! - extension - .exr
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT File
{
public:

    //! Constructor.

    File();

    //! Constructor.

    File(const String &, bool stat = true);

    //! Set the file name.
    //
    //! \param stat Get information from the file system.

    void set(const String &, bool stat = true);

    //! Get the file name.
    //
    //! \param frame Specify a frame number or -1 for the entire sequence.
    //! \param path Include the path in the file name.

    String get(int64_t frame = -1, bool path = true) const;

    //! Get the file name without the path.

    String name() const;

    //! Set the path.

    void path(const String &);

    //! Set the base.

    void base(const String &);

    //! Set the number.

    void number(const String &);

    //! Set the extension.

    void extension(const String &);

    //! Get the path.

    inline const String & path() const;

    //! Get the base.

    inline const String & base() const;

    //! Get the number.

    inline const String & number() const;

    //! Get the extension.

    inline const String & extension() const;

    //! File type.

    enum TYPE
    {
        FILE,       //!< Regular file
        SEQ,        //!< File sequence
        DIRECTORY,  //!< Directory

        _TYPE_SIZE
    };

    //! File permissions.

    enum PERM
    {
        READ  = 1,  //!< Readable
        WRITE = 2,  //!< Writable
        EXEC  = 4   //!< Executable
    };

    static const int _PERM_SIZE = 3;

    //! Set the type.

    void type(TYPE);

    //! Set the size.

    void size(uint64_t);

    //! Set the user.

    void user(uid_t);

    //! Set the permissions.

    void perm(int);

    //! Set the time.

    void time(time_t);

    //! Get the type.

    inline TYPE type() const;

    //! Get the size.

    inline uint64_t size() const;

    //! Get the user.

    inline uid_t user() const;

    //! Get the permissions.

    inline int perm() const;

    //! Get the time.

    inline time_t time() const;

    //! Get information from the file system.

    bool stat(const String & path = String());

    //! Set the sequence.

    void seq(const Seq &);

    //! Get the sequence.

    inline const Seq & seq() const;

    //! Is the sequence valid?

    inline bool is_seq_valid() const;

    //! Is the sequence a wildcard?

    inline bool is_seq_wildcard() const;

    //! Add a file to the sequence. The file is checked to see whether it is
    //! compatible.

    inline bool seq_add(const File &);

    //! Is the file extension compatible with sequencing?

    inline bool has_seq_extension() const;

    //! The sequence extension list.

    static Set<String> seq_extensions;

    inline operator String() const;

private:

    void init();

    void update_has_seq_extension();

    String _path;
    String _base;
    String _number;
    String _extension;

    TYPE     _type;
    uint64_t _size;
    uid_t    _user;
    int      _perm;
    time_t   _time;

    Seq  _seq;
    bool _has_seq_extension;

    friend struct File_Util;
};

//------------------------------------------------------------------------------
//! \struct File_Util
//!
//! This struct provides file utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT File_Util
{
    //! Split a file name into pieces.

    static void split(
        const String &,
        String & path,
        String & base,
        String & number,
        String & extension);

    //! Check if a file exists.

    static bool exists(const File &);

    //! Sort frame numbers in a sequence.

    static void seq_sort(File &);

    //! Find a match for a sequence wildcard. If nothing is found the input is
    //! returned.
    //!
    //! Example wildcard: render.#.exr

    static const File & seq_wildcard_match(const File &, const List<File> &);

    //! Compress a list of files into a sequence.

    static void seq_compress(
        List<File> &,
        Seq::COMPRESS = Seq::COMPRESS_RANGE);

    //! Expand a sequence into a list of file names.

    static List<String> seq_expand(const File &);

    //! File filtering.

    enum FILTER
    {
        FILTER_NONE = 0,
        FILTER_FILE = 1,       //!< Filter files
        FILTER_DIRECTORY = 2,  //!< Filter directories
        FILTER_HIDDEN = 4      //!< Filter hidden items
    };

    //! Filter a list of files.

    static void filter(
        List<File> &,
        int filter,
        const List<String> & glob = List<String>(),
        bool glob_files_only = false);

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

    static void sort(
        List<File> &,
        SORT,
        bool reverse = false);

    //! Sort list so directories are first.

    static void sort_directories_first(List<File> &);

    //! Convert a path to an absolute path.

    static String path_absolute(const String &);

    //! Is a path absolute?

    static bool path_is_absolute(const String &);

    //! Fix a path.

    static String path_fix(const String &);

    //static String path_posix(const String &);

    //! Update a recent files list.

    static void recent(const String &, List<String> &, int max = 10);

    //! Path separators.

    static List<char> path_separators;

    //! Path separator.

# if defined(DJV_WINDOWS)
    static const char & path_separator;
# else
    static const char & path_separator;
# endif

    //! Path list separators.

    static List<char> list_separators;

    //! Path list separator.

# if defined(DJV_WINDOWS)
    static const char & list_separator;
# else
    static const char & list_separator;
# endif

    static const List<String> & label_type();
    static String               label_size(uint64_t);
    static const List<String> & label_perm();
    static const String &       label_perm(int);
    static const List<String> & label_sort();
};

//------------------------------------------------------------------------------

inline bool operator == (const File &, const File &);

inline bool operator != (const File &, const File &);

DJV_CORE_EXPORT String & operator >> (String &, File::TYPE &) throw (String);
DJV_CORE_EXPORT String & operator >> (String &, File_Util::SORT &)
    throw (String);

DJV_CORE_EXPORT String & operator << (String &, File::TYPE);
DJV_CORE_EXPORT String & operator << (String &, File_Util::SORT);

DJV_CORE_EXPORT Debug & operator << (Debug &, const File &);

} // djv

#include <djv_file_inline.h>

#endif // DJV_FILE_H
