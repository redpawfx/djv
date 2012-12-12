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

//! \file djv_view_file_save.h

#ifndef DJV_VIEW_FILE_SAVE_H
#define DJV_VIEW_FILE_SAVE_H

#include <djv_callback.h>

#include <djv_file.h>
#include <djv_gl_image.h>
#include <djv_image_io.h>
#include <djv_time.h>

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \struct File_Save_Info
//!
//! This struct provides file save information.
//------------------------------------------------------------------------------

struct File_Save_Info
{
    //! Constructor.

    File_Save_Info(
        const djv::File &            input_file    = djv::File(),
        const djv::File &            output_file   = djv::File(),
        const Pixel_Data_Info &      info          = Pixel_Data_Info(),
        const Seq &                  seq           = Seq(),
        int                          layer         = 0,
        const Pixel_Data_Info::PROXY proxy         = Pixel_Data_Info::PROXY_NONE,
        bool                         u8_conversion = false,
        bool                         color_profile = true,
        const Gl_Image_Options &     options       = Gl_Image_Options());

    djv::File              input_file;
    djv::File              output_file;
    Pixel_Data_Info        info;
    Seq                    seq;
    int                    layer;
    Pixel_Data_Info::PROXY proxy;
    bool                   u8_conversion;
    bool                   color_profile;
    Image_Io_Frame_Info    frame_info;
    Gl_Image_Options       options;
};

//------------------------------------------------------------------------------
//! \class File_Save
//!
//! This class provides file saving.
//------------------------------------------------------------------------------

class File_Save : public Callback
{
public:

    //! Constructor.

    virtual ~File_Save();

    //! Save a file.

    void set(const File_Save_Info &);

    //! Get the global file save.

    static File_Save * global();

private:

    struct _File_Save;

    void del(_File_Save *);

    List<_File_Save *> _list;
};

} // djv_view

#endif // DJV_VIEW_FILE_SAVE_H

