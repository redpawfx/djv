/*------------------------------------------------------------------------------
 include/djv_base/string_fnc.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace string {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Convert to lower-case.

inline char lower(char);
inline String lower(const String &);
inline List<String> lower(const List<String> &);

//! Convert to upper-case.

inline char upper(char);
inline String upper(const String &);
inline List<String> upper(const List<String> &);

//! Replace characters in a string.

inline String replace(
  const String &,
  char match,
  char replace
);

//! String formatting.

#if defined(DJV_WINDOWS)
#define SNPRINTF ::sprintf_s
#else
#define SNPRINTF ::snprintf
#endif

//! Split a string into lines of a given width.

_DJV_BASE List<String> lines(const String &, int width, bool indent = true);

//------------------------------------------------------------------------------
//!\name String Lists
//------------------------------------------------------------------------------
//@{

//! Split a string into a list of strings.

_DJV_BASE List<String> split(
  const String &,
  const String & separator,
  bool keep_empty = false
);

_DJV_BASE List<String> split(
  const String &,
  char separator,
  bool keep_empty = false
);

_DJV_BASE List<String> split(
  const String &,
  const List<String> & separator,
  bool keep_empty = false
);

_DJV_BASE List<String> split(
  const String &,
  const List<char> & separator,
  bool keep_empty = false
);

//! Join a list of strings into a single string.

inline String join(
  const List<String> &,
  const String & separator = String()
);

inline String join(
  const List<String> &,
  char separator
);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

inline bool is_upper(char);
inline bool is_lower(char);
inline bool is_digit(char);

inline bool compare_no_case(const String &, const String &);

// XXX list::find() collisions?

/*inline bool find(
  const String &,
  const List<String> &,
  list::size_type * index = 0
);*/

inline bool find_no_case(
  const String &,
  const List<String> &,
  list::size_type * index = 0
);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

//! C-string conversion.

_DJV_BASE String::size_type cstring(
  const String &,
  char *,
  String::size_type max_len = 0,
  bool terminate = true
);

//! Integer conversion.

template<typename T>
inline T string_to_int(
  const char *,
  String::size_type max_len = cstring_size
);

inline int string_to_int(const String &);
inline int64_t string_to_int64(const String &);

template<typename T>
inline String::size_type int_to_string(
  T,
  char *,
  String::size_type max_len = cstring_size
);

template<typename T>
inline String int_to_string(T);

//! Unsigned integer conversion.

template<typename T>
inline T string_to_uint(
  const char *,
  String::size_type max_len = cstring_size
);

inline uint string_to_uint(const String &);

template<typename T>
inline String::size_type uint_to_string(
  T,
  char *,
  String::size_type max_len = cstring_size
);

template<typename T>
inline String uint_to_string(T);

//! Floating-point conversion.

_DJV_BASE double string_to_float(const String &);

_DJV_BASE String float_to_string(double, int precision = 0);

//! Serialize.

_DJV_BASE bool serialize(String *, String *);
_DJV_BASE bool serialize(String *, uint *, const List<String> & label);
_DJV_BASE bool serialize(String *, String *, const List<String> & label);
template <typename T>
inline bool serialize(String *, T *, const List<String> & label);

_DJV_BASE String serialize(const String &);
template <typename T>
inline String serialize(T, const List<String> & label);

_DJV_BASE String & operator >> (String &, String &) throw (String);
_DJV_BASE String & operator >> (String &, bool &) throw (String);
_DJV_BASE String & operator >> (String &, int &) throw (String);
_DJV_BASE String & operator >> (String &, uint &) throw (String);
_DJV_BASE String & operator >> (String &, double &) throw (String);
template <typename T>
inline String & operator >> (String &, List<T> &) throw (String);

_DJV_BASE String & operator << (String &, const String &);
_DJV_BASE String & operator << (String &, bool);
_DJV_BASE String & operator << (String &, int);
_DJV_BASE String & operator << (String &, uint);
_DJV_BASE String & operator << (String &, double);
template <typename T>
inline String & operator << (String &, const List<T> &);

//! Convenience macros for defining string serialize operators.

#define _DJV_STRING_OPERATOR_LABEL(TYPE, LABEL) \
  \
  String & operator >> (String & in, TYPE & out) throw (String) \
  { \
    if (! string::serialize(&in, &out, LABEL)) \
      throw in; \
    return in; \
  } \
  \
  String & operator << (String & out, TYPE in) \
  { \
    return out << string::serialize(in, LABEL); \
  }

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

template<typename T>
inline String label(const T &);

_DJV_BASE const List<String> & label_bool();

//@}
//------------------------------------------------------------------------------

}
}

