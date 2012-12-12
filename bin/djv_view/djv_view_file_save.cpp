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

//! \file djv_view_file_save.cpp

#include <djv_view_file_save.h>

#include <djv_application.h>
#include <djv_progress_dialog.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// File_Save_Info
//------------------------------------------------------------------------------

File_Save_Info::File_Save_Info(
    const File &             input_file,
    const File &             output_file,
    const Pixel_Data_Info &  info,
    const Seq &              seq,
    int                      layer,
    Pixel_Data_Info::PROXY   proxy,
    bool                     u8_conversion,
    bool                     color_profile,
    const Gl_Image_Options & options) :
    input_file   (input_file),
    output_file  (output_file),
    info         (info),
    seq          (seq),
    layer        (layer),
    proxy        (proxy),
    u8_conversion(u8_conversion),
    color_profile(color_profile),
    options      (options)
{}

//------------------------------------------------------------------------------
// File_Save::_File_Save
//------------------------------------------------------------------------------

struct File_Save::_File_Save : public Callback
{
    _File_Save(const File_Save_Info &, File_Save * parent);

    virtual ~_File_Save();

private:

    DJV_CALLBACK(_File_Save, callback, int);
    DJV_CALLBACK(_File_Save, del_callback, bool);

    File_Save_Info            _info;
    std::auto_ptr<Image_Load> _load;
    std::auto_ptr<Image_Save> _save;
    Seq                       _load_seq;
    Seq                       _save_seq;
    Progress_Dialog *         _dialog;
    File_Save *               _parent;
};

namespace
{

const String
    label_save = "Saving \"%%\":";

} // namespace

File_Save::_File_Save::_File_Save(
    const File_Save_Info & in,
    File_Save * parent) :
    _info    (in),
    _load_seq(in.seq),
    _save_seq(
        in.output_file.seq().start(),
        in.output_file.seq().start() + static_cast<int>(in.seq.list.size()) - 1,
        in.seq.pad,
        in.seq.speed),
    _dialog  (0),
    _parent  (parent)
{
    //DJV_DEBUG("File_Save::_File_Save::_File_Save");
    //DJV_DEBUG_PRINT("input = " << _info.input_file);
    //DJV_DEBUG_PRINT("output = " << _info.output_file);
    //DJV_DEBUG_PRINT("load time = " << _load_seq);
    //DJV_DEBUG_PRINT("save time = " << _save_seq);

    _info.options.xform.rotate = -_info.options.xform.rotate; //! \todo WTF?

    const Box2f bbox =
        Gl_Image_Xform::xform_matrix(_info.options.xform) *
        Box2f(_info.info.size * Pixel_Data::proxy_scale(_info.info.proxy));

    //DJV_DEBUG_PRINT("bbox = " << bbox);

    _info.options.xform.position = -bbox.position;
    _info.info.size = bbox.size;

    // Open input.

    Image_Io_Info load_info;

    try
    {
        _load = std::auto_ptr<Image_Load>(
            Image_Load_Factory::global()->get(_info.input_file, &load_info));
    }
    catch (Error error)
    {
        DJV_APP->error(error);

        return;
    }

    // Open output.

    Image_Io_Info save_info(_info.info);
    save_info.tag = load_info.tag;
    save_info.seq = _save_seq;

    try
    {
        _save = std::auto_ptr<Image_Save>(
            Image_Save_Factory::global()->get(_info.output_file, save_info));
    }
    catch (Error error)
    {
        DJV_APP->error(error);

        return;
    }

    // Start...

    _dialog = new Progress_Dialog;
    _dialog->signal.set(this, callback);
    _dialog->del_signal.set(this, del_callback);
    _dialog->set(static_cast<int>(_save_seq.list.size()));
    _dialog->label(String_Format(label_save).arg(_info.output_file));
    _dialog->show();
}

File_Save::_File_Save::~_File_Save()
{
    //DJV_DEBUG("File_Save::_File_Save::~_File_Save");

    if (_save.get())
    {
        try
        {
            //DJV_DEBUG_PRINT("close");

            _save->close();
        }
        catch (Error error)
        {
            DJV_APP->error(error);
        }

        _save.reset();
    }

    if (_dialog)
    {
        _dialog->signal.del(this);
        _dialog->del_signal.del(this);
        _dialog->del();
    }
}

void File_Save::_File_Save::callback(int in)
{
    //DJV_DEBUG("File_Save::_File_Save::callback");
    //DJV_DEBUG_PRINT("in = " << in);

    // Load the frame.

    djv::Image image;

    try
    {
        //DJV_DEBUG_PRINT("load");

        _load->load(
            image,
            Image_Io_Frame_Info(
                _load_seq.list[in],
                _info.layer,
                _info.proxy));

        //DJV_DEBUG_PRINT("image = " << image);
    }
    catch (Error error)
    {
        DJV_APP->error(error);

        del_callback(false);

        return;
    }

    // Process the frame.

    djv::Image tmp(_info.info);
    tmp.tag = image.tag;
    
    Gl_Image_Options options(_info.options);
    
    if (_info.u8_conversion || _info.color_profile)
    {
        options.color_profile = image.color_profile;
    }

    try
    {
        //DJV_DEBUG_PRINT("process");

        Gl_Image::copy(image, tmp, options);
    }
    catch (Error error)
    {
        DJV_APP->error(error);

        del_callback(false);

        return;
    }

    // Save the frame.

    try
    {
        //DJV_DEBUG_PRINT("save");

        _save->save(tmp, _save_seq.list[in]);
    }
    catch (Error error)
    {
        DJV_APP->error(error);

        del_callback(false);

        return;
    }
}

void File_Save::_File_Save::del_callback(bool)
{
    //DJV_DEBUG("File_Save::_File_Save::del_callback");

    _parent->del(this);
}

//------------------------------------------------------------------------------
// File_Save
//------------------------------------------------------------------------------

File_Save::~File_Save()
{
    //DJV_DEBUG("File_Save::~File_Save");

    for (size_t i = 0; i < _list.size(); ++i)
    {
        delete _list[i];
    }
}

void File_Save::set(const File_Save_Info & in)
{
    //DJV_DEBUG("File_Save::File_Save::set");

    _list += new _File_Save(in, this);
}

void File_Save::del(_File_Save * in)
{
    //DJV_DEBUG("File_Save::File_Save::del");

    for (
        List<_File_Save *>::iterator i = _list.begin(), end = _list.end();
        i != end;
        ++i)
    {
        if (in == *i)
        {
            delete *i;
            _list.erase(i);
            break;
        }
    }
}

File_Save * File_Save::global()
{
    static File_Save * global = 0;

    if (! global)
    {
        global = new File_Save;
    }

    return global;
}

} // djv_view

