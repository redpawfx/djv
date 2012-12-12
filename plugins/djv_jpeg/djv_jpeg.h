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

//! \file djv_jpeg.h

#ifndef DJV_JPEG_H
#define DJV_JPEG_H

#include <djv_string.h>

#include <stdio.h>
namespace libjpeg   //! \todo Windows conflicts?
{
extern "C" {
#include <jpeglib.h>
}
}
#include <setjmp.h>

//! \namespace djv_jpeg
//!
//! This plugin supports the Joint Photographic Experts Group (JPEG) image
//! file format.
//!
//! Requires:
//!
//! - libjpeg - http://www.ijg.org
//!
//! Supports:
//!
//! - Images: 8-bit, Luminance, RGB
//! - File compression

namespace djv_jpeg
{
using namespace djv;

static const String name = "JPEG";

static const List<String> extensions = List<String>() <<
    ".jpeg" <<
    ".jpg" <<
    ".jfif";

//! This struct provides libjpeg error handling.

struct Jpeg_Error
{
    struct libjpeg::jpeg_error_mgr pub;

    char msg [JMSG_LENGTH_MAX];

    jmp_buf jump;
};

extern "C" {

    void djv_jpeg_error(libjpeg::j_common_ptr);
    void djv_jpeg_warning(libjpeg::j_common_ptr, int);

}

} // djv_jpeg

#endif // DJV_JPEG_H

