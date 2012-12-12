/*------------------------------------------------------------------------------
 include/djv_base/string_fnc_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace string {

//------------------------------------------------------------------------------
// lower(), upper()
//------------------------------------------------------------------------------

// Case-conversion lookup tables.

static int _lower [] =
{
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
   15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
   30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
   45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
   60,  61,  62,  63,  64,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
  107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
  122,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104,
  105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
  120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
  135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
  150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
  165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
  180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
  195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
  210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
  225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
  240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
  255
};

static int _upper [] =
{
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
   15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
   30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
   45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
   60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,
   75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
   90,  91,  92,  93,  94,  95,  96,  65,  66,  67,  68,  69,  70,  71,  72,
   73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,
   88,  89,  90, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
  135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
  150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
  165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
  180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
  195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
  210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
  225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
  240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
  255
};

inline char lower(char in)
{
  return _lower[in];
}

inline String lower(const String & in)
{
  const String::size_type size = in.size();

  String out;
  out.resize(size);
  
  for (String::size_type i = 0; i < size; ++i)
    out[i] = _lower[static_cast<int>(in[i])];

  return out;
}

inline List<String> lower(const List<String> & in)
{
  const String::size_type size = in.size();

  List<String> out;
  out.resize(size);

  for (String::size_type i = 0; i < size; ++i)
    out[i] = lower(in[i]);

  return out;
}

inline char upper(char in)
{
  return _upper[in];
}

inline String upper(const String & in)
{
  const String::size_type size = in.size();

  String out;
  out.resize(size);

  for (String::size_type i = 0; i < size; ++i)
    out[i] = _upper[static_cast<int>(in[i])];

  return out;
}

inline List<String> upper(const List<String> & in)
{
  const String::size_type size = in.size();

  List<String> out;
  out.resize(size);

  for (String::size_type i = 0; i < size; ++i)
    out[i] = upper(in[i]);

  return out;
}

//------------------------------------------------------------------------------
// join()
//------------------------------------------------------------------------------

inline String join(const List<String> & in, const String & separator)
{
  const String::size_type size = in.size();

  String out;
  
  for (String::size_type i = 0; i < size; ++i)
  {
    if (i)
      out += separator;
    out += in[i];
  }

  return out;
}

inline String join(const List<String> & in, char separator)
{
  const String::size_type size = in.size();

  String out;

  for (String::size_type i = 0; i < size; ++i)
  {
    if (i)
      out += separator;
    out += in[i];
  }

  return out;
}

//------------------------------------------------------------------------------
// replace()
//------------------------------------------------------------------------------

inline String replace(const String & in, char match, char replace)
{
  const String::size_type size = in.size();

  String out = in;

  for (String::size_type i = 0; i < size; ++i)
    if (out[i] == match)
      out[i] = replace;

  return out;
}

//------------------------------------------------------------------------------
// is_upper(), is_lower(), is_digit()
//------------------------------------------------------------------------------

inline bool is_upper(char in)
{
  return in >= 65 && in <= 90;
}

inline bool is_lower(char in)
{
  return in >= 97 && in <= 122;
}

inline bool is_digit(char in)
{
  return in >= 48 && in <= 57;
}

//------------------------------------------------------------------------------
// compare_no_case()
//------------------------------------------------------------------------------

inline bool compare_no_case(const String & a, const String & b)
{
  return lower(a) == lower(b);
}

//------------------------------------------------------------------------------
// find()
//------------------------------------------------------------------------------

/*inline bool find(
  const String & a,
  const List<String> & b,
  list::size_type * index
) {
  const list::size_type size = b.size();

  for (list::size_type i = 0; i < size; ++i)
    if (a == b[i])
    {
      if (index)
        *index = i;
      return true;
    }

  return false;
}*/

//------------------------------------------------------------------------------
// find_no_case()
//------------------------------------------------------------------------------

inline bool find_no_case(
  const String & a,
  const List<String> & b,
  list::size_type * index
) {
  const list::size_type size = b.size();

  const String tmp = lower(a);
  for (list::size_type i = 0; i < size; ++i)
    if (tmp == lower(b[i]))
    {
      if (index)
        *index = i;
      return true;
    }

  return false;
}

//------------------------------------------------------------------------------
// string_to_int()
//------------------------------------------------------------------------------

namespace {

// Valid character lookup tables.

static int _string_to_int [] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0
};

}

template<typename T>
inline T string_to_int(const char * in, String::size_type max_len)
{
  bool negative = false;
  if (*in)
    if ('-' == *in)
    {
      negative = true;
      ++in;
    }
  
  T out = 0;
  while (_string_to_int[static_cast<int>(*in)] && max_len--)
  {
    out *= 10;
    out += *in++ - '0';
  }
  return negative ? -out : out;
}

