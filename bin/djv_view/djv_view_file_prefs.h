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

//! \file djv_view_file_prefs.h

#ifndef DJV_VIEW_FILE_PREFS_H
#define DJV_VIEW_FILE_PREFS_H

#include <djv_view_cache.h>

#include <djv_widget.h>

#include <djv_pixel_data.h>

namespace djv_view
{

//------------------------------------------------------------------------------
//! \class File_Prefs
//!
//! This class provides file preferences.
//------------------------------------------------------------------------------

class File_Prefs : public Callback
{
public:

    //! Constructor.

    File_Prefs();

    //! Destructor.

    virtual ~File_Prefs();

    //! Add a recent file.

    void recent(const String &);

    //! Get the recent files.

    const List<String> & recent() const;

    //! This signal is emitted when the recent files are changed.

    Signal<const List<String> &> recent_signal;

    //! Set whether sequences are automatically opened.

    void seq_auto(bool);

    //! Get whether sequences are automatically opened.

    bool seq_auto() const;

    //! This signal is emitted when automatic sequences is changed.

    Signal<bool> seq_auto_signal;

    //! Set whether to combine command line arguments.

    void command_line_combine(bool);

    //! Get whether to combine command line arguments.

    bool command_line_combine() const;

    //! Set the proxy scale.

    void proxy(Pixel_Data_Info::PROXY);

    //! Get the proxy scale.

    Pixel_Data_Info::PROXY proxy() const;

    //! This signal is emitted when the proxy scale is changed.

    Signal<Pixel_Data_Info::PROXY> proxy_signal;
    
    //! Set whether images are converted to 8-bits.
    
    void u8_conversion(bool);
    
    //! Get whther images are converted to 8-bits.
    
    bool u8_conversion() const;

    //! This signal is emitted when 8-bit conversion is changed.

    Signal<bool> u8_conversion_signal;

    //! Set whether the cache is enabled.

    void cache(bool);

    //! Set the cache size.

    void cache_size(int);

    //! Set the cache type.

    void cache_type(Cache::CACHE);

    //! Set the cache display.

    void cache_display(bool);

    //! Get whether the cache is enabled.

    bool cache() const;

    //! Get the cache size.

    int cache_size() const;

    //! Get the cache type.

    Cache::CACHE cache_type() const;

    //! Get the cache display.

    bool cache_display() const;

    //! This signal is emitted when the cache is changed.

    Signal<bool> cache_signal;

    //! Get the global preferences.

    static File_Prefs * global();

private:

    List<String>           _recent;
    bool                   _seq_auto;
    bool                   _command_line_combine;
    Pixel_Data_Info::PROXY _proxy;
    bool                   _u8_conversion;
    bool                   _cache;
    int                    _cache_size;
    Cache::CACHE           _cache_type;
    bool                   _cache_display;
};

//------------------------------------------------------------------------------
//! \class File_Prefs_Widget
//!
//! This class provides a file preferences widget.
//------------------------------------------------------------------------------

class File_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    File_Prefs_Widget();

private:

    DJV_CALLBACK(File_Prefs_Widget, seq_auto_callback, bool);
    DJV_CALLBACK(File_Prefs_Widget, command_line_combine_callback, bool);
    DJV_CALLBACK(File_Prefs_Widget, proxy_callback, int);
    DJV_CALLBACK(File_Prefs_Widget, u8_conversion_callback, bool);
    DJV_CALLBACK(File_Prefs_Widget, cache_callback, bool);
    DJV_CALLBACK(File_Prefs_Widget, cache_size_callback, int);
    DJV_CALLBACK(File_Prefs_Widget, cache_type_callback, int);
    DJV_CALLBACK(File_Prefs_Widget, cache_display_callback, bool);
};

} // djv_view

#endif // DJV_VIEW_FILE_PREFS_H

