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

//! \file djv_file_browser_prefs.h

#ifndef DJV_FILE_BROWSER_PREFS_H
#define DJV_FILE_BROWSER_PREFS_H

#include <djv_file_browser.h>
#include <djv_prefs.h>
#include <djv_shortcut.h>
#include <djv_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class File_Browser_Prefs
//!
//! This class provides file browser preferences.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT File_Browser_Prefs
{
public:

    //! Constructor.

    File_Browser_Prefs();

    //! Destructor.

    ~File_Browser_Prefs();

    //! Set the recent files.

    void recent(const List<String> &);

    //! Add a recent file.

    void recent_add(const String &);

    //! Get the recent files.

    const List<String> & recent() const;

    //! Set the bookmarks.

    void bookmark(const List<String> &);

    //! Add a bookmark.

    void bookmark_add(const String &);

    //! Get the bookmarks.

    const List<String> & bookmark() const;

    //! Set the navigation.

    void navigate(File_Browser::NAVIGATE);

    //! Get the navigation.

    File_Browser::NAVIGATE navigate() const;

    //! Shortcuts.

    enum SHORTCUT
    {
        UP,
        PREV,
        CURRENT,
        HOME,
        DESKTOP,
        ROOT,
        RELOAD,
        SEQ_OFF,
        SEQ_SPARSE,
        SEQ_RANGE,
        HIDDEN,
        SORT_NAME,
        SORT_SIZE,
#   if ! defined(DJV_WINDOWS)
        SORT_USER,
#   endif
        SORT_TIME,
        SORT_REVERSE,
        SORT_DIRECTORY,
        BOOKMARK_ADD,
        BOOKMARK_1,
        BOOKMARK_2,
        BOOKMARK_3,
        BOOKMARK_4,
        BOOKMARK_5,
        BOOKMARK_6,
        BOOKMARK_7,
        BOOKMARK_8,
        BOOKMARK_9,
        BOOKMARK_10,
        BOOKMARK_11,
        BOOKMARK_12,

        _SHORTCUT_SIZE
    };

    //! Get the shortcut labels.

    static const List<String> & label_shortcut();

    //! Set the shortcuts.

    void shortcut(const List<Shortcut> &);

    //! Get the shortcuts.

    const List<Shortcut> & shortcut() const;

    //! Get the global preferences.

    static File_Browser_Prefs * global();

private:

    List<String>           _recent;
    List<String>           _bookmark;
    File_Browser::NAVIGATE _navigate;
    List<Shortcut>         _shortcut;
};

//------------------------------------------------------------------------------
//! \class File_Browser_Prefs_Instance
//!
//! This class provides per-instance file browser preferences.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT File_Browser_Prefs_Instance
{
public:

    //! Constructor.

    File_Browser_Prefs_Instance(Prefs *, const String & prefs_name);

    //! Destructor.

    ~File_Browser_Prefs_Instance();

    //! Set image thumbnails.

    void image(File_Browser::IMAGE);

    //! Get image thumbnails.

    File_Browser::IMAGE image() const;

    //! Set file sequencing.

    void seq(Seq::COMPRESS);

    //! Get file sequencing.

    Seq::COMPRESS seq() const;

    //! Set the file type filter.

    void type(int);

    //! Get the file type filter.

    int type() const;

    //! Set whether hidden files are shown.

    void hidden(bool);

    //! Get whether hidden files are shown.

    bool hidden() const;

    //! Set sorting.

    void sort(File_Browser::SORT);

    //! Set whether sorting is reversed.

    void sort_reverse(bool);

    //! Set whether directories are sorted first.

    void sort_directory(bool);

    //! Get sorting.

    File_Browser::SORT sort() const;

    //! Get whether sorting is reversed.

    bool sort_reverse() const;

    //! Get whether directories are sorted first.

    bool sort_directory() const;

    //! Set whether the dialog is pinned.

    void pin(bool);

    //! Get whether the dialog is pinned.

    bool pin() const;

private:

    Prefs *             _prefs;
    String              _prefs_name;
    File_Browser::IMAGE _image;
    Seq::COMPRESS       _seq;
    int                 _type;
    bool                _hidden;
    File_Browser::SORT  _sort;
    bool                _sort_reverse;
    bool                _sort_directory;
    bool                _pin;
};

//------------------------------------------------------------------------------
//! \class File_Browser_Prefs_Widget
//!
//! This class provides a file browser preferences widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT File_Browser_Prefs_Widget : public Widget
{
public:

    //! Constructor.

    File_Browser_Prefs_Widget();

private:

    DJV_CALLBACK(File_Browser_Prefs_Widget, shortcut_callback,
                 const List<Shortcut> &);
};

} // djv

#endif // DJV_FILE_BROWSER_PREFS_H

