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

//! \file djv_file_test.cpp

#include <djv_assert.h>
#include <djv_file.h>

using namespace djv;

void split()
{
    DJV_DEBUG("split");

    const struct Data
    {
        String in, path, base, number, extension;
    } data [] =
    {
        { "/tmp/image.1-100.tiff", "/tmp/", "image.", "1-100", ".tiff" },
        { "image.1-100.tiff", "", "image.", "1-100", ".tiff" },
        { "image.001-100.tiff", "", "image.", "001-100", ".tiff" },
        { "image1-100.tiff", "", "image", "1-100", ".tiff" },
        { "image001-100.tiff", "", "image", "001-100", ".tiff" },
        { "image_01-100.tiff", "", "image_01-", "100", ".tiff" },
        { "1-100.tiff", "", "", "1-100", ".tiff" },
        { "-1.tiff", "", "", "-1", ".tiff" },
        { "1-100", "", "", "1-100", "" },
        { "1", "", "", "1", "" },
        { ".hidden", "", ".hidden", "", "" },
        { "/", "/", "", "", "" },
        { "//", "//", "", "", "" },
        { "/.", "/", "", "", "." },
        { "./", "./", "", "", "" },
        { "C:\\Documents and Settings\\darby\\Desktop\\movie.mov",
          "C:\\Documents and Settings\\darby\\Desktop\\", "movie", "", ".mov"}
    };
    const int data_size = sizeof(data) / sizeof(Data);

    for (int i = 0; i < data_size; ++i)
    {
        const File tmp(data[i].in);
        DJV_DEBUG_PRINT(data[i].in << " = " <<
                        String_Format("'%%' '%%' '%%' '%%'").
                        arg(tmp.path()).
                        arg(tmp.base()).
                        arg(tmp.number()).
                        arg(tmp.extension()));
        DJV_ASSERT(
            tmp.path() == data[i].path &&
            tmp.base() == data[i].base &&
            tmp.number() == data[i].number &&
            tmp.extension() == data[i].extension);
    }
}

void absolute()
{
    DJV_DEBUG("absolute");

    const struct Data
    {
        String path;
    } data [] =
    {
        { "." },
        { ".." },
        { "/" }
    };
    const int data_size = sizeof(data) / sizeof(Data);

    for (int i = 0; i < data_size; ++i)
    {
        const String tmp = File_Util::path_absolute(data[i].path);

        DJV_DEBUG_PRINT(data[i].path << " = " << tmp);
    }
}

void fix()
{
    DJV_DEBUG("fix");

    const struct Data
    {
        String path;
    } data [] =
    {
        { File_Util::path_absolute(".") },
        { "/Documents and Settings/darby/Desktop" }
    };
    const int data_size = sizeof(data) / sizeof(Data);

    for (int i = 0; i < data_size; ++i)
    {
        DJV_DEBUG_PRINT(
            data[i].path << " = " << File_Util::path_fix(data[i].path));
    }
}

int main(int argc, char ** argv)
{
    split();
    absolute();
    fix();

    return 0;
}

