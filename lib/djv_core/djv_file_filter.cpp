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

//! \file djv_file_filter.cpp

#include <djv_file.h>

#include <FL/filename.H>

namespace djv
{

//------------------------------------------------------------------------------
// File_Util::filter
//------------------------------------------------------------------------------

namespace
{

inline bool path_is_relative(const String & in)
{
    if ("." == in)
        ;
    else if (".." == in)
        ;
    else if (in.size() >= 2 && '.' == in[0] && ('/' == in[1] || '\\' == in[1]))
        ;
    else if (in.size() >= 3 && '.' == in[0] && '.' == in[1] &&
        ('/' == in[2] || '\\' == in[2])) ;
    else
    {
        return false;
    }

    return true;
}

} // namespace

void File_Util::filter(
    List<File> & in,
    int filter,
    const List<String> & glob,
    bool glob_files_only)
{
    //DJV_DEBUG("File_Util::filter");
    //DJV_DEBUG_PRINT("filter = " << filter);
    //DJV_DEBUG_PRINT("glob = " << glob);

    size_t i = 0;

    for (size_t j = 0; j < in.size(); ++j)
    {
        const String name = in[j].get(-1, false);

        //DJV_DEBUG_PRINT("name = " << name);

        bool valid = true;

        const File::TYPE type = in[j].type();

        if ((filter & FILTER_FILE) && (File::FILE == type || File::SEQ == type))
        {
            valid = false;
        }

        if ((filter & FILTER_DIRECTORY) && File::DIRECTORY == type)
        {
            valid = false;
        }

        if ((filter & FILTER_HIDDEN) && '.' == name[0] &&
            ! path_is_relative(name))
        {
            valid = false;
        }

        if (
            glob.size() &&
            ((glob_files_only && type != File::DIRECTORY) || ! glob_files_only))
        {
            size_t k = 0;

            for (; k < glob.size(); ++k)
            {
                if (fl_filename_match(name.c_str(), glob[k].c_str()))
                {
                    break;
                }
            }

            if (glob.size() == k)
            {
                valid = false;
            }
        }

        //DJV_DEBUG_PRINT("valid = " << valid);

        if (valid)
        {
            if (i != j)
            {
                in[i] = in[j];
            }

            ++i;
        }
    }

    //DJV_DEBUG_PRINT("size = " << i);

    in.resize(i);
}

} // djv

