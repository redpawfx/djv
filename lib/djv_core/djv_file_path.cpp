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

//! \file djv_file_path.cpp

#include <djv_file.h>

#include <FL/filename.H>

namespace djv
{

//------------------------------------------------------------------------------
// File_Util
//------------------------------------------------------------------------------

String File_Util::path_absolute(const String & in)
{
    //DJV_DEBUG("File_Util::path_absolute");
    //DJV_DEBUG_PRINT("in = " << in);

    char * p = 0;

    char tmp [cstring_size] = "";
    fl_filename_absolute(tmp, cstring_size, in.c_str());
    p = tmp;

    //! \todo Is this still necessary?

#if defined(DJV_WINDOWS)

    TCHAR tmp2 [4096];
    GetFullPathName(p, 4096, tmp2, 0);
    p = tmp2;

#endif // DJV_WINDOWS

    String out(p);
    //DJV_DEBUG_PRINT("out = " << out);
    return out;
}

bool File_Util::path_is_absolute(const String & in)
{
    if (! in.size())
    {
        return false;
    }

    //! \todo Windows drive letter.

    if (1 == in.find(':', 0))
    {
        return true;
    }

    return List_Util::find(in[0], path_separators) != -1;
}

String File_Util::path_fix(const String & in)
{
    //DJV_DEBUG("File_Util::path_fix");
    //DJV_DEBUG_PRINT("in = " << in);

    // Let FLTK do it's stuff.

    char tmp [cstring_size] = "";
    char path [cstring_size] = "";
    fl_filename_expand(tmp, cstring_size, in.c_str());
    fl_filename_absolute(path, cstring_size, tmp);

    //! \todo Keep original path separators?

    //for (char * p = path; *p && (p - path) < cstring_size; ++p)
    //  if (path_separators[0] == *p)
    //    *p = path_separator[0];

    //DJV_DEBUG_PRINT("path = " << path);

    // Split path into pieces.

    List<String> list = String_Util::split(path, path_separators);

    //! \todo Keep extra root piece for Windows.

    String root;

    if (
        (path_separators[0] == path[0] && path_separators[0] == path[1]) ||
        (path_separators[1] == path[0] && path_separators[1] == path[1]))
    {
        root = path_separator;
    }
    else if (list.size())
    {
        if (list[0].find(':', 0) != String::npos)
        {
            root = list[0];
            list.erase(list.begin());
        }
    }

    // Remove extra pieces.

    List<String> list_tmp;

    for (List<String>::const_iterator i = list.begin(); i < list.end(); ++i)
        if (".." == *i && list_tmp.size())
        {
            if (list_tmp[list_tmp.size() - 1] != "..")
            {
                list_tmp.erase(list_tmp.end() - 1);
            }
            else
            {
                list_tmp += *i;
            }
        }
        else if ("." == *i && list_tmp.size()) ;
        else
        {
            list_tmp += *i;
        }

    // Combine pieces.

    String out =
        root + path_separator + String_Util::join(list_tmp, path_separator);

    if (list_tmp.size())
    {
        out += path_separator;
    }

    //DJV_DEBUG_PRINT("out = " << out);
    return out;
}

/*String path_posix(const String & in)
{
  String out = in;
  for (size_t i = 0; i < out.size(); ++i)
    if (path_separators[1] == out[i])
      out[i] = path_separators[0];
  return out;
}*/

} // djv

