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

//! \file djv_view_file_group.cpp

#include <djv_view_file_group.h>

#include <djv_view_application.h>
#include <djv_view_file_prefs.h>
#include <djv_view_image_prefs.h>
#include <djv_view_shortcut.h>
#include <djv_view_shortcut_prefs.h>

#include <djv_application_message_dialog.h>
#include <djv_file_browser.h>
#include <djv_file_browser_prefs.h>
#include <djv_frame_group.h>
#include <djv_image_io.h>
#include <djv_image_io_widget.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_menu.h>
#include <djv_prefs.h>
#include <djv_prefs_dialog.h>
#include <djv_question_dialog.h>
#include <djv_tool_button.h>

#include <djv_directory.h>
#include <djv_file.h>
#include <djv_image_io.h>

#include <FL/Fl.H>

namespace djv_view
{

//------------------------------------------------------------------------------
// File_Group
//------------------------------------------------------------------------------

File_Group::File_Group(
    Menu *                menu,
    Abstract_Row_Layout * toolbar,
    const File_Group *    copy) :
    open_signal             (this),
    reload_signal           (this),
    save_signal             (this),
    save_frame_signal       (this),
    cache_signal            (this),
    image_signal            (this),
    image_frame_store_signal(this),
    update_signal           (this),
    _image                  (0),
    _seq_auto               (File_Prefs::global()->seq_auto()),
    _layer                  (0),
    _proxy                  (File_Prefs::global()->proxy()),
    _u8_conversion          (File_Prefs::global()->u8_conversion()),
    _cache                  (File_Prefs::global()->cache()),
    _cache_ref              (0),
    _menu                   (0),
    _open_widget            (0),
    _reload_widget          (0),
    _close_widget           (0),
    _open_dialog            (0),
    _save_dialog            (0)
{
    if (copy)
    {
        _file          = copy->_file;
        _seq_auto      = copy->_seq_auto;
        _layer         = copy->_layer;
        _proxy         = copy->_proxy;
        _u8_conversion = copy->_u8_conversion;
        _cache         = copy->_cache;
    }

    // Create widgets.

    _menu = menu;

    _open_widget = new Tool_Button("file_open");

    _reload_widget = new Tool_Button("file_reload");

    _close_widget = new Tool_Button("file_close");

    _open_dialog = new File_Browser(
        new File_Browser_Prefs_Instance(Prefs::prefs(), "file/open"));

    _save_dialog = new File_Browser(
        new File_Browser_Prefs_Instance(Prefs::prefs(), "file/save"));

    // Layout.

    toolbar->add(new Horizontal_Frame_Group(List<Frame *>() <<
        _open_widget <<
        _reload_widget <<
        _close_widget));

    // Initialize.

    if (copy)
    {
        open(_file);
    }

    // Callbacks.

    _open_widget->signal.set(this, open_callback);
    _reload_widget->signal.set(this, reload_callback);
    _close_widget->signal.set(this, close_callback);

    _open_dialog->signal.set(this, open_callback);
    //_save_dialog->signal.set(this, save_callback);

    File_Prefs::global()->recent_signal.set(this, recent_callback);
    File_Prefs::global()->seq_auto_signal.set(this, seq_auto_callback);
    File_Prefs::global()->proxy_signal.set(this, proxy_callback);
    File_Prefs::global()->u8_conversion_signal.set(
        this, u8_conversion_callback);
    File_Prefs::global()->cache_signal.set(this, cache_callback);

    Cache::global()->signal.set(this, cache_update_callback);

    Image_Load_Widget_Factory::global()->signal.set(this, reload_callback);
}

File_Group::~File_Group()
{}

void File_Group::del()
{
    Callback::del();

    _open_dialog->del();
    _save_dialog->del();

    // Cleanup.

    if (_cache_ref)
    {
        _cache_ref->ref_del();
    }

    cache_del();
}

namespace
{

const String
    menu_title = "File",
    menu_open = "&Open",
    menu_recent = "&Recent",
    menu_reload = "Re&load",
    menu_close = "Clos&e",
    menu_save = "&Save",
    menu_save_frame = "Save &Frame",
    menu_seq_auto = "&Auto Sequence",
    menu_layer = "La&yer",
    menu_layer_prev = "Layer Previous",
    menu_layer_next = "Layer Next",
    menu_proxy = "Pro&xy Scale",
    menu_u8_conversion = "&8-bit Conversion",
    menu_cache = "&Memory Cache",
    menu_cache_clear = "Clear Memory Cac&he",
    menu_messages = "Messa&ges",
    menu_prefs = "&Preferences",
    menu_exit = "E&xit";

} // namespace

void File_Group::menu_update(Menu_Item_Group * in, bool seq)
{
    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    in->add(menu_title, 0, 0, 0, Menu_Item::SUB_MENU);

    in->add(
        menu_open,
        shortcuts[Shortcut::FILE_OPEN].value,
        _open_callback,
        this);

    in->add(menu_recent, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_recent = in->add(
        File_Prefs::global()->recent(),
        List<int>(),
        _recent_callback,
        this);
    
    in->end();

    in->add(
        menu_reload,
        shortcuts[Shortcut::FILE_RELOAD].value,
        _reload_callback,
        this);

    in->add(
        menu_close,
        shortcuts[Shortcut::FILE_CLOSE].value,
        _close_callback,
        this);

    in->add(
        menu_save,
        shortcuts[Shortcut::FILE_SAVE].value,
        _save_callback,
        this,
        ! _load.get() ? Menu_Item::INACTIVE : 0);

    in->add(
        menu_save_frame,
        shortcuts[Shortcut::FILE_SAVE_FRAME].value,
        _save_frame_callback,
        this,
        ! _load.get() || ! seq ? Menu_Item::INACTIVE : 0);

    in->add(
        menu_seq_auto,
        0,
        _seq_auto_callback,
        this,
        Menu_Item::TOGGLE | Menu_Item::DIVIDER,
        _seq_auto);

    in->add(menu_layer, 0, 0, 0, Menu_Item::SUB_MENU);
 
    _menu_layer = in->add(
        _layers,
        List<int>() <<
        shortcuts[Shortcut::FILE_LAYER_DEFAULT].value <<
        shortcuts[Shortcut::FILE_LAYER_1].value <<
        shortcuts[Shortcut::FILE_LAYER_2].value <<
        shortcuts[Shortcut::FILE_LAYER_3].value <<
        shortcuts[Shortcut::FILE_LAYER_4].value <<
        shortcuts[Shortcut::FILE_LAYER_5].value <<
        shortcuts[Shortcut::FILE_LAYER_6].value <<
        shortcuts[Shortcut::FILE_LAYER_7].value <<
        shortcuts[Shortcut::FILE_LAYER_8].value <<
        shortcuts[Shortcut::FILE_LAYER_9].value <<
        shortcuts[Shortcut::FILE_LAYER_10].value,
        _layer_callback,
        this,
        Menu_Item::RADIO,
        _layer);
    
    in->end();

    in->add(
        menu_layer_prev,
        shortcuts[Shortcut::FILE_LAYER_PREV].value,
        _layer_prev_callback,
        this);

    in->add(
        menu_layer_next,
        shortcuts[Shortcut::FILE_LAYER_NEXT].value,
        _layer_next_callback,
        this);

    in->add(menu_proxy, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_proxy = in->add(
        Pixel_Data_Info::label_proxy(),
        List<int>() <<
        shortcuts[Shortcut::FILE_PROXY_NONE].value <<
        shortcuts[Shortcut::FILE_PROXY_1_2].value <<
        shortcuts[Shortcut::FILE_PROXY_1_4].value <<
        shortcuts[Shortcut::FILE_PROXY_1_8].value,
        _proxy_callback,
        this,
        Menu_Item::RADIO,
        _proxy);
    
    in->end();

    in->add(
        menu_u8_conversion,
        0,
        _u8_conversion_callback,
        this,
        Menu_Item::TOGGLE | Menu_Item::DIVIDER,
        _u8_conversion);
    
    in->add(
        menu_cache,
        0,
        _cache_callback,
        this,
        Menu_Item::TOGGLE,
        _cache);

    in->add(
        menu_cache_clear,
        0,
        _cache_clear_callback,
        this,
        Menu_Item::DIVIDER);

    in->add(menu_messages, 0, _messages_callback, this);
    in->add(menu_prefs, 0, _prefs_callback, this, Menu_Item::DIVIDER);

    in->add(
        menu_exit,
        shortcuts[Shortcut::EXIT].value,
        _exit_callback,
        this);

    in->end();
}

namespace
{

const String
    tooltip_open = "Open a file\n\nShortcut: %%",
    tooltip_reload = "Reload the current file\n\nShortcut: %%",
    tooltip_close = "Close the current file\n\nShortcut: %%";

} // namespace

void File_Group::toolbar_update()
{
    // Update tooltips.

    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    _open_widget->tooltip(String_Format(tooltip_open).
        arg(djv::Shortcut::label(shortcuts[Shortcut::FILE_OPEN].value)));

    _reload_widget->tooltip(String_Format(tooltip_reload).
        arg(djv::Shortcut::label(shortcuts[Shortcut::FILE_RELOAD].value)));

    _close_widget->tooltip(String_Format(tooltip_close).
        arg(djv::Shortcut::label(shortcuts[Shortcut::FILE_CLOSE].value)));
}

void File_Group::open(const File & in)
{
    //DJV_DEBUG("File_Group::open");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("in type = " << in.type());

    File file = in;

    // Initialize.

    cache_del();

    _file = File();
    _info = Image_Io_Info();
    _load.reset();

    // Match wildcards.

    if (file.is_seq_wildcard())
    {
        file = File_Util::seq_wildcard_match(
            file,
            Directory(file.path()).list());
    }

    //DJV_DEBUG_PRINT("file = " << file);
    //DJV_DEBUG_PRINT("file type = " << file.type());
    //DJV_DEBUG_PRINT("seq valid = " << file.is_seq_valid());
    //DJV_DEBUG_PRINT("seq = " << file.seq());

    // Automatic sequence loading.

    if (_seq_auto &&
        file.is_seq_valid() &&
        file.seq().list.size() == 1)
    {
        file.type(File::SEQ);

        const List<File> list = Directory(file.path()).list();

        Seq seq = file.seq();
        seq.list.clear();
        file.seq(seq);

        for (size_t i = 0; i < list.size(); ++i)
        {
            file.seq_add(list[i]);
        }

        File_Util::seq_sort(file);

        //DJV_DEBUG_PRINT("sequence = " << file);
    }

    // Load.

    if (! file.get().empty())
    {
        //DJV_DEBUG_PRINT("loading...");

        try
        {
            _load = std::auto_ptr<Image_Load>(
                Image_Load_Factory::global()->get(file, &_info));

            _file = file;

            File_Prefs::global()->recent(_file);
        }
        catch (Error error)
        {
            //DJV_DEBUG_PRINT("error = " << error);

            DJV_APP->error(error);
        }
    }

    _layer = 0;
    _layers.resize(_info.layer_size());

    for (List<String>::size_type i = 0; i < _layers.size(); ++i)
    {
        _layers[i] = _info[i].layer_name;
    }
}

void File_Group::open_callback(const File & in)
{
    open_signal.emit(in);
}

void File_Group::open_callback(bool)
{
    _open_dialog->show();
}

void File_Group::_open_callback()
{
    open_callback(true);
}

const djv::Image * File_Group::get(int64_t frame) const
{
    //DJV_DEBUG("File_Group::get");
    //DJV_DEBUG_PRINT("frame = " << frame);

    File_Group * that = const_cast<File_Group *>(this);

    that->_image = 0;

    Cache_Ref * prev = _cache_ref;

    that->_cache_ref =
        _cache ?
        Cache::global()->get(this, frame) :
        0;

    if (_cache_ref)
    {
        //DJV_DEBUG_PRINT("cached image");

        that->_image = _cache_ref->get();
    }
    else
    {
        if (_load.get())
        {
            //DJV_DEBUG_PRINT("loading image");

            try
            {
                _load->load(
                    _u8_conversion ? that->_image_tmp2 : that->_image_tmp,
                    Image_Io_Frame_Info(
                        _info.seq.list.size() ? _info.seq.list[frame] : -1,
                        _layer,
                        _proxy));
                
                if (_u8_conversion)
                {
                    //DJV_DEBUG_PRINT("u8 conversion");
                    //DJV_DEBUG_PRINT("image = " << _image_tmp2);
                    
                    Pixel_Data_Info info(_image_tmp2.info());
                    info.pixel = Pixel::pixel(info.pixel, Pixel::U8);
                    that->_image_tmp.set(info);
                    that->_image_tmp.tag = _image_tmp2.tag;
                    that->_image_tmp.color_profile = Color_Profile();
                    
                    Gl_Image_Options options;
                    options.color_profile = _image_tmp2.color_profile;
                    options.proxy_scale = false;
                    
                    Gl_Image::copy(_image_tmp2, that->_image_tmp, options);
                }
                
                that->_image = &_image_tmp;
                
                //DJV_DEBUG_PRINT("image = " << *_image);
            }
            catch (Error error)
            {
                //DJV_DEBUG_PRINT("error" << error);

                DJV_APP->error(error);
            }
        }

        if (_image && _cache)
        {
            that->_cache_ref = Cache::global()->create(
                new djv::Image(*_image),
                this,
                frame);
        }
    }

    if (prev)
    {
        prev->ref_del();
    }

    return _image;
}

const File & File_Group::file() const
{
    return _file;
}

const Image_Io_Info & File_Group::info() const
{
    return _info;
}

void File_Group::recent_callback(bool)
{
    //DJV_DEBUG("File_Group::recent_callback");

    File file =
        File_Prefs::global()->recent()[
            List_Util::find(_menu->item(), _menu_recent)];

    if (file.is_seq_valid())
    {
        file.type(File::SEQ);
    }

    open_signal.emit(file);
}

void File_Group::_recent_callback()
{
    recent_callback(true);
}

void File_Group::recent_callback(const List<String> &)
{
    update_signal.emit(true);
}

void File_Group::reload_callback(bool)
{
    //DJV_DEBUG("File_Group::reload_callback");

    if (Image_Prefs::global()->frame_store_file_reload())
    {
        image_frame_store_signal.emit(true);
    }

    cache_del();

    _load.reset();

    if (_file.get().size())
    {
        try
        {
            _load = std::auto_ptr<Image_Load>(
                Image_Load_Factory::global()->get(_file, &_info));
        }
        catch (Error error)
        {
            DJV_APP->error(error);
        }
    }

    reload_signal.emit(true);
}

void File_Group::reload_callback(Image_Io_Base * in)
{
    if (! _load.get())
    {
        return;
    }

    //DJV_DEBUG("File_Group::reload_callback");

    if (! in)
    {
        reload_callback(true);
    }
    else if (in->name() == _load->name())
    {
        cache_del();

        const List<String> & options = in->options();

        for (size_t i = 0; i < options.size(); ++i)
        {
            String tmp = in->option(options[i]);
            _load->option(options[i], &tmp);
        }

        reload_signal.emit(true);
    }
}

void File_Group::_reload_callback()
{
    reload_callback(true);
}

void File_Group::close_callback(bool)
{
    //DJV_DEBUG("File_Group::close_callback");

    open_signal.emit(File());
}

void File_Group::_close_callback()
{
    close_callback(true);
}

namespace
{

const String
    label_save_check = "Overwrite existing file \"%%\"?";

} // namespace

void File_Group::save_callback(const File & in)
{
    //DJV_DEBUG("File_Group::save_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    _file_save = in;

    if (File_Util::exists(_file_save))
    {
        //DJV_DEBUG_PRINT("exists");

        Question_Dialog::dialog(
            String_Format(label_save_check).arg(_file_save),
            this,
            save_check_callback);
    }
    else
    {
        save_signal.emit(_file_save);
    }
}

void File_Group::save_check_callback(bool)
{
    save_signal.emit(_file_save);
}

void File_Group::_save_callback()
{
    _save_dialog->signal.del(this);
    _save_dialog->signal.set(this, save_callback);

    _save_dialog->show();
}

void File_Group::save_frame_callback(const File & in)
{
    _file_save = in;

    if (File_Util::exists(_file_save))
    {
        Question_Dialog::dialog(
            String_Format(label_save_check).arg(_file_save),
            this,
            save_frame_check_callback);
    }
    else
    {
        save_frame_signal.emit(_file_save);
    }
}

void File_Group::save_frame_check_callback(bool)
{
    save_frame_signal.emit(_file_save);
}

void File_Group::_save_frame_callback()
{
    _save_dialog->signal.del(this);
    _save_dialog->signal.set(this, save_frame_callback);

    _save_dialog->show();
}

void File_Group::seq_auto(bool in)
{
    if (in == _seq_auto)
    {
        return;
    }

    _seq_auto = in;

    update_signal.emit(true);
}

bool File_Group::seq_auto() const
{
    return _seq_auto;
}

void File_Group::seq_auto_callback(bool in)
{
    seq_auto(in);
}

void File_Group::_seq_auto_callback()
{
    seq_auto(_menu->value());
}

void File_Group::layer(int in)
{
    if (in == _layer)
    {
        return;
    }

    //DJV_DEBUG("File_Group::layer");
    //DJV_DEBUG_PRINT("in = " << in);

    const int size = static_cast<int>(_layers.size());

    //DJV_DEBUG_PRINT("layer list = " << size);

    cache_del();

    _layer = Math::wrap(in, 0, size - 1);

    //DJV_DEBUG_PRINT("layer = " << _layer);

    image_signal.emit(true);
    update_signal.emit(true);
}

int File_Group::layer() const
{
    return _layer;
}

void File_Group::layer_callback(int in)
{
    layer(in);
}

void File_Group::_layer_callback()
{
    layer(List_Util::find(_menu->item(), _menu_layer));
}

void File_Group::_layer_prev_callback()
{
    layer(_layer - 1);
}

void File_Group::_layer_next_callback()
{
    layer(_layer + 1);
}

void File_Group::proxy(Pixel_Data_Info::PROXY in)
{
    if (in == _proxy)
        return;

    //DJV_DEBUG("File_Group::proxy");
    //DJV_DEBUG_PRINT("in = " << in);

    cache_del();

    _proxy = in;

    image_signal.emit(true);
    update_signal.emit(true);
}

Pixel_Data_Info::PROXY File_Group::proxy() const
{
    return _proxy;
}

void File_Group::proxy_callback(Pixel_Data_Info::PROXY in)
{
    proxy(in);
}

void File_Group::_proxy_callback()
{
    proxy(static_cast<Pixel_Data_Info::PROXY>(
        List_Util::find(_menu->item(), _menu_proxy)));
}

void File_Group::u8_conversion(bool in)
{
    if (in == _u8_conversion)
        return;
    
    cache_del();
    
    _u8_conversion = in;

    image_signal.emit(true);
    update_signal.emit(true);
}

bool File_Group::u8_conversion() const
{
    return _u8_conversion;
}

void File_Group::u8_conversion_callback(bool in)
{
    u8_conversion(in);
}

void File_Group::_u8_conversion_callback()
{
    u8_conversion(_menu->value());
}

void File_Group::cache(bool in)
{
    if (in == _cache)
    {
        return;
    }

    //DJV_DEBUG("File_Group::cache");
    //DJV_DEBUG_PRINT("in = " << in);

    cache_del();

    _cache = in;

    update_signal.emit(true);
}

bool File_Group::cache() const
{
    return _cache;
}

void File_Group::cache_del()
{
    //DJV_DEBUG("File_Group::cache_del");

    Cache::global()->del(this);
}

void File_Group::cache_callback(bool in)
{
    cache(in);
}

void File_Group::_cache_callback()
{
    cache(_menu->value());
}

void File_Group::cache_clear_callback(bool)
{
    Cache::global()->del();
}

void File_Group::_cache_clear_callback()
{
    cache_clear_callback(true);
}

void File_Group::cache_update_callback(bool)
{
    //DJV_DEBUG("File_Group::cache_update_callback");

    cache_signal.emit(Cache::global()->frames(this));
}

void File_Group::_messages_callback()
{
    Application_Message_Dialog::global()->show();
}

void File_Group::_prefs_callback()
{
    Prefs_Dialog::global()->show();
}

void File_Group::_exit_callback()
{
    DJV_APP->exit(Application::EXIT_DEFAULT);
}

} // djv_view

