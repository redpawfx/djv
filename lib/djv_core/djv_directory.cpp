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

//! \file djv_directory.cpp

#include <djv_directory.h>

#include <djv_file.h>

#include <FL/filename.H>

#if defined(DJV_WINDOWS)
#include <shlobj.h>
#include <windows.h>
#endif // DJV_WINDOWS

#include <stdlib.h>

namespace djv
{

//------------------------------------------------------------------------------
// Directory
//------------------------------------------------------------------------------

Directory::Directory() :
    _is_valid(false)
{}

Directory::Directory(const String & in) :
    _is_valid(false)
{
    set(in);
}

void Directory::set(const String & in)
{
    //DJV_DEBUG("Directory::set");
    //DJV_DEBUG_PRINT("in = " << in);

    _path = File_Util::path_fix(in);

    //DJV_DEBUG_PRINT("path = " << _path);

    update();
}

const String & Directory::get() const
{
    return _path;
}

List<File> Directory::list() const
{
    //DJV_DEBUG("Directory::list");
    //DJV_DEBUG_PRINT("path = " << _path);

    List<File> out;

    // Read directory entries.

    struct Dirent_List
    {
        Dirent_List() :
            p(0),
            size(0)
        {}

        ~Dirent_List()
        {
            if (p)
            {
                for (int i = 0; i < size; ++i)
                {
                    free((void *)p[i]);
                }

                free((void *)p);
            }
        }

        dirent ** p;
        int size;
    };

    Dirent_List list;
    
    list.size = fl_filename_list(_path.c_str(), &list.p, fl_alphasort);

    //DJV_DEBUG_PRINT("size = " << list.size);
    if (-1 == list.size)
    {
        return out;
    }

    // Convert to our format.

    out.resize(list.size);

    int j = 0;

    for (int i = 0; i < list.size; ++i)
    {
        const char * name = list.p[i]->d_name;

        // Go to the end of the string.

        const char * p = name;

        for (; *p; ++p)
            ;

        // Move back one if there is a trailing path separator.

        if (p - name)
            if ('/' == *(p - 1) || '\\' == *(p - 1))
            {
                --p;
            }

        // Ignore the "." and ".." directories.

        if (1 == (p - name))
        {
            if ('.' == name[0])
            {
                name = 0;
            }
        }
        else if (2 == (p - name))
        {
            if ('.' == name[0] && '.' == name[1])
            {
                name = 0;
            }
        }

        // Convert.

        if (name)
        {
            //DJV_DEBUG_PRINT("file = " << name);

            out[j++] = File(_path + String(name, p - name));
        }
    }

    if (j < list.size)
    {
        out.resize(j);
    }

    //DJV_DEBUG_PRINT("size = " << out.size());

    return out;
}

void Directory::cd(const String & in)
{
    //DJV_DEBUG("Directory::cd");
    //DJV_DEBUG_PRINT("in = " << in);

    _path = File_Util::path_fix(_path + in);

    update();
}

void Directory::up()
{
    cd("..");
}

bool Directory::is_valid() const
{
    return _is_valid;
}

String Directory::home()
{
    String out;

#if ! defined(DJV_WINDOWS)

    char tmp [cstring_size] = "";
    fl_filename_expand(tmp, cstring_size, "~");
    out = tmp;

#endif // DJV_WINDOWS

    return File_Util::path_fix(out);
}

String Directory::desktop()
{
    String out;

#if defined(DJV_WINDOWS)

    //! \todo TCHAR conversion?

    TCHAR tmp [MAX_PATH] = "";
    ::SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, tmp);
    out = tmp;

#else // DJV_WINDOWS

    //! \todo Is this still necessary?

    out = home() + "/Desktop";

#endif // DJV_WINDOWS

    return File_Util::path_fix(out);
}

List<String> Directory::drives()
{
    //DJV_DEBUG("Directory::drives");

    List<String> out;

#if defined(DJV_WINDOWS)

    //! \todo TCHAR conversion?

    TCHAR path [MAX_PATH] = "";
    const DWORD size = ::GetLogicalDriveStrings(MAX_PATH - 1, path);

    for (DWORD i = 0, j = 0; i < size; ++i)
        if (! path[i])
        {
            out += file::path_fix(String(path + j, i - j));

            j = i + 1;
        }

#endif // DJV_WINDOWS

    //DJV_DEBUG_PRINT("out = " << out);

    return out;
}

String Directory::root()
{
    String out;

#if ! defined(DJV_WINDOWS)

    out = String(File_Util::path_separator, 1);

#endif // ! DJV_WINDOWS

    return File_Util::path_fix(out);
}

void Directory::update()
{
    dirent ** dirent_list = 0;

    const int size =
        fl_filename_list(get().c_str(), &dirent_list, fl_alphasort);

    _is_valid = size >= 0;

    for (int i = 0; i < size; ++i)
    {
        ::free((void *)dirent_list[i]);
    }

    if (dirent_list)
    {
        ::free((void *)dirent_list);
    }
}

} // djv

