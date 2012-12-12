/*------------------------------------------------------------------------------
 plugin/djv_quicktime/save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "save.h"

#include <djv_image/gl_data.h>

namespace qt {
#if defined(DJV_MACOSX)
#elif defined(DJV_WINDOWS)
#include <ImageCompression.h>
#include <QTML.h>
#endif
#define qt_anyCodec ((qt::CodecComponent)0) // XXX
}

namespace djv_plugin {
namespace quicktime {

//------------------------------------------------------------------------------
// djv_image_save()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_save()
{
  return new Save;
}

}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save() :
  _f(0),
  _frame_duration(0),
  _movie(0),
  _track(0),
  _media(0),
  _gworld(0)
{}

void Save::init() throw (Error)
{
  quicktime_init();
}

void Save::del()
{
  quicktime_del();
}

plugin::Plugin * Save::copy() const
{
  Save * out = new Save;
  out->_option = _option;
  return out;
}

String Save::name() const
{
  return quicktime::name;
}

List<String> Save::extension_list() const
{
  return quicktime::extension_list;
}

bool Save::is_seq() const
{
  return false;
}

//------------------------------------------------------------------------------
// Save::option(), Save::option_list(), Save::option_quality(),
// Save::option_default()
//------------------------------------------------------------------------------

bool Save::option(const String & in, String * data)
{
  const List<String> & list = option_list();
  try
  {
    if (string::compare_no_case(in, list[SAVE_CODEC]))
      *data >> _option.codec;
    else if (string::compare_no_case(in, list[SAVE_QUALITY]))
      *data >> _option.quality;
  }
  catch (String)
  {
    return false;
  }
  
  return true;
}

String Save::option(const String & in) const
{
  const List<String> & list = option_list();
  
  String out;
  
  if (string::compare_no_case(in, list[SAVE_CODEC]))
    out << _option.codec;
  else if (string::compare_no_case(in, list[SAVE_QUALITY]))
    out << _option.quality;
  
  return out;
}

List<String> Save::option_list() const
{
  return label_save_option();
}

List<String> Save::option_value(const String & in) const
{
  List<String> out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_save_option(), &index))
    switch (index)
    {
      case SAVE_CODEC: out = label_codec(); break;
      case SAVE_QUALITY: out = label_quality(); break;
    }
  
  return out;
}

String Save::option_default(const String & in) const
{
  String out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_save_option(), &index))
    switch (index)
    {
      case SAVE_CODEC: out << _option.codec; break;
      case SAVE_QUALITY: out << _option.quality; break;
    }
  
  return out;
}

//------------------------------------------------------------------------------
// Save::open(), Save::_open(), Save::close()
//------------------------------------------------------------------------------

void Save::open(const File & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::open");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("info = " << info);
  
  _open(in, info);
  
  _track = qt::NewMovieTrack(
    _movie,
    qt::FixRatio(info.size.x, 1),
    qt::FixRatio(info.size.y, 1),
    qt::kNoVolume
  );
  if (! _track)
    io::throw_error(name(), string::Format(error_track).arg(in).
      arg(qt::GetMoviesError()));
  
  _media = qt::NewTrackMedia(
    _track,
    qt::VideoMediaType,
    info.time.speed.scale,
    0,
    0
  );
  if (! _media)
    io::throw_error(name(), string::Format(error_media).arg(in).
      arg(qt::GetMoviesError()));
  
  qt::OSErr err = qt::BeginMediaEdits(_media);
  if (err != qt::noErr)
    io::throw_error(name(), string::Format(error_media_begin).arg(in).
      arg(err));
  
  // Information.
  
  _file = in;

  _frame_duration = info.time.speed.duration;

  // Temporary buffers.
  
  Info _info;
  _info.size = info.size;
  _info.mirror.y = true;
  _info.pixel = pixel::RGBA_U8;
  _image.set(_info);
  
  const qt::Rect rect = { 0, 0, _info.size.y, _info.size.x };
  err = qt::NewGWorldFromPtr(
    &_gworld,
    qt::k32RGBAPixelFormat,
    &rect,
    0,
    0,
    0,
    (char *)_image.data(0, 0),
    _info.size.x * 4
  );
  if (err != qt::noErr)
    io::throw_error(name(), string::Format(error_gworld).arg(in).arg(err));

  qt::SetMovieGWorld(_movie, _gworld, 0);
}

void Save::_open(const String & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("info = " << info);

  /*qt::FSSpec file_spec;
#if defined(DJV_MACOSX)
  qt::String255 file_name;
  qt::c2pstrcpy(file_name, file::path_absolute(in.get()).c_str());
  qt::FSMakeFSSpec(0, 0, file_name, &file_spec);
#elif defined(DJV_WINDOWS)
  char file_name [string::cstring_size];
  string::cstring(file::path_absolute(in), file_name, string::cstring_size);
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
    io::throw_error(name(), string::Format(error_open).arg(in).arg(err));*/

  Handle handle;
  qt::OSType handle_type;
  qt::OSErr err = qt::QTNewDataReferenceFromFullPathCFString(
    CFStringRef(file::path_absolute(in)).p,
    qt::kQTNativeDefaultPathStyle,
    //qt::kQTPOSIXPathStyle,
    0,
    &handle.p,
    &handle_type
  );
  if (err != qt::noErr)
    io::throw_error(name(), string::Format(error_data_ref).arg(in).arg(err));

  err = qt::CreateMovieStorage(
    handle.p,
    handle_type,
    FOUR_CHAR_CODE('TVOD'),
    qt::smCurrentScript,
    qt::createMovieFileDeleteCurFile | qt::createMovieFileDontCreateResFile,
    &_f,
    &_movie
  );
  if (err != qt::noErr)
    io::throw_error(name(), string::Format(error_open).arg(in).arg(err));
}

