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

//! \file djv_ifl_load.cpp

#include <djv_ifl_load.h>

#include <djv_file.h>
#include <djv_file_io.h>

namespace djv_ifl
{

//------------------------------------------------------------------------------
// djv_image_load
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_load()
{
    return new Load;
}

}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Plugin * Load::copy() const
{
    return new Load;
}

String Load::name() const
{
    return djv_ifl::name;
}

List<String> Load::extensions() const
{
    return djv_ifl::extensions;
}

void Load::open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::open");
    //DJV_DEBUG_PRINT("in = " << in);

    _list.clear();

    List<String> tmp;

    try
    {
        tmp = File_Io::lines(in);

        //DJV_DEBUG_PRINT("list = " << tmp);
    }
    catch (Error error)
    {
        throw_error_read(name(), in);
    }

    for (size_t i = 0; i < tmp.size(); ++i)
    {
        if (tmp[i].empty())
        {
            continue;
        }

        File file(tmp[i]);

        if (file.path().empty())
        {
            file.path(in.path());
        }

        if (file.is_seq_valid())
        {
            file.type(File::SEQ);

            _list += File_Util::seq_expand(file);
        }
        else
        {
            _list += tmp[i];
        }
    }

    //DJV_DEBUG_PRINT("list = " << _list);

    std::auto_ptr<Image_Load> plugin(Image_Load_Factory::global()->get(
        _list.size() ? _list[0] : String(), &info));

    info.seq.list.resize(_list.size());

    for (size_t i = 0; i < _list.size(); ++i)
    {
        info.seq.list[i] = static_cast<int>(i);
    }
}

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
{
    //DJV_DEBUG("Load::load");
    //DJV_DEBUG_PRINT("frame = " << frame);

    image.color_profile = Color_Profile();
    image.tag = Image_Tag();

    String file_name;

    if (_list.size())
    {
        if (-1 == frame.frame)
        {
            file_name = _list[0];
        }
        else if (
            frame.frame >= 0 &&
            frame.frame < static_cast<int>(_list.size()))
        {
            file_name = _list[frame.frame];
        }
    }

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info;

    std::auto_ptr<Image_Load> plugin(
        Image_Load_Factory::global()->get(file_name, &info));

    plugin->load(image, Image_Io_Frame_Info(-1, frame.layer, frame.proxy));
}

} // djv_ifl

