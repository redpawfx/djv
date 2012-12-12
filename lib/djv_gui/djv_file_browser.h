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

//! \file djv_file_browser.h

#ifndef DJV_FILE_BROWSER_H
#define DJV_FILE_BROWSER_H

#include <djv_dialog.h>

#include <djv_file.h>

namespace djv
{

class Check_Button;
class Choice_Widget;
class Columns;
class File_Browser_Browser;
class File_Browser_Prefs_Instance;
class Frame;
class Menu;
class Text_Edit;
class Tool_Button;

//------------------------------------------------------------------------------
//! \class File_Browser
//!
//! This class provides a file browser dialog.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT File_Browser : public Dialog
{
public:

    //! Constructor.

    File_Browser(File_Browser_Prefs_Instance * = 0);

    //! Destructor.

    virtual ~File_Browser();

    void del();

    //! Set the file.

    void set(const File &);

    //! Get the file.

    const File & get() const;

    //! This signal is emitted when the file is changed.

    Signal<const File &> signal;

    //! Image thumbnails.

    enum IMAGE
    {
        IMAGE_OFF,
        IMAGE_LOW,
        IMAGE_HIGH,

        _IMAGE_SIZE
    };

    //! Get the image thumbnail labels.

    static const List<String> & label_image();

    //! Set the image thumbnails.

    void image(IMAGE);

    //! Get the image thumbnails.

    IMAGE image() const;

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

    //! Sorting.

    enum SORT
    {
        SORT_NAME,
        SORT_TYPE,
        SORT_SIZE,
# if ! defined(DJV_WINDOWS)
        SORT_USER,
# endif
        SORT_TIME,

        _SORT_SIZE
    };

    //! Get the sorting labels.

    static const List<String> & label_sort();

    //! Set sorting.

    void sort(SORT);

    //! Set whether sorting is reversed.

    void sort_reverse(bool);

    //! Set whether directories are sorted first.

    void sort_directory(bool);

    //! Get sorting.

    SORT sort() const;

    //! Get whether sorting is reversed.

    bool sort_reverse() const;

    //! Get whether directories are sorted first.

    bool sort_directory() const;

    //! Navigation.

    enum NAVIGATE
    {
        NAVIGATE_SINGLE_CLICK,
        NAVIGATE_DOUBLE_CLICK,

        _NAVIGATE_SIZE
    };

    //! Get navigation labels.

    static const List<String> & label_navigate();

    //! Set whether the dialog is pinned.

    void pin(bool);

    //! Get whether the dialog is pinned.

    bool pin() const;

    virtual void show();

    virtual void dirty();

    //! Show the global file browser dialog.

    static void set(Callback *, Signal<const File &>::Callback_Fnc *);

    //! Remove the global file browser dialog.

    static void del(Callback *);

    //! Get the global file browser.

    static File_Browser * global();

private:

    // Callbacks.

    DJV_CALLBACK(File_Browser, value_callback, const String &);
    DJV_CALLBACK(File_Browser, value_action_callback, const String &);
    DJV_CALLBACK(File_Browser, value_change_callback, const String &);
    DJV_CALLBACK(File_Browser, browser_callback, const File &);
    DJV_CALLBACK(File_Browser, browser_mouse_callback, const File &);
    DJV_CALLBACK(File_Browser, browser_action_callback, const File &);
    DJV_CALLBACK(File_Browser, columns_callback, const List<int> &);
    DJV_CALLBACK(File_Browser, columns_action_callback, int);

    DJV_CALLBACK(File_Browser, up_callback, bool);
    DJV_CALLBACK(File_Browser, prev_callback, bool);
    DJV_CALLBACK(File_Browser, current_callback, bool);
# if ! defined(DJV_WINDOWS)
    DJV_CALLBACK(File_Browser, home_callback, bool);
# endif
    DJV_CALLBACK(File_Browser, desktop_callback, bool);
# if defined(DJV_WINDOWS)
    DJV_CALLBACK(File_Browser, drive_callback, bool);
# else
    DJV_CALLBACK(File_Browser, root_callback, bool);
# endif
    DJV_CALLBACK(File_Browser, reload_callback, bool);
    DJV_CALLBACK(File_Browser, seq_callback, int);

    DJV_FL_WIDGET_CALLBACK(File_Browser, _up_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _prev_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _recent_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _current_callback);
# if ! defined(DJV_WINDOWS)
    DJV_FL_WIDGET_CALLBACK(File_Browser, _home_callback);
# endif
    DJV_FL_WIDGET_CALLBACK(File_Browser, _desktop_callback);
# if defined(DJV_WINDOWS)
    DJV_FL_WIDGET_CALLBACK(File_Browser, _drive_callback);
# else
    DJV_FL_WIDGET_CALLBACK(File_Browser, _root_callback);
# endif
    DJV_FL_WIDGET_CALLBACK(File_Browser, _reload_callback);

    DJV_FL_WIDGET_CALLBACK(File_Browser, _image_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _seq_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _type_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _hidden_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _sort_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _sort_reverse_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _sort_directory_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _navigate_callback);

    DJV_CALLBACK(File_Browser, bookmark_del_callback, const List<int> &);
    DJV_CALLBACK(File_Browser, bookmark_del_all_callback, bool);

    DJV_FL_WIDGET_CALLBACK(File_Browser, _bookmark_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _bookmark_add_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _bookmark_del_callback);
    DJV_FL_WIDGET_CALLBACK(File_Browser, _bookmark_del_all_callback);

    DJV_CALLBACK(File_Browser, pin_callback, bool);
    DJV_CALLBACK(File_Browser, ok_callback, bool);
    DJV_CALLBACK(File_Browser, close_callback, bool);

    // Update.

    void directory_update();
    void widget_update();
    void type_update();
    void menu_update();

    // Variables.

    std::auto_ptr<File_Browser_Prefs_Instance> _prefs;

    File                   _value;
    String                 _directory_prev;
    IMAGE                  _image;
    Seq::COMPRESS          _seq;
    bool                   _hidden;
    SORT                   _sort;
    bool                   _sort_reverse;
    bool                   _sort_directory;
    int                    _type;
    List<String>           _type_glob;
    List<String>           _type_label;
    bool                   _pin;
    Menu *                 _menu;
    List<int>              _menu_recent;
    List<int>              _menu_image;
    List<int>              _menu_seq;
    List<int>              _menu_type;
    List<int>              _menu_sort;
    List<int>              _menu_navigate;
    List<int>              _menu_bookmark;
# if defined(DJV_WINDOWS)
    List<int>              _menu_drive;
# endif
    Text_Edit *            _value_widget;
    Tool_Button *          _up_widget;
    Tool_Button *          _prev_widget;
    Tool_Button *          _reload_widget;
    Choice_Widget *        _seq_widget;
    File_Browser_Browser * _browser;
    Columns *              _columns;
    Check_Button *         _pin_widget;
    Frame *                _browser_frame;
};

//------------------------------------------------------------------------------

DJV_GUI_EXPORT String & operator >> (String &, File_Browser::IMAGE &)
    throw (String);
DJV_GUI_EXPORT String & operator >> (String &, File_Browser::SORT &)
    throw (String);
DJV_GUI_EXPORT String & operator >> (String &, File_Browser::NAVIGATE &)
    throw (String);

DJV_GUI_EXPORT String & operator << (String &, File_Browser::IMAGE);
DJV_GUI_EXPORT String & operator << (String &, File_Browser::SORT);
DJV_GUI_EXPORT String & operator << (String &, File_Browser::NAVIGATE);

} // djv

#endif // DJV_FILE_BROWSER_H

