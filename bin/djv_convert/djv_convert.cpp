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

//! \file djv_convert.cpp

#include <djv_convert.h>

#include <djv_image_io.h>
#include <djv_user.h>
#include <djv_system.h>
#include <djv_time.h>

namespace djv_convert
{
using djv::Time;

//------------------------------------------------------------------------------
// Options
//------------------------------------------------------------------------------

Options::Options() :
    scale(1.0),
    channel(Gl_Image_Options::CHANNEL(0))
{}

//------------------------------------------------------------------------------
// Input
//------------------------------------------------------------------------------

Input::Input() :
    layer(0),
    proxy(Pixel_Data_Info::PROXY(0)),
    slate_frames(0),
    timeout(0)
{}

//------------------------------------------------------------------------------
// Output
//------------------------------------------------------------------------------

Output::Output() :
    tag_auto(true)
{}

//------------------------------------------------------------------------------
// Application
//------------------------------------------------------------------------------

namespace
{

String label_image(const Pixel_Data_Info & in, const Seq & seq)
{
    return String_Format("%%x%%:%% %% %%@%%").
        arg(in.size.x).
        arg(in.size.y).
        arg(Vector_Util::aspect(in.size), 0, -2).
        arg(String_Util::label(in.pixel)).
        arg(Time::units_to_string(
            static_cast<int>(seq.list.size()), seq.speed)).
        arg(Speed::speed_to_float(seq.speed));
}

const String
    label_slate = "Slating...",
    label_timeout = "Timeout...",
    label_estimate = "[%%%] Estimated = %% (%% Frames/Second)",
    label_complete = "[100%] ",
    label_elapsed = "Elapsed = %%";

const String
    error_command_line_input = "Input",
    error_command_line_output = "Output";

} // namespace

Application::Application(int argc, char ** argv) :
    Core_Application("djv_convert", argc, argv)
{
    //DJV_DEBUG("Application::Application");

    // Command line.

    try
    {
        String tmp = command_line_args();
        command_line(tmp);
    }
    catch (Error in)
    {
        error(Error(String_Format(error_command_line).arg(in)));
        exit(EXIT_ERROR);
    }

    if (exit() != EXIT_DEFAULT)
    {
        return;
    }

    // Work.

    if (! work())
    {
        exit(EXIT_ERROR);
    }
}

void Application::command_line(String & in) throw (Error)
{
    //DJV_DEBUG("command_line");
    //DJV_DEBUG_PRINT("in = " << in);

    Core_Application::command_line(in);

    if (exit() != EXIT_DEFAULT)
    {
        return;
    }

    if (in.empty())
    {
        exit(EXIT_HELP);
        return;
    }

    List<String> args;
    String arg;

    try
    {
        while (! in.empty())
        {
            in >> arg;

            // Options.

            if ("-mirror_h" == arg)
            {
                _options.mirror.x = true;
            }
            else if ("-mirror_v" == arg)
            {
                _options.mirror.y = true;
            }
            else if ("-scale" == arg)
            {
                in >> _options.scale.x;
                _options.scale.y = _options.scale.x;
            }
            else if ("-scale_xy" == arg)
            {
                in >> _options.scale;
            }
            else if ("-resize" == arg)
            {
                in >> _options.size;
            }
            else if ("-width" == arg)
            {
                in >> _options.size.x;
            }
            else if ("-height" == arg)
            {
                in >> _options.size.y;
            }
            else if ("-channel" == arg)
            {
                in >> _options.channel;
            }

            // Input options.

            else if ("-layer" == arg)
            {
                in >> _input.layer;
            }
            else if ("-proxy" == arg)
            {
                in >> _input.proxy;
            }
            else if ("-time" == arg)
            {
                in >> _input.start;
                in >> _input.end;
            }
            else if ("-slate" == arg)
            {
                String tmp;
                in >> tmp;
                _input.slate = tmp;
                in >> _input.slate_frames;
            }
            else if ("-timeout" == arg)
            {
                in >> _input.timeout;
            }

            // Output options.

            else if ("-pixel" == arg)
            {
                Pixel::PIXEL value = Pixel::PIXEL(0);
                in >> value;
                _output.pixel =
                    std::auto_ptr<Pixel::PIXEL>(new Pixel::PIXEL(value));
            }
            else if ("-speed" == arg)
            {
                Speed::FPS value = Speed::FPS(0);
                in >> value;
                _output.speed =
                    std::auto_ptr<Speed::FPS>(new Speed::FPS(value));
            }
            else if ("-tag" == arg)
            {
                String name, value;
                in >> name;
                in >> value;
                _output.tag[name] = value;
            }
            else if ("-tag_auto" == arg)
            {
                in >> _output.tag_auto;
            }

            // Arguments.

            else
            {
                args += arg;
            }
        }
    }
    catch (String)
    {
        throw Error(arg);
    }

    //DJV_DEBUG_PRINT("arg list = " << args);

    // Input file.

    if (! args.size())
    {
        throw Error(error_command_line_input);
    }

    _input.file = args.pop_front();

    if (_input.file.is_seq_valid())
    {
        _input.file.type(File::SEQ);
    }

    // Output file.

    if (! args.size())
    {
        throw Error(error_command_line_output);
    }

    _output.file = args.pop_front();

    if (_output.file.is_seq_valid())
    {
        _output.file.type(File::SEQ);
    }

    if (! args.empty())
    {
        throw Error(args[0]);
    }
}

namespace
{

const String label_command_line_help =
" djv_convert\n"
"\n"
"     This program provides a command line tool for movie and image\n"
"     conversion.\n"
"\n"
" Usage\n"
"\n"
"     djv_convert (input) (output) [option]...\n"
"\n"
" Image Options\n"
"\n"
"     -mirror_h\n"
"     -mirror_v\n"
"         Mirror the image horizontally or vertically.\n"
"\n"
"     -scale (value)\n"
"     -scale_xy (x) (y)\n"
"         Scale the image.\n"
"\n"
"     -resize (width) (height)\n"
"         Resize the image.\n"
"\n"
"     -width (value)\n"
"     -height (value)\n"
"         Resize the width or height of the image maintaining the aspect"
"ratio.\n"
"\n"
"     -channel (value)\n"
"         Show only specific image channels. Options = %%.\n"
"\n"
" Input Options\n"
"\n"
"     -layer (value)\n"
"         Set the input layer.\n"
"\n"
"     -proxy (value)\n"
"         Set the proxy scale. Options = %%. Default = %%.\n"
"\n"
"     -time (start) (end)\n"
"         Set the start and end time.\n"
"\n"
"     -slate (input) (frames)\n"
"         Set the slate.\n"
"\n"
"     -timeout (value)\n"
"         Set the maximum number of seconds to wait for each input frame."
" Default = %%.\n"
"\n"
" Output Options\n"
"\n"
"     -pixel (value)\n"
"         Convert the pixel type. Options = %%.\n"
"\n"
"     -speed (value)\n"
"         Set the speed. Options = %%.\n"
"\n"
"     -tag (name) (value)\n"
"         Set an image tag.\n"
"\n"
"     -tag_auto (value)\n"
"         Automatically generate image tags (e.g., timecode). Options = %%."
" Default = %%.\n"
"\n"
"%%"
" Examples\n"
"\n"
"     > djv_convert input.sgi output.tga\n"
"     Convert an image.\n"
"\n"
"     > djv_convert input.1-100.sgi output.1.tga\n"
"     Convert an image sequence. Note that only the first frame of the output"
" is specified\n"
"\n"
"     > djv_convert input.1-100.sgi output.1.tga -save tga compression rle\n"
"     Create an RLE compressed image sequence.\n"
"\n"
"     > djv_convert input.0001-0100.dpx output.mov\n"
"     Convert an image sequence to a movie.\n"
"\n"
"     > djv_convert input.mov output.1.tga\n"
"     Convert a movie to an image sequence.\n"
"\n"
"     > djv_convert input.sgi output.sgi -pixel rgb u16\n"
"     Convert the pixel type.\n"
"\n"
"     > djv_convert input.tga output.tga -scale 0.5 0.5\n"
"     Scale an image by half.\n"
"\n"
"     > djv_convert input.tga output.tga -resize 2048 1556\n"
"     Resize an image.\n"
"\n"
"     > djv_convert input.cin output.tga\n"
"     Convert a Cineon file to a linear format using the default settings.\n"
"\n"
"     > djv_convert input.cin output.tga -load cineon print 95 685 2.2 10\n"
"     Convert a Cineon file to a linear format using custom print settings"
" (black point, white point, gamma, and soft clip).\n";

} // namespace

String Application::command_line_help() const
{
    return String(String_Format(label_command_line_help).
        arg(String_Util::lower(Gl_Image_Options::label_channel()), ", ").
        arg(String_Util::lower(Pixel_Data_Info::label_proxy()), ", ").
        arg(String_Util::lower(String_Util::label(_input.proxy))).
        arg(_input.timeout).
        arg(String_Util::lower(Pixel::label_pixel()), ", ").
        arg(String_Util::lower(Speed::label_fps()), ", ").
        arg(String_Util::lower(String_Util::label_bool()), ", ").
        arg(String_Util::lower(String_Util::label(_output.tag_auto))).
        arg(Core_Application::command_line_help()));
}

bool Application::work()
{
    //DJV_DEBUG("Application::work");
    //DJV_DEBUG_PRINT("input = " << _input.file);
    //DJV_DEBUG_PRINT("output = " << _output.file);

    Timer timer;
    timer.start();

    Gl_Image_Options options;
    options.xform.mirror = _options.mirror;
    options.xform.scale = _options.scale;
    options.channel = _options.channel;

    // Open input.

    std::auto_ptr<Image_Load> load;
    Image_Io_Info load_info;

    Error error;

    while (! load.get())
    {
        try
        {
            load = std::auto_ptr<Image_Load>(
                Image_Load_Factory::global()->get(_input.file, &load_info));
        }
        catch (Error in)
        {
            error = in;
        }

        if (! load.get() && _input.timeout)
        {
            print(label_timeout);
            Time::sleep(1);
        }
        else
        {
            break;
        }
    }

    if (! load.get())
    {
        this->error(error);
        return false;
    }

    _input.layer = Math::clamp(
        _input.layer,
        0,
        static_cast<int>(load_info.layer_size()) - 1);

    int64_t start = 0;
    int64_t end =
        load_info.seq.list.size() ?
        (static_cast<int64_t>(load_info.seq.list.size()) - 1) :
        0;
    
    if (! _input.start.empty())
        start = Seq::find_closest(
            Time::string_to_units(_input.start, load_info.seq.speed),
            load_info.seq.list);

    if (! _input.end.empty())
        end = Seq::find_closest(
            Time::string_to_units(_input.end, load_info.seq.speed),
            load_info.seq.list);

    //DJV_DEBUG_PRINT("start = " << start);
    //DJV_DEBUG_PRINT("end = " << end);

    load_info.seq.list =
        List<int64_t>(load_info.seq.list, start, end - start + 1);

    print(String_Format("%% %%").
        arg(_input.file).
        arg(label_image(load_info, load_info.seq)));

    // Open output.

    std::auto_ptr<Image_Save> save;

    Image_Io_Info save_info(load_info[_input.layer]);

    if (Vector_Util::is_size_valid(_options.size))
    {
        save_info.size = _options.size;
    }
    else if (_options.size.x)
    {
        save_info.size = V2i(
            _options.size.x,
            Math::ceil(_options.size.x / Vector_Util::aspect(load_info.size)));
    }
    else if (_options.size.y)
    {
        save_info.size = V2i(
            Math::ceil(_options.size.x * Vector_Util::aspect(load_info.size)),
            _options.size.y);
    }
    else
    {
        save_info.size = Vector_Util::ceil<double, int>(
            V2f(load_info.size) * options.xform.scale);
    }

    if (_output.pixel.get())
    {
        save_info.pixel = *_output.pixel;
    }

    save_info.tag = _output.tag;

    save_info.seq = Seq(
        _output.file.seq().start(),
        _output.file.seq().start() +
        _input.slate_frames + (
            load_info.seq.list.size() ?
            (static_cast<int64_t>(load_info.seq.list.size()) - 1) :
            0),
        _output.file.seq().pad,
        load_info.seq.speed);

    if (_output.speed.get())
    {
        save_info.seq.speed =  *_output.speed;
    }
    
    //DJV_DEBUG_PRINT("save seq = " << save_info.seq);

    try
    {
        save = std::auto_ptr<Image_Save>(
            Image_Save_Factory::global()->get(_output.file, save_info));
    }
    catch (Error error)
    {
        this->error(error);
        return false;
    }

    print(String_Format("%% %%").
        arg(_output.file).
        arg(label_image(save_info, save_info.seq)));

    // Slate.

    Image slate;

    if (! _input.slate.name().empty())
    {
        try
        {
            print(label_slate);

            Image_Io_Info info;
            std::auto_ptr<Image_Load> plugin(
                Image_Load_Factory::global()->get(_input.slate, &info));

            Image image;
            
            plugin->load(image);

            slate.set(save_info);

            Gl_Image_Options options;
            options.xform.scale = V2f(save_info.size) / V2f(info.size);
            options.color_profile = image.color_profile;

            Gl_Image::copy(image, slate, options);
        }
        catch (Error error)
        {
            this->error(error);
            return false;
        }
    }

    // Work.

    _offscreen_buffer =
        std::auto_ptr<Gl_Offscreen_Buffer>(new Gl_Offscreen_Buffer(save_info));

    static const int progress_length = 10;
    int progress_count = 0;
    double progress_accum = 0.0;

    for (int64_t i = 0; i < _input.slate_frames; ++i)
    {
        try
        {
            save->save(
                slate,
                Image_Io_Frame_Info(save_info.seq.list.pop_front()));
        }
        catch (Error error)
        {
            this->error(error);
            return false;
        }
    }

    const int64_t length = static_cast<int64_t>(save_info.seq.list.size());

    for (int64_t i = 0; i < length; ++i)
    {
        Timer frame_timer;
        frame_timer.start();

        // Load.

        Image image;

        int timeout = _input.timeout;

        while (! image.is_valid())
        {
            try
            {
                load->load(
                    image,
                    Image_Io_Frame_Info(
                        load_info.seq.list.size() ? load_info.seq.list[i] : -1,
                        _input.layer,
                        _input.proxy));
            }
            catch (Error in)
            {
                error = in;
            }

            if (! image.is_valid() && timeout > 0)
            {
                //print(label_timeout);
                --timeout;
                Time::sleep(1);
            }
            else
            {
                break;
            }
        }

        if (! image.is_valid())
        {
            this->error(error);
            
            return false;
        }

        //DJV_DEBUG_PRINT("image = " << *image);

        // Image tags.

        Image_Tag tag = _output.tag;

        tag.add(image.tag);

        if (_output.tag_auto)
        {
            tag[Image_Tag::label_tag()[Image_Tag::CREATOR]] =
                User::current();

            tag[Image_Tag::label_tag()[Image_Tag::TIME]] =
                Time::time_to_string(Time::current());

            tag[Image_Tag::label_tag()[Image_Tag::TIMECODE]] =
                Time::timecode_to_string(
                    Time::frame_to_timecode(
                        save_info.seq.list.size() ? save_info.seq.list[i] : 0,
                        save_info.seq.speed));
        }

        // Process.

        Image tmp(save_info);
        tmp.tag = tag;

        options.xform.scale = V2f(save_info.size) / V2f(load_info.size);
        options.color_profile = image.color_profile;

        Gl_Image::copy(
            image,
            tmp,
            options,
            &_state,
            _offscreen_buffer.get());

        // Save.

        //DJV_DEBUG_PRINT("output = " << tmp);

        try
        {
            save->save(
                tmp,
                Image_Io_Frame_Info(
                    save_info.seq.list.size() ? save_info.seq.list[i] : -1));
        }
        catch (Error error)
        {
            this->error(error);
            return false;
        }

        // Statistics.

        timer.check();
        frame_timer.check();

        progress_accum += frame_timer.seconds();

        if (
            length > 1 &&
            i / static_cast<double>(length) * progress_length > progress_count)
        {
            const double estimate =
                progress_accum /
                static_cast<double>(i + 1) * (length - (i + 1));

            print(String_Format(label_estimate).
                arg(static_cast<int>(
                    i / static_cast<double>(length) * 100.0), 3).
                arg(Time::label_time(estimate)).
                arg(i / timer.seconds(), 0, 2));

            ++progress_count;
        }
    }

    if (length > 1)
    {
        print(label_complete, false);
    }

    try
    {
        save->close();
    }
    catch (Error error)
    {
        this->error(error);
        return false;
    }

    print(String_Format(label_elapsed).
        arg(Time::label_time(timer.seconds())));

    return true;
}

} // djv_convert

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
    int r = 1;

    try
    {
        r = djv_convert::Application(argc, argv).exec();
    }
    catch (djv::Error in)
    {
        djv::Error_Util::print(in);
    }

    return r;
}