void Save::close() throw (Error)
{
  DJV_DEBUG("Save::close");
  
  if (_track && _media)
  {
    DJV_DEBUG_PRINT("end");

    qt::OSErr err = qt::EndMediaEdits(_media);
    if (err != qt::noErr)
      io::throw_error(name(), string::Format(error_media_end).arg(_file).
        arg(err));
    
    err = qt::InsertMediaIntoTrack(
      _track,
      0,
      0,
      qt::GetMediaDuration(_media),
      qt_fixed1
    );
    if (err != qt::noErr)
      io::throw_error(name(), string::Format(error_media_add).arg(_file).
        arg(err));
    
    /*short id = qt::movieInDataForkResID;
    err = qt::AddMovieResource(_movie, _f, &id, 0);
    
    if (err != qt::noErr)
      io::throw_error(name(), string::Format(error_movie_add).arg(_file).
        arg(err));
    */
    
    qt::AddMovieToStorage(_movie, _f);
  }
  _track = 0;
  _media = 0;
  
  //if (_f) { qt::CloseMovieFile(_f); _f = 0; }
  if (_f) { qt::CloseMovieStorage(_f); _f = 0; }
  if (_movie) { qt::DisposeMovie(_movie); _movie = 0; }
  if (_gworld) { qt::DisposeGWorld(_gworld); _gworld = 0; }
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

namespace {

struct _Lock_Pixels
{
  _Lock_Pixels(qt::PixMapHandle in) :
    _p(in)
  {
    qt::LockPixels(_p);
  }

  ~_Lock_Pixels()
  {
    qt::UnlockPixels(_p);
  }

private:

  qt::PixMapHandle _p;
};

}

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("frame = " << frame);
  
  // Copy input to temporary buffer.
  
  gl_data::copy(in, &_image);
  
  // Compress image.

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
    quicktime_quality(_option.quality),
    quicktime_codec(_option.codec),
    (qt::CompressorComponent)qt_anyCodec,
    &compress_max
  );
  if (err != qt::noErr)
    io::throw_error(name(), string::Format(error_compress).
      arg(frame.frame).arg(_file).arg(err));

  DJV_DEBUG_PRINT("compress max = " << static_cast<int>(compress_max));

  Handle compress;
  Handle description;
  compress.init(compress_max);
  description.init(4);
  if (! compress.p || ! description.p)
    io::throw_error(name(), string::Format(error_compress).
      arg(frame.frame).arg(_file).arg(err));

  _Lock_Pixels lock_pixels(pixmap);

  err = qt::CompressImage(
    pixmap,
    &rect,
    quicktime_quality(_option.quality),
    quicktime_codec(_option.codec),
    (qt::ImageDescriptionHandle)description.p,
    *compress.p
  );
  if (err != qt::noErr)
    io::throw_error(name(), string::Format(error_compress).
      arg(frame.frame).arg(_file).arg(err));
  
  // Write image.
  
  err = qt::AddMediaSample(
    _media,
    compress.p,
    0,
    ((qt::ImageDescription *)*description.p)->dataSize,
    _frame_duration,
    (qt::SampleDescriptionHandle)description.p,
    1,
    0,
    0
  );
  if (err != qt::noErr)
    io::throw_error(name(), string::Format(error_media_sample).
      arg(frame.frame).arg(_file).arg(err));
}

//------------------------------------------------------------------------------

}
}