inline int string_to_int(const String & in)
{
  return string_to_int<int>(in.c_str());
}

inline int64_t string_to_int64(const String & in)
{
  return string_to_int<int64_t>(in.c_str());
}

//------------------------------------------------------------------------------
// int_to_string()
//------------------------------------------------------------------------------

template<typename T>
inline String::size_type int_to_string(
  T in,
  char * out,
  String::size_type max_len
) {
  //DJV_ASSERT(max_len > 0);

  const String::size_type max = max_len - 1;
  
  const bool negative = ! (0 == in || in > 0); // XXX Compiler warnings.
  //if (negative)
  //  in = -in;
  
  String::size_type size = 0;
  T tmp = in;
  do
  {
    ++size;
    tmp /= 10;
  }
  while (tmp && size < max);
  
  if (negative)
  {
    ++size;
    out[0] = '-';
  }
  
  if (size > 0)
  {
    // XXX Using a signed integer for now to keep the logic simple...
    
    const int end = negative ? 1 : 0;
    for (int i = static_cast<int>(size) - 1; i >= end; --i, in /= 10)
      if (negative)
        out[i] = '0' - (in % 10);
      else
        out[i] = '0' + (in % 10);
  }
  
  out[size] = 0;

  return size;
}

template<typename T>
inline String int_to_string(T in)
{
  char tmp [cstring_size] = "";

  //const int size = SNPRINTF(tmp, cstring_size, "%d", in);

  const String::size_type size = int_to_string(in, tmp);

  return String(tmp, size);
}

//------------------------------------------------------------------------------
// string_to_uint()
//------------------------------------------------------------------------------

template<typename T>
inline T string_to_uint(const char * in, String::size_type max_len)
{  
  T out = 0;
  while (_string_to_int[static_cast<int>(*in)] && max_len--)
  {
    out *= 10;
    out += *in++ - '0';
  }
  return out;
}

inline uint string_to_uint(const String & in)
{
  return string_to_uint<uint>(in.c_str());
}

//------------------------------------------------------------------------------
// uint_to_string()
//------------------------------------------------------------------------------

template<typename T>
inline String::size_type uint_to_string(
  T in,
  char * out,
  String::size_type max_len
) {
  //DJV_ASSERT(max_len > 0);

  const String::size_type max = max_len - 1;
  
  String::size_type size = 0;
  T tmp = in;
  do
  {
    ++size;
    tmp /= 10;
  }
  while (tmp && size < max);
  
  if (size > 0)
  {
    // XXX Using a signed integer for now to keep the logic simple...
    
    for (int i = static_cast<int>(size) - 1; i >= 0; --i, in /= 10)
      out[i] = '0' + (in % 10);
  }
  
  out[size] = 0;

  return size;
}

template<typename T>
inline String uint_to_string(T in)
{
  char tmp [cstring_size] = "";

  const String::size_type size = uint_to_string(in, tmp);

  return String(tmp, size);
}

//------------------------------------------------------------------------------
// serialize()
//------------------------------------------------------------------------------

template <typename T>
inline bool serialize(String * in, T * out, const List<String> & label)
{
  uint i = 0;
  if (! serialize(in, &i, label))
    return false;
  *out = static_cast<T>(i);
  return true;
}

template <typename T>
inline String serialize(T in, const List<String> & label)
{
  if (static_cast<list::size_type>(in) >= label.size())
    return String();
  return label[in];
}

template <typename T>
inline String & operator >> (String & in, List<T> & out) throw (String)
{
  List<T> tmp;
  while (in.size())
  {
    T value;
    in >> value;
    tmp += value;
  }
  
  // XXX
  
  if (! out.size())
  {
    out = tmp;
  }
  else
  {
    const list::size_type out_size = out.size();
    const list::size_type tmp_size = tmp.size();
    for (list::size_type i = 0; i < out_size && i < tmp_size; ++i)
      out[i] = tmp[i];
  }
  
  return in;
}

template <typename T>
inline String & operator << (String & out, const List<T> & in)
{
  const list::size_type size = in.size();
  if (size && out.size())
    out += ' ';
  
  List<String> tmp;
  tmp.resize(size);
  for (list::size_type i = 0; i < size; ++i)
    tmp[i] << in[i];
  out += join(tmp, ' ');
  
  return out;
}

//------------------------------------------------------------------------------
// label()
//------------------------------------------------------------------------------

template<typename T>
inline String label(const T & in)
{
  List<String> out;
  
  String value;
  value << in;

  try
  {
    while (value.size())
    {
      String tmp;
      value >> tmp;
      out += tmp;
    }
  }
  catch (String) {}
  
  return join(out, ' ');
}

//------------------------------------------------------------------------------

}
}

