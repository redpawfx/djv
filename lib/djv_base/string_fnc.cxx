/*------------------------------------------------------------------------------
 lib/djv_base/string_fnc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "string.h"

#include <djv_base/math.h>
#include <djv_base/memory.h>
#if defined(DJV_LINUX)
#include <regex.h>
#endif
#include <stdlib.h>

namespace djv_base {
namespace string {

//------------------------------------------------------------------------------
// lines()
//------------------------------------------------------------------------------

namespace {

#if defined(DJV_LINUX)

// XXX Special-case indentation.

static const List<String> indent_special_pattern = List<String>() <<
  "^\\s*\\* .*\\: " <<  // e.g., " * Load: "
  "^\\s*\\* " <<        // e.g., " * "
  "^\\s*Values = " <<   // e.g., "   Values = "
  "^\\s*Default = ";    // e.g., "   Default = "

class Regex
{
public:

  Regex(const String & pattern, const String & in)
  {
    _match.rm_so = -1;
    _match.rm_eo = 0;
    
    int err = regcomp(&_regex, pattern.c_str(), REG_EXTENDED);
    if (err)
      return;
    
    err = regexec(&_regex, in.c_str(), 1, &_match, 0);
    if (err)
      return;
  }

  ~Regex()
  {
    regfree(&_regex);
  }
  
  bool match() const
  {
    return _match.rm_so != -1;
  }
  
  int size() const
  {
    return _match.rm_eo;
  }
  
private:

  regex_t _regex;
  regmatch_t _match;
};

int indent_special(const String & line, int indent)
{
  int out = indent;
  
  for (list::size_type i = 0; i < indent_special_pattern.size(); ++i)
  {
    const Regex regex(indent_special_pattern[i], line);
    
    if (regex.match())
    {
      out = regex.size();
      
      break;
    }
  }

  return out;
}

#else

// XXX Special-case indentation.

static const List<String> indent_special_pattern = List<String>() <<
  " * " <<
  "   Values = " <<
  "   Default = ";

String::size_type indent_special(
  const String & line,
  String::size_type indent
) {
  String::size_type out = indent;
  
  for (list::size_type i = 0; i < indent_special_pattern.size(); ++i)
  {
    const String & pattern = indent_special_pattern[i];
    
    if (pattern == String(line, 0, pattern.size()))
    {
      out = pattern.size();
      
      break;
    }
  }

  return out;
}

#endif

}

List<String> lines(const String & in, int width, bool indent)
{
  //width = 30;
  
  DJV_DEBUG("lines");
  //DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("width = " << width);
  
  width = math::max(width, 1);
  
  List<String> out;
  
  // Split into lines.
  
  const List<String> line_list = string::split(in, '\n', true);
  
  for (list::size_type i = 0; i < line_list.size(); ++i)
  {
    DJV_DEBUG_PRINT("in = \"" << line_list[i] << "\"");
    
    // Split line into words.
    
    List<String> word_list = string::split(line_list[i], ' ', true);
    
    // Indentation.
    
    String::size_type indent_size = 0;
    
    if (indent && word_list.size())
      for (
        ;
        indent_size < word_list.size() && word_list[indent_size].empty();
        ++indent_size
      )
        ;
        
    // Make new lines according to width.
    
    bool line_break = false;

    for (list::size_type j = 0; j < word_list.size(); ++j)
    {
      String line;
      
      if (line_break)
        line += String(' ', indent_size);
      
      for (list::size_type k = 0; j < word_list.size(); ++k, ++j)
      {
        // Add space between words.
        
        if (k)
          line += ' ';
        
        // Add the word.
        
        line += word_list[j];
        
        // Break the line if we have exceeded the width.
        
        if (j < word_list.size() - 1)
          if (
            static_cast<int>(line.size() + 1 + word_list[j + 1].size()) >=
            width
          ) {
            if (! line_break)
            {
              line_break = true;
              
              if (indent)
                indent_size = indent_special(line, indent_size);
            }
            
            break;
          }
      }

      DJV_DEBUG_PRINT("     \"" << line << "\"");
      
      out += line;
    }
  }

  return out;
}

//------------------------------------------------------------------------------
// split()
//------------------------------------------------------------------------------

List<String> split(
  const String & in,
  const String & separator,
  bool keep_empty
) {
  return split(in, List<String>() << separator, keep_empty);
}

List<String> split(
  const String & in, const List<String> & separator, bool keep_empty
) {
  List<String> out;

  const String::size_type size = in.size();
  String::size_type i = 0, j = 0;
  for (; i < size; ++i)
    for (list::size_type k = 0; k < separator.size(); ++k)
    {
      const String::size_type separator_size = separator[k].size();
      if (0 == in.compare(i, separator_size, separator[k]))
      {
        if (i != j)
          out += String(in, j, i - j);
        else if (keep_empty)
          out += String();
        i += separator_size - 1;
        j = i + 1;
        break;
      }
    }

  if (j < size)
    out += String(in, j, i - j);
  else if (! size)
    out += String();

  return out;
}

List<String> split(
  const String & in,
  char separator,
  bool keep_empty
) {
  return split(in, List<char>() << separator, keep_empty);
}

List<String> split(
  const String & in,
  const List<char> & separator,
  bool keep_empty
) {
  List<String> out;
    
  const String::size_type size = in.size();
  String::size_type i = 0, j = 0;
  for (; i < size; ++i)
    for (list::size_type k = 0; k < separator.size(); ++k)
      if (in[i] == separator[k])
      {
        if (i != j)
          out += String(in, j, i - j);
        else if (keep_empty)
          out += String();
        j = i + 1;
        break;
      }

  if (j < size)
    out += String(in, j, i - j);
  else if (! size)
    out += String();

  return out;
}

//------------------------------------------------------------------------------
// cstring()
//------------------------------------------------------------------------------

String::size_type cstring(
  const String & in,
  char * out,
  String::size_type max_len,
  bool terminate
) {
  DJV_ASSERT(max_len > 0);

  const String::size_type size = max_len ?
    math::min(in.size(), max_len - static_cast<String::size_type>(terminate)) :
    (in.size() + static_cast<String::size_type>(terminate));

  String::size_type i = 0;
  for (; i < size; ++i)
    out[i] = in[i];
  if (terminate)
    out[i++] = 0;
  return i;
}

//------------------------------------------------------------------------------
// string_to_float(), float_to_string()
//------------------------------------------------------------------------------

double string_to_float(const String & in)
{
  return atof(in.c_str());
}

String float_to_string(double in, int precision)
{  
  char tmp [cstring_size] = "";
  int size = 0;
  
  if (precision)
  {
    char format [] = "%0. f";
    format[3] = '0' + abs(precision);
    size = SNPRINTF(tmp, cstring_size, format, in);
  }
  else
  {
    size = SNPRINTF(tmp, cstring_size, "%0.3f", in);
  }

  if (precision <= 0 && size)
  {
    int i = size - 1;
    for (; i >= 0 && '0' == tmp[i]; --i, --size) ;
    if (i)
      if ('.' == tmp[i]) --size;
  }
  return String(tmp, (size > 0) ? size : 0);
}

//------------------------------------------------------------------------------
// serialize()
//------------------------------------------------------------------------------

namespace {

const String serialize_null = "null";

const char serialize_separator = '|';

}

bool serialize(String * in, String * out)
{
  const String::size_type size = in->size();
  if (! size)
    return false;
  
  // Find word.
  
  String::size_type start = 0;
  for (; ' ' == (*in)[start] && start < size; ++start)
    ;
  String::size_type end = start;
  for (; ' ' != (*in)[end] && end < size; ++end)
    ;
  
  // Extract word.
  
  const String tmp = String(*in, start, end - start);
  if (tmp != serialize_null)
    //*out = tmp;
    *out = replace(tmp, serialize_separator, ' ');
  *in = String(*in, end, size - end + 1);
  return true;
}

bool serialize(String * in, uint * out, const List<String> & label)
{
  const String::size_type size = in->size();
  if (! size)
    return false;
  
  // XXX Case-insensitive?

  //const String _in = lower(*in);

  // XXX Extra space prevents partial matches.
  
  const String _in = lower(*in) + " ";
  
  // Find word.
  
  String::size_type start = 0;
  for (; ' ' == _in[start] && start < size; ++start)
    ;

  // Extract word.
  
  const list::size_type label_size = label.size();
  for (list::size_type i = 0; i < label_size; ++i)
  {
    //if (0 == _in.compare(start, label[i].size(), lower(label[i])))
    const String tmp = replace(String(_in, start, label[i].size() + 1),
      serialize_separator, ' ');
    if (compare_no_case(tmp, label[i] + " "))
    {
      *out = static_cast<uint>(i);
      const String::size_type end = start + label[i].size();
      *in = String(*in, end, size - end + 1);
      return true;
    }
  }
  
  return false;
}

bool serialize(String * in, String * out, const List<String> & label)
{
  uint i = 0;
  if (! serialize(in, &i, label))
    return false;
  *out = label[i];
  return true;
}

String serialize(const String & in)
{
  //return in.size() ? in : serialize_null;
  const String tmp = replace(in, ' ', serialize_separator);
  return tmp.size() ? tmp : serialize_null;
}

String & operator >> (String & in, String & out) throw (String)
{
  if (! serialize(&in, &out)) throw in;
  return in;
}

String & operator << (String & out, const String & in)
{
  if (out.size()) out += ' ';
  out += serialize(in);
  return out;
}

String & operator >> (String & in, bool & out) throw (String)
{
  uint i = 0;
  if (! serialize(&in, &i, label_bool()))
    throw in;
  out = i != 0;
  return in;
}

String & operator << (String & out, bool in)
{
  if (out.size()) out += ' ';
  out += label_bool()[in];
  return out;
}

String & operator >> (String & in, int & out) throw (String)
{
  String tmp;
  in >> tmp;
  out = string_to_int(tmp);
  return in;
}

String & operator << (String & out, int in)
{
  if (out.size()) out += ' ';
  out += int_to_string(in);
  return out;
}

String & operator >> (String & in, uint & out) throw (String)
{
  String tmp;
  in >> tmp;
  out = string_to_uint(tmp);
  return in;
}

String & operator << (String & out, uint in)
{
  if (out.size()) out += ' ';
  out += uint_to_string(in);
  return out;
}

String & operator >> (String & in, double & out) throw (String)
{
  String tmp;
  in >> tmp;
  out = string_to_float(tmp);
  return in;
}

String & operator << (String & out, double in)
{
  if (out.size()) out += ' ';
  out += float_to_string(in);
  return out;
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_bool()
{
  static const List<String> data = List<String>() <<
    "False" <<
    "True";

  return data;
}

//------------------------------------------------------------------------------

}
}

