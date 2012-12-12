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

//! \file djv_quicktime_save.cpp

#include <djv_quicktime_save.h>

#include <djv_gl_image.h>

namespace qt
{
#if defined(DJV_OSX)
#elif defined(DJV_WINDOWS)
#include <ImageCompression.h>
#include <QTML.h>
#endif
#define qt_anyCodec ((qt::CodecComponent)0) //! \todo Is this still necessary?
}

namespace djv_quicktime
{

//------------------------------------------------------------------------------
// djv_image_save
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_save()
{
    return new Save;
}

}

//------------------------------------------------------------------------------
// Save::Options
//------------------------------------------------------------------------------

Save::Options::Options() :
    codec  (CODEC_JPEG),
    quality(QUALITY_NORMAL)
{}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

const List<String> & Save::label_options()
{
    static const List<String> data = List<String>() <<
        "Codec" <<
        "Quality";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Save::Save() :
    _f             (0),
    _frame_duration(0),
    _movie         (0),
    _track         (0),
    _media         (0),
    _gworld        (0)
{}

void Save::init() throw (Error)
{
    quicktime_init();
}

void Save::del()
{
    quicktime_del();
}

Plugin * Save::copy() const
{
    Save * out = new Save;
    out->_options = _options;
    return out;
}

String Save::name() const
{
    return djv_quicktime::name;
}

List<String> Save::extensions() const
{
    return djv_quicktime::extensions;
}

bool Save::is_seq() const
{
    return false;
}

bool Save::option(const String & in, String * data)
{
    const List<String> & list = options();

    try
    {
        if (String_Util::compare_no_case(in, list[CODEC_OPTION]))
        {
            *data >> _options.codec;
        }
        else if (String_Util::compare_no_case(in, list[QUALITY_OPTION]))
        {
            *data >> _options.quality;
        }
    }
    catch (String)
    {
        return false;
    }

    return true;
}

String Save::option(const String & in) const
{
    const List<String> & list = options();

    String out;

    if (String_Util::compare_no_case(in, list[CODEC_OPTION]))
    {
        out << _options.codec;
    }
    else if (String_Util::compare_no_case(in, list[QUALITY_OPTION]))
    {
        out << _options.quality;
    }

    return out;
}

List<String> Save::options() const
{
    return label_options();
}

namespace
{

const String
    error_track = "Cannot create track: %% (#%%)",
    error_media = "Cannot create media: %% (#%%)",
    error_media_begin = "Cannot begin editing media: %% (#%%)",
    error_media_end = "Cannot end editing media: %% (#%%)",
    error_gworld = "Cannot create GWorld: %% (#%%)",
    error_media_add = "Cannot add media: %% (#%%)",
    error_media_sample = "Cannot add frame %%: %% (#%%)",
    error_data_ref = "Cannot create data reference: %% (#%%)",
    error_open = "Cannot open: %%";

} // namespace

void Save::open(const File & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::open");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("info = " << info);

    // Open the file.
    
    _open(in, info);

    _track = qt::NewMovieTrack(
        _movie,
        qt::FixRatio(info.size.x, 1),
        qt::FixRatio(info.size.y, 1),
        qt::kNoVolume);

    if (! _track)
        throw_error(name(), String_Format(error_track).arg(in).
                    arg(qt::GetMoviesError()));

    _media = qt::NewTrackMedia(
        _track,
        qt::VideoMediaType,
        info.seq.speed.scale,
        0,
        0);

    if (! _media)
        throw_error(name(), String_Format(error_media).arg(in).
            arg(qt::GetMoviesError()));

    qt::OSErr err = qt::BeginMediaEdits(_media);

    if (err != qt::noErr)
        throw_error(name(), String_Format(error_media_begin).arg(in).
            arg(err));

    _file = in;

    _frame_duration = info.seq.speed.duration;

    // Allocate temporary buffers.

    Pixel_Data_Info _info;
    _info.size = info.size;
    _info.mirror.y = true;
    _info.pixel = Pixel::RGBA_U8;
    _image.set(_info);

    const qt::Rect rect = { 0, 0, _info.size.y, _info.size.x };
    
    err = QTNewGWorldFromPtr(
        &_gworld,
        qt::k32RGBAPixelFormat,
        &rect,
        0,
        0,
        0,
        (char *)_image.data(0, 0),
        _info.size.x * 4);

    if (err != qt::noErr)
    {
        throw_error(name(), String_Format(error_gworld).arg(in).arg(err));
    }

    qt::SetMovieGWorld(_movie, _gworld, 0);
}

void Save::_open(const String & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("info = " << info);

    /*qt::FSSpec file_spec;
    #if defined(DJV_OSX)
    qt::String255 file_name;
    qt::c2pstrcpy(file_name, file::path_absolute(in.get()).c_str());
    qt::FSMakeFSSpec(0, 0, file_name, &file_spec);
    #elif defined(DJV_WINDOWS)
    char file_name [cstring_size];
    cstring(file::path_absolute(in), file_name, cstring_size);
    qt::NativePathNameToFSSpec(file_name, &file_spec, 0);
    #endif

    qt::OSErr err = qt::CreateMovieFile(
      &file_spec,
      FOUR_CHAR_CODE('TVOD'),
      qt::smCurrentScript,
      qt::createMovieFileDeleteCurFile | qt::createMovieFileDontCreateResFile,
      &_f,
      &_movie
    );

    if (err != qt::noErr)
      throw_error(name(), String_Format(error_open).arg(in).arg(err));*/

    Handle handle;
    qt::OSType handle_type;
    qt::OSErr err = qt::QTNewDataReferenceFromFullPathCFString(
        CFStringRef(File_Util::path_absolute(in)).p,
        qt::kQTNativeDefaultPathStyle,
        //qt::kQTPOSIXPathStyle,
        0,
        &handle.p,
        &handle_type);

    if (err != qt::noErr)
    {
        throw_error(name(), String_Format(error_data_ref).arg(in).arg(err));
    }

    err = qt::CreateMovieStorage(
        handle.p,
        handle_type,
        FOUR_CHAR_CODE('TVOD'),
        qt::smCurrentScript,
        qt::createMovieFileDeleteCurFile | qt::createMovieFileDontCreateResFile,
        &_f,
        &_movie);

    if (err != qt::noErr)
    {
        throw_error(name(), String_Format(error_open).arg(in).arg(err));
    }
}

void Save::close() throw (Error)
{
    //DJV_DEBUG("Save::close");

    if (_track && _media)
    {
        //DJV_DEBUG_PRINT("end");

        qt::OSErr err = qt::EndMediaEdits(_media);

        if (err != qt::noErr)
        {
            throw_error(
                name(), String_Format(error_media_end).arg(_file).arg(err));
        }

        err = qt::InsertMediaIntoTrack(
            _track,
            0,
            0,
            qt::GetMediaDuration(_media),
            qt_fixed1);

        if (err != qt::noErr)
        {
            throw_error(
                name(), String_Format(error_media_add).arg(_file).arg(err));
        }

        /*short id = qt::movieInDataForkResID;
        err = qt::AddMovieResource(_movie, _f, &id, 0);

        if (err != qt::noErr)
            throw_error(
                name(), String_Format(error_movie_add).arg(_file).arg(err));*/

        qt::AddMovieToStorage(_movie, _f);
    }

    _track = 0;
    _media = 0;

    //if (_f) { qt::CloseMovieFile(_f); _f = 0; }
    
    if (_f)
    {
        qt::CloseMovieStorage(_f);
        
        _f = 0;
    }

    if (_movie)
    {
        qt::DisposeMovie(_movie);
        
        _movie = 0;
    }

    if (_gworld)
    {
        qt::DisposeGWorld(_gworld);
        
        _gworld = 0;
    }
}

namespace
{

struct _Lock_Pixels
{
    _Lock_Pixels(qt::PixMapHandle in) :
        _p(in)
    {
        LockPixels(_p);
    }

    ~_Lock_Pixels()
    {
        UnlockPixels(_p);
    }

private:

    qt::PixMapHandle _p;
};

const String
    error_compress = "Cannot compress frame %%: %% (#%%)";

} // namespace

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::save");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("frame = " << frame);

    // Copy the input to the temporary buffer.

    Gl_Image::copy(in, _image);

    // Compress the image.

    const qt::PixMapHandle pixmap =
#if defined(DJV_WINDOWS)
        _gworld->portPixMap;
#else
        qt::GetPortPixMap(_gworld);
#endif
    const qt::Rect rect = { 0, 0, _image.h(), _image.w() };
    long compress_max = 0;

    qt::OSErr err = qt::GetMaxCompressionSize(
        pixmap,
        &rect,
        0,
        quicktime_quality(_options.quality),
        quicktime_codec(_options.codec),
        (qt::CompressorComponent)qt_anyCodec,
        &compress_max);

    if (err != qt::noErr)
        throw_error(name(), String_Format(error_compress).
            arg(frame.frame).arg(_file).arg(err));

    //DJV_DEBUG_PRINT("compress max = " << static_cast<int>(compress_max));

    Handle compress;
    Handle description;
    compress.init(compress_max);
    description.init(4);

    if (! compress.p || ! description.p)
        throw_error(name(), String_Format(error_compress).
            arg(frame.frame).arg(_file).arg(err));

    _Lock_Pixels lock_pixels(pixmap);

    err = qt::CompressImage(
        pixmap,
        &rect,
        quicktime_quality(_options.quality),
        quicktime_codec(_options.codec),
        (qt::ImageDescriptionHandle)description.p,
        *compress.p);

    if (err != qt::noErr)
        throw_error(name(), String_Format(error_compress).
            arg(frame.frame).arg(_file).arg(err));

    // Write the image.

    err = qt::AddMediaSample(
        _media,
        compress.p,
        0,
        ((qt::ImageDescription *)*description.p)->dataSize,
        _frame_duration,
        (qt::SampleDescriptionHandle)description.p,
        1,
        0,
        0);

    if (err != qt::noErr)
        throw_error(name(), String_Format(error_media_sample).
            arg(frame.frame).arg(_file).arg(err));
}

} // djv_quicktime

