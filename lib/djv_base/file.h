/*------------------------------------------------------------------------------
 include/djv_base/file.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_FILE_H
#define DJV_FILE_H

#include <djv_base/file_def.h>

#include <djv_base/file_seq.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::file
//
//! File-system information.
//------------------------------------------------------------------------------

namespace file {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

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

//@}
//------------------------------------------------------------------------------
//!\class File
//
//! Information about files.
//------------------------------------------------------------------------------

class _DJV_BASE File
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  File();
  
  File(const String &, bool stat = true);

  //@}
  //----------------------------------------------------------------------------
  //!\name File Name
  //@{
  
  //!\fn Set the file name.
  //
  //!\param stat Get information from the file system.
  
  void set(const String &, bool stat = true);
  
  //!\fn Get the file name.
  //
  //!\param frame Specify a frame number or -1 for the entire sequence.
  //!\param path Include the path in the file name.
  
  String get(int64_t frame = -1, bool path = true) const;
  
  //! Get the file name without the path.
  
  String name() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name File Name Components
  //!
  //! Example: /tmp/render.1-100.exr
  //!
  //! - path      - /tmp/
  //! - base      - render.
  //! - number    - 1-100
  //! - extension - .exr
  //@{
  
  void path(const String &);
  void base(const String &);
  void number(const String &);
  void extension(const String &);
  
  inline const String & path() const;
  inline const String & base() const;
  inline const String & number() const;
  inline const String & extension() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name File Information
  //@{
  
  void type(TYPE);
  void size(uint64_t);
  void user(uid_t);
  void perm(int);
  void time(time_t);
  
  inline TYPE     type() const;
  inline uint64_t size() const;
  inline uid_t    user() const;
  inline int      perm() const;
  inline time_t   time() const;
  
  //! Get information from the file system.
  
  bool stat(const String & path = String());
  
  //@}
  //----------------------------------------------------------------------------
  //!\name File Sequence
  //@{
  
  void seq(const file_seq::Seq &);

  inline const file_seq::Seq & seq() const;
  
  //! Is the sequence valid?
  
  inline bool is_seq_valid() const;

  //! Is the sequence a wildcard?
  
  inline bool is_seq_wildcard() const;
  
  //! Add a file to the sequence. The file is checked to see whether it is
  //! compatible.
  
  inline bool seq_add(const File &);
  
  //! Is the file extension compatible with sequencing?

  inline bool has_seq_extension() const;
  
  static void seq_extension_list(const List<String> &);

  static const List<String> & seq_extension_list();

  //@}
  //----------------------------------------------------------------------------
  //!\name Conversion
  //@{

  inline operator String() const;
  
  //@}

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
  
  file_seq::Seq _seq;
  bool _has_seq_extension;
  static List<String> _seq_extension_list;
  
  _DJV_BASE friend void seq_sort(File &);
  _DJV_BASE friend void seq_compress(
    List<File> &,
    file_seq::COMPRESS
  );
};

//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

inline bool operator == (const File &, const File &);

inline bool operator != (const File &, const File &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_BASE String & operator >> (String &, TYPE &) throw (String);

_DJV_BASE String & operator << (String &, TYPE);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_BASE const List<String> & label_type();
_DJV_BASE String               label_size(uint64_t);
_DJV_BASE const List<String> & label_perm();
_DJV_BASE const String &       label_perm(int);

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_BASE Debug & operator << (Debug &, const File &);

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_base/file_inline.h>
#include <djv_base/file_fnc.h>
#endif

