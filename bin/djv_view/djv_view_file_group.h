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

//! \file djv_view_file_group.h

#ifndef DJV_VIEW_FILE_GROUP_H
#define DJV_VIEW_FILE_GROUP_H

#include <djv_view_cache.h>

#include <djv_row_layout.h>
#include <djv_menu.h>

#include <djv_file.h>
#include <djv_image_io.h>

namespace djv
{

class File_Browser;
class Menu;
class Tool_Button;

} // djv

namespace djv_view
{

//------------------------------------------------------------------------------
//! \class File_Group
//!
//! This class provides the file group.
//------------------------------------------------------------------------------

class File_Group : public Callback
{
public:

    //! Constructor.

    File_Group(
        Menu *                menu,
        Abstract_Row_Layout * toolbar,
        const File_Group *    copy    = 0);

    //! Destructor.

    virtual ~File_Group();

    void del();

    //! Open a file.

    void open(const File &);

    //! Get an image.

    const djv::Image * get(int64_t frame) const;

    //! Get the file.

    const File & file() const;

    //! Get the image I/O information.

    const Image_Io_Info & info() const;

    //! Set whether to automatically open sequences.

    void seq_auto(bool);

    //! Get whether to automatically open sequences.

    bool seq_auto() const;

    //! Set the layer to load.

    void layer(int);

    //! Get the layer to load.

    int layer() const;

    //! Set the proxy scale.

    void proxy(Pixel_Data_Info::PROXY);

    //! Get the proxy scale.

    Pixel_Data_Info::PROXY proxy() const;
    
    //! Set whether images are converted to 8-bits.
    
    void u8_conversion(bool);
    
    //! Get whther images are converted to 8-bits.
    
    bool u8_conversion() const;

    //! Set whether the cache is enabled.

    void cache(bool);

    //! Get whether the cache is enabled.

    bool cache() const;

    //! This signal is emitted when a file is opened.

    Signal<const File &> open_signal;

    //! This signal is emitted when the file is reloaded.

    Signal<bool> reload_signal;

    //! This signal is emitted when the file is saved.

    Signal<const File &> save_signal;

    //! This signal is emitted when a frame is saved.

    Signal<const File &> save_frame_signal;

    //! This signal is emitted when the cache size is changed.

    Signal<const List<int64_t> &> cache_signal;

    //! This signal is emitted when the image needs to be updated.

    Signal<bool> image_signal;

    //! This signal is emitted when the frame store is changed.

    Signal<bool> image_frame_store_signal;

    //! This signal is emitted when an update is needed.

    Signal<bool> update_signal;

    //! Update menu items.

    void menu_update(Menu_Item_Group *, bool seq);

    //! Update toolbar.

    void toolbar_update();

private:

    void cache_del();

    // Callbacks.

    DJV_CALLBACK(File_Group, open_callback, const File &);
    DJV_CALLBACK(File_Group, open_callback, bool);
    DJV_CALLBACK(File_Group, recent_callback, bool);
    DJV_CALLBACK(File_Group, recent_callback, const List<String> &);
    DJV_CALLBACK(File_Group, reload_callback, bool);
    DJV_CALLBACK(File_Group, reload_callback, Image_Io_Base *);
    DJV_CALLBACK(File_Group, close_callback, bool);
    DJV_CALLBACK(File_Group, save_callback, const File &);
    DJV_CALLBACK(File_Group, save_check_callback, bool);
    DJV_CALLBACK(File_Group, save_frame_callback, const File &);
    DJV_CALLBACK(File_Group, save_frame_check_callback, bool);
    DJV_CALLBACK(File_Group, seq_auto_callback, bool);
    DJV_CALLBACK(File_Group, layer_callback, int);
    DJV_CALLBACK(File_Group, proxy_callback, Pixel_Data_Info::PROXY);
    DJV_CALLBACK(File_Group, u8_conversion_callback, bool);
    DJV_CALLBACK(File_Group, cache_callback, bool);
    DJV_CALLBACK(File_Group, cache_clear_callback, bool);
    DJV_CALLBACK(File_Group, cache_update_callback, bool);

    DJV_FL_WIDGET_CALLBACK(File_Group, _open_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _recent_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _reload_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _close_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _save_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _save_frame_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _seq_auto_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _layer_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _layer_prev_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _layer_next_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _proxy_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _u8_conversion_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _cache_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _cache_clear_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _messages_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _prefs_callback);
    DJV_FL_WIDGET_CALLBACK(File_Group, _exit_callback);

    // Variables.

    File                      _file;
    File                      _file_save;
    Image_Io_Info             _info;
    std::auto_ptr<Image_Load> _load;
    const djv::Image *        _image;
    djv::Image                _image_tmp;
    djv::Image                _image_tmp2;
    bool                      _seq_auto;
    int                       _layer;
    List<String>              _layers;
    Pixel_Data_Info::PROXY    _proxy;
    bool                      _u8_conversion;
    bool                      _cache;
    Cache_Ref *               _cache_ref;
    Menu *                    _menu;
    List<int>                 _menu_recent;
    List<int>                 _menu_layer;
    List<int>                 _menu_proxy;
    Tool_Button *             _open_widget;
    Tool_Button *             _reload_widget;
    Tool_Button *             _close_widget;
    File_Browser *            _open_dialog;
    File_Browser *            _save_dialog;
};

} // djv_view

#endif // DJV_VIEW_FILE_GROUP_H

