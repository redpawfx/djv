/*------------------------------------------------------------------------------
 lib/djv_image/io_rla_load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_rla.h"

namespace djv_image {
namespace io_rla {

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load()
{
  _io.endian(memory::endian() != memory::MSB);
}

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return io_rla::name;
}

List<String> Load::extension_list() const
{
  return io_rla::extension_list;
}

//------------------------------------------------------------------------------
// Load::open(), Load::_open()
//------------------------------------------------------------------------------

void Load::open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::open");
  DJV_DEBUG_PRINT("in = " << in);

  _file = in;
  _open(_file.get(_file.seq().start()), info);

  if (file::SEQ == _file.type())
    info.time.list = _file.seq().list;
}

namespace {

struct Header
{
  int16_t dimensions [4]; // Left, right, bottom, top.
  int16_t active [4];
  int16_t frame;
  int16_t color_channel_type;
  int16_t color_channels, matte_channels, aux_channels;
  int16_t version;
  char gamma [16];
  char chroma [3][24], whitepoint [24];
  int32_t job;
  char file_name [128];
  char description [128];
  char progam [64];
  char machine [32];
  char user [32];
  char date [20];
  char aspect [24], aspect_ratio [8];
  char color_format [32];
  int16_t field;
  char render_time [12];
  char filter [32];
  int16_t color_bit_depth;
  int16_t matte_channel_type;
  int16_t matte_bit_depth;
  int16_t aux_channel_type;
  int16_t aux_bit_depth;
  char aux_format [32];
  char pad [36];
  int32_t offset;
};

void endian(Header * in)
{
  memory::endian(&in->dimensions, 4, 2);
  memory::endian(&in->active, 4, 2);
  memory::endian(&in->frame, 1, 2);
  memory::endian(&in->color_channel_type, 1, 2);
  memory::endian(&in->color_channels, 1, 2);
  memory::endian(&in->matte_channels, 1, 2);
  memory::endian(&in->aux_channels, 1, 2);
  memory::endian(&in->version, 1, 2);
  memory::endian(&in->job, 1, 4);
  memory::endian(&in->field, 1, 2);
  memory::endian(&in->color_bit_depth, 1, 2);
  memory::endian(&in->matte_channel_type, 1, 2);
  memory::endian(&in->matte_bit_depth, 1, 2);
  memory::endian(&in->aux_channel_type, 1, 2);
  memory::endian(&in->aux_bit_depth, 1, 2);
  memory::endian(&in->offset, 1, 4);
}

void debug(const Header & in)
{
  DJV_DEBUG("debug(Header)");
  DJV_DEBUG_PRINT("dimensions = " << in.dimensions[0] << " " << in.dimensions[1] <<
    " " << in.dimensions[2] << " " << in.dimensions[3]);
  DJV_DEBUG_PRINT("active = " << in.active[0] << " " << in.active[1] << " " <<
    in.active[2] << " " << in.active[3]);
  DJV_DEBUG_PRINT("frame = " << in.frame);
  DJV_DEBUG_PRINT("color channel type = " << in.color_channel_type);
  DJV_DEBUG_PRINT("color channels = " << in.color_channels);
  DJV_DEBUG_PRINT("matte channels = " << in.matte_channels);
  DJV_DEBUG_PRINT("aux channels = " << in.aux_channels);
  DJV_DEBUG_PRINT("version = " << in.version);
  DJV_DEBUG_PRINT("gamma = " << in.gamma);
  DJV_DEBUG_PRINT("chroma = " << in.chroma[0] << " " << in.chroma[1] << " " <<
    in.chroma[2]);
  DJV_DEBUG_PRINT("whitepoint = " << in.whitepoint);
  DJV_DEBUG_PRINT("job = " << in.job);
  DJV_DEBUG_PRINT("file name = " << in.file_name);
  DJV_DEBUG_PRINT("description = " << in.description);
  DJV_DEBUG_PRINT("progam = " << in.progam);
  DJV_DEBUG_PRINT("machine = " << in.machine);
  DJV_DEBUG_PRINT("user = " << in.user);
  DJV_DEBUG_PRINT("date = " << in.date);
  DJV_DEBUG_PRINT("aspect = " << in.aspect);
  DJV_DEBUG_PRINT("aspect ratio = " << in.aspect_ratio);
  DJV_DEBUG_PRINT("color format = " << in.color_format);
  DJV_DEBUG_PRINT("field = " << in.field);
  DJV_DEBUG_PRINT("render time = " << in.render_time);
  DJV_DEBUG_PRINT("filter = " << in.filter);
  DJV_DEBUG_PRINT("color bit depth = " << in.color_bit_depth);
  DJV_DEBUG_PRINT("matte channel type = " << in.matte_channel_type);
  DJV_DEBUG_PRINT("matte bit depth = " << in.matte_bit_depth);
  DJV_DEBUG_PRINT("aux channel type = " << in.aux_channel_type);
  DJV_DEBUG_PRINT("aux bit depth = " << in.aux_bit_depth);
  DJV_DEBUG_PRINT("aux format = " << in.aux_format);
  DJV_DEBUG_PRINT("offset = " << in.offset);
}

}

void Load::_open(const String & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::_open");
  DJV_DEBUG_PRINT("in = " << in);
    
  // Open.
  
  _io.open(in, file_io::READ);
  
  // Header.
  
  Header header;
  
  DJV_DEBUG_PRINT("header size = " << static_cast<int>(sizeof(Header)));
  
  _io.get(&header, sizeof(Header));
  if (_io.endian())
    endian(&header);
  
  debug(header);
  
  const int w = header.active[1] - header.active[0] + 1;
  const int h = header.active[3] - header.active[2] + 1;
  
  // Scanline table.
  
  _rle_offset.size(h);
  _io.get_32(_rle_offset(), h);

  // Information.
  
  const V2i size(w, h);
  PIXEL pixel(PIXEL(0));

  if (header.matte_channels > 1)
    io::throw_error_unsupported(name(), in);
  
  if (header.matte_channel_type != header.color_channel_type)
    io::throw_error_unsupported(name(), in);

  if (header.matte_bit_depth != header.color_bit_depth)
    io::throw_error_unsupported(name(), in);
  
  if (
    ! pixel::pixel_(
      header.color_channels + header.matte_channels,
      header.color_bit_depth,
      3 == header.color_channel_type,
      &pixel
    )
  )
    io::throw_error_unsupported(name(), in);

  if (header.field)
    io::throw_error_unsupported(name(), in);
  
  info = Info(in, size, pixel);
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

const Image * Load::get(const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Load::get");
  DJV_DEBUG_PRINT("frame = " << frame);
  
  // Open.
  
  const String file_name =
    _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());
  
  DJV_DEBUG_PRINT("file name = " << file_name);
  
  io::Info info;
  _open(file_name, info);

  if (
    frame.layer < 0 ||
    frame.layer >= static_cast<int>(info.layer_size())
  )
    io::throw_error_read(name(), file_name);

  Info _info = info[frame.layer];

  // Read.

  _io.read_ahead();
  
  Data * p = frame.proxy ? &_tmp : &_image;
  p->set(_info);
  
  const int w = _info.size.x, h = _info.size.y;
  const int channels = pixel::channels(_info.pixel);
  const int bytes = pixel::channel_bytes(_info.pixel);
  
  DJV_DEBUG_PRINT("channels = " << channels);
  DJV_DEBUG_PRINT("bytes = " << bytes);
  
  uint8_t * data_p = p->data();
  for (int y = 0; y < h; ++y, data_p += w * channels * bytes)
  {    
    _io.position(_rle_offset()[y]);
    
    for (int c = 0; c < channels; ++c)
      if (pixel::F32 == pixel::type(_info.pixel))
        float_load(_io, data_p + c * bytes, w, channels);
      else
        rle_load(_io, data_p + c * bytes, w, channels, bytes);
  }

  if (frame.proxy)
  {
    _info.size = data::proxy_scale(_info.size, frame.proxy);
    _info.proxy = frame.proxy;
    _image.set(_info);
    
    data::proxy_scale(_tmp, &_image, frame.proxy);
  }

  DJV_DEBUG_PRINT("image = " << _image);
  
  return &_image;
}

//------------------------------------------------------------------------------

}
}
