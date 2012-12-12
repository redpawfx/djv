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

//! \file djv_string.h

#ifndef DJV_STRING_H
#define DJV_STRING_H

#include <djv_list.h>

#include <string>

namespace djv
{

//------------------------------------------------------------------------------
//! \class String
//!
//! This class provides a string.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT String : public std::string
{
public:

    //! Constructor.

    inline String();

    //! Constructor.

    inline String(
        const String &,
        size_type position = 0,
        size_type size     = npos);

    //! Constructor.

    inline String(const std::string &);

    //! Constructor.

    inline String(const char *);

    //! Constructor.

    inline String(const char *, size_type size);

    //! Constructor.

    inline String(char, size_type size = 1);
};

//! Default C-string size.

static const size_t cstring_size = 256;

//------------------------------------------------------------------------------
//! \class String_Format
//!
//! This class provides string formatting.
//!
//! Example:
//!
//! String s = String_Format("string = %%, int = %%, double = %%").
//!   arg("abc").
//!   arg(123).
//!   arg(456.0);
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT String_Format
{
public:

    //! Constructor.

    String_Format(const String &);

    //! String argument.

    String_Format arg(
        const String &,
        int width = 0,
        char fill = ' ') const;

    //! String list argument.

    String_Format arg(
        const List<String> &,
        const String & separator = " ",
        int width = 0,
        char fill = ' ') const;

    //! Integer argument.

    String_Format arg(
        int,
        int width = 0,
        char fill = ' ') const;

    //! Integer argument.

    String_Format arg(
        int64_t,
        int width = 0,
        char fill = ' ') const;

    //! Integer argument.

    String_Format arg(
        uint,
        int width = 0,
        char fill = ' ') const;

    //! Integer argument.

    String_Format arg(
        uint64_t,
        int width = 0,
        char fill = ' ') const;

    //! Floating-point argument.

    String_Format arg(
        double,
        int width = 0,
        int precision = 0,
        char fill = ' ') const;

    operator String () const;

private:

    String _value;
};

//------------------------------------------------------------------------------
//! \struct String_Util
//!
//! This struct provides string utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT String_Util
{
    //! Convert to lower-case.

    static inline char lower(char);

    //! Convert to lower-case.

    static inline String lower(const String &);

    //! Convert to lower-case.

    static inline List<String> lower(const List<String> &);

    //! Convert to upper-case.

    static inline char upper(char);

    //! Convert to upper-case.

    static inline String upper(const String &);

    //! Convert to upper-case.

    static inline List<String> upper(const List<String> &);

    //! Replace characters in a string.

    static inline String replace(
        const String &,
        char match,
        char replace);

    //! Split a string into a list of strings.

    static List<String> split(
        const String &,
        const String & separator,
        bool keep_empty = false);

    //! Split a string into a list of strings.

    static List<String> split(
        const String &,
        char separator,
        bool keep_empty = false);

    //! Split a string into a list of strings.

    static List<String> split(
        const String &,
        const List<String> & separator,
        bool keep_empty = false);

    //! Split a string into a list of strings.

    static List<String> split(
        const String &,
        const List<char> & separator,
        bool keep_empty = false);

    //! Join a list of strings into a single string.

    static inline String join(
        const List<String> &,
        const String & separator = String());

    //! Join a list of strings into a single string.

    static inline String join(
        const List<String> &,
        char separator);

    //! Test whether a character is upper case.

    static inline bool is_upper(char);

    //! Test whether a character is lower case.

    static inline bool is_lower(char);

    //! Test whether a character is a number.

    static inline bool is_digit(char);

    //! Compare two string case insensitive.

    static inline bool compare_no_case(const String &, const String &);

    //! Find a string in a list.

    static inline bool find(
        const String &,
        const List<String> &,
        size_t * index = 0);

    //! Find a string in a list case insensitive.

    static inline bool find_no_case(
        const String &,
        const List<String> &,
        size_t * index = 0);

    //! C-string conversion.

    static size_t cstring(
        const String &,
        char *,
        size_t max_len = 0,
        bool terminate = true);

    //! Convert a string to an integer.

    template<typename T>
    static inline T string_to_int(
        const char *,
        size_t max_len = cstring_size);

    //! Convert a string to an integer.

    static inline int string_to_int(const String &);

    //! Convert a string to an integer.

    static inline int64_t string_to_int64(const String &);

    //! Convert an integer to a string.

    template<typename T>
    static inline size_t int_to_string(
        T,
        char *,
        size_t max_len = cstring_size);

    //! Convert an integer to a string.

    template<typename T>
    static inline String int_to_string(T);

    //! Convert a string to an unsigned integer.

    template<typename T>
    static inline T string_to_uint(
        const char *,
        size_t max_len = cstring_size);

    //! Convert a string to an unsigned integer.

    static inline uint string_to_uint(const String &);

    //! Convert an unsigned integer to a string.

    template<typename T>
    static inline size_t uint_to_string(
        T,
        char *,
        size_t max_len = cstring_size);

    //! Convert an unsigned integer to a string.

    template<typename T>
    static inline String uint_to_string(T);

    //! Convert a string to a floating-point number.

    static double string_to_float(const String &);

    //! Convert a floating-point number to a string.

    static String float_to_string(double, int precision = 0);

    //! Get a label for a value.

    template<typename T>
    static inline String label(const T &);

    //! Get the boolean labels.

    static const List<String> & label_bool();
};

//! String formatting.

#if defined(DJV_WINDOWS)
//! \todo MinGW does not define sprintf_s()?
//#define SNPRINTF ::sprintf_s
#define SNPRINTF ::snprintf
#else
#define SNPRINTF ::snprintf
#endif

//------------------------------------------------------------------------------

//! Serialize.

DJV_CORE_EXPORT bool serialize(String *, String *);
DJV_CORE_EXPORT bool serialize(String *, uint *, const List<String> & label);
DJV_CORE_EXPORT bool serialize(String *, String *, const List<String> & label);
template <typename T>
inline bool serialize(String *, T *, const List<String> & label);

DJV_CORE_EXPORT String serialize(const String &);
template <typename T>
inline String serialize(T, const List<String> & label);

DJV_CORE_EXPORT String & operator >> (String &, String &) throw (String);
DJV_CORE_EXPORT String & operator >> (String &, bool &) throw (String);
DJV_CORE_EXPORT String & operator >> (String &, int &) throw (String);
DJV_CORE_EXPORT String & operator >> (String &, uint &) throw (String);
DJV_CORE_EXPORT String & operator >> (String &, double &) throw (String);
template <typename T>
inline String & operator >> (String &, List<T> &) throw (String);

DJV_CORE_EXPORT String & operator << (String &, const String &);
DJV_CORE_EXPORT String & operator << (String &, bool);
DJV_CORE_EXPORT String & operator << (String &, int);
DJV_CORE_EXPORT String & operator << (String &, uint);
DJV_CORE_EXPORT String & operator << (String &, double);
template <typename T>
inline String & operator << (String &, const List<T> &);

//! Convenience macros for defining string serialize operators.

#define _DJV_STRING_OPERATOR_LABEL(TYPE, LABEL) \
    \
    String & operator >> (String & in, TYPE & out) throw (String) \
    { \
        if (! serialize(&in, &out, LABEL)) \
        { \
            throw in; \
        } \
        \
        return in; \
    } \
    \
    String & operator << (String & out, TYPE in) \
    { \
        return out << serialize(in, LABEL); \
    }

} // djv

#include <djv_string_inline.h>

#endif // DJV_STRING_H

