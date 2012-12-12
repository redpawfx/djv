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

//! \file djv_libquicktime.h

#ifndef DJV_LIBQUICKTIME_H
#define DJV_LIBQUICKTIME_H

#include <djv_string.h>

#include <lqt/colormodels.h>
#include <lqt/lqt.h>

//!\namespace djv_libquicktime
//
//! This plugin supports libquicktime, an open source library for reading and
//! writing movies.
//!
//! Requires:
//!
//! - libquicktime - http://libquicktime.sourceforge.net
//!
//! Supports:
//!
//! - Images: 8-bit RGBA
//! - File compression

namespace djv_libquicktime
{
using namespace djv;

static const String name = "libquicktime";

static const List<String> extensions = List<String>() <<
    ".qt"  <<
    ".mov" <<
    ".avi" <<
    ".mp4";

//! Initialize libquicktime.

void quicktime_init();

//! Get codec labels.

List<String> label_codec();

//! Get codec text labels.

List<String> label_codec_text();

} // djv_libquicktime

#endif // DJV_LIBQUICKTIME_H

