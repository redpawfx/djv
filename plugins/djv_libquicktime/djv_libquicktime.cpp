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

//! \file djv_libquicktime.cpp

#include <djv_libquicktime.h>

#include <djv_core_application.h>
#include <djv_debug.h>
#include <djv_system.h>

namespace djv_libquicktime
{

extern "C" {

void _log_callback(lqt_log_level_t, const char *, const char *, void *)
{}

}

void quicktime_init()
{
    //DJV_DEBUG("quicktime_init");

#if defined(DJV_THIRD_PARTY_LIBQUICKTIME)

    const String plugin_path = DJV_CORE_APP->path() + "../lib/libquicktime";

    //DJV_DEBUG_PRINT("plugin path = " << plugin_path);

    System::env("LIBQUICKTIME_PLUGIN_DIR", plugin_path);

#endif // DJV_THIRD_PARTY_LIBQUICKTIME

    lqt_set_log_callback(_log_callback, 0);

    //DJV_DEBUG_PRINT("codec = " << label_codec());
}

List<String> label_codec()
{
    List<String> out;

    lqt_codec_info_t ** info = lqt_query_registry(0, 1, 1, 0);

    if (info)
    {
        for (int i = 0; info[i]; ++i)
        {
            out += info[i]->name;
        }

        lqt_destroy_codec_info(info);
    }

    return out;
}

List<String> label_codec_text()
{
    List<String> out;

    lqt_codec_info_t ** info = lqt_query_registry(0, 1, 1, 0);

    if (info)
    {
        for (int i = 0; info[i]; ++i)
        {
            out += info[i]->long_name;
        }

        lqt_destroy_codec_info(info);
    }

    return out;
}

} // djv_libquicktime

