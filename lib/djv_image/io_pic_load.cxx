/*------------------------------------------------------------------------------
 lib/djv_image/io_pic_load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_pic.h"

namespace djv_image {
namespace io_pic {

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
  _type(TYPE(0))
{
  _io.endian(memory::endian() != memory::MSB);
  
  _compression[0] = false;
  _compression[1] = false;
}

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return io_pic::name;
}

List<String> Load::extension_list() const
{
  return io_pic::extension_list;
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
  uint32_t magic;
  float version;
  char comment [80];
  char id [4];
  uint16_t width, height;
  float ratio;
  uint16_t fields;
  uint8_t pad [2];
};

enum CHANNEL
{
  CHANNEL_A = 0x10,
  CHANNEL_B = 0x20,
  CHANNEL_G = 0x40,
  CHANNEL_R = 0x80
};
  
struct Channel
{
  uint8_t chained;
  uint8_t size;
  uint8_t type;
  uint8_t channel;
};

void _channel(File_Io & io, Channel * out)
{
  io.get_u8(&out->chained);
  io.get_u8(&out->size);
  io.get_u8(&out->type);
  io.get_u8(&out->channel);
}

String debug_channel(int in)
{
  String out;
  if (in & CHANNEL_R) out += 'R';
  if (in & CHANNEL_G) out += 'G';
  if (in & CHANNEL_B) out += 'B';
  if (in & CHANNEL_A) out += 'A';
  return out;
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
  memory::zero(&header, sizeof(Header));
  
  _io.get_u32(&header.magic);
  if (header.magic != 0x5380F634)
    io::throw_error_unrecognized(name(), in);

  _io.get_f32(&header.version);
  _io.get(header.comment, sizeof(header.comment));
  _io.get(header.id, sizeof(header.id));
  _io.get_u16(&header.width);
  _io.get_u16(&header.height);
  _io.get_f32(&header.ratio);
  _io.get_u16(&header.fields);
  _io.seek(sizeof(header.pad));
  
  DJV_DEBUG_PRINT("version = " << header.version);
  DJV_DEBUG_PRINT("comment = " << String(header.comment, sizeof(header.comment)));
  DJV_DEBUG_PRINT("id = " << String(header.id, sizeof(header.id)));
  DJV_DEBUG_PRINT("width = " << header.width);
  DJV_DEBUG_PRINT("height = " << header.height);
  DJV_DEBUG_PRINT("ratio = " << header.ratio);
  DJV_DEBUG_PRINT("fields = " << header.fields);

  if (String(header.id, sizeof(header.id)) != "PICT")
    io::throw_error_unsupported(name(), in);
  
  // Information.
  
  info.file_name = in;
  
  info.size = V2i(header.width, header.height);
  
  info.mirror.y = true;

  Channel channel;
  memory::zero(&channel, sizeof(Channel));
  _channel(_io, &channel);
  _compression[0] = 2 == channel.type;
  
  DJV_DEBUG_PRINT("channel = " << debug_channel(channel.channel));
  
  int type = -1;
  if (
    (CHANNEL_R & channel.channel) &&
    (CHANNEL_G & channel.channel) &&
    (CHANNEL_B & channel.channel) &&
    (CHANNEL_A & channel.channel) &&
    8 == channel.size &&
    ! channel.chained
  ) {
    type = TYPE_RGBA;
  }
  else if (
    (CHANNEL_R & channel.channel) &&
    (CHANNEL_G & channel.channel) &&
    (CHANNEL_B & channel.channel) &&
    8 == channel.size &&
    ! channel.chained
  ) {
    type = TYPE_RGB;
  }
  else if (
    (CHANNEL_R & channel.channel) &&
    (CHANNEL_G & channel.channel) &&
    (CHANNEL_B & channel.channel) &&
    8 == channel.size &&
    channel.chained
  ) {
    memory::zero(&channel, sizeof(Channel));
    
    _channel(_io, &channel);
    _compression[1] = 2 == channel.type;
    
    DJV_DEBUG_PRINT("channel = " << debug_channel(channel.channel));
    
    if (
      ! (CHANNEL_R & channel.channel) &&
      ! (CHANNEL_G & channel.channel) &&
      ! (CHANNEL_B & channel.channel) &&
        (CHANNEL_A & channel.channel) &&
      8 == channel.size &&
      ! channel.chained
    ) {
      type = TYPE_RGB_A;
    }
  }
  
  if (-1 == type)
    io::throw_error_unsupported(name(), in);

  _type = TYPE(type);
  switch (_type)
  {
    case TYPE_RGB: info.pixel = pixel::RGB_U8; break;
    case TYPE_RGBA:
    case TYPE_RGB_A: info.pixel = pixel::RGBA_U8; break;
  }
  
  // Image tags.
  
  if (header.comment[0])
    info.tag[tag::label_tag()[tag::DESCRIPTION]] =
      String(header.comment, sizeof(header.comment));

  DJV_DEBUG_PRINT("info = " << info);
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
  _image.tag = info.tag;

  // Read.
    
  _io.read_ahead();
  
  Data * data = frame.proxy ? &_tmp : &_image;
  data->set(info);
  
  const int channels = pixel::channels(info.pixel);
  const int bytes = pixel::channel_bytes(info.pixel);
  const bool endian = _io.endian();
  
  const uint8_t * p = _io.mmap_p(), * const end = _io.mmap_end();
  for (int y = 0; y < info.size.y; ++y)
  {
    DJV_DEBUG_PRINT("y = " << y);
    
    switch (_type)
    {
      case TYPE_RGB:
      case TYPE_RGBA:
      
        if (_compression[0])
        {
          p = rle_load(
            p,
            end,
            data->data(0, y),
            info.size.x,
            channels,
            channels * bytes,
            endian
          );
          
          if (! p)
            io::throw_error_read(name(), file_name);
        }
        else
        {
          const int size = info.size.x * channels * bytes;
          memory::copy(p, data->data(0, y), size);
          
          p += size;
        }
        
        break;

      case TYPE_RGB_A:
      
        if (_compression[0])
        {
          p = rle_load(
            p,
            end,
            data->data(0, y),
            info.size.x,
            3,
            channels * bytes,
            endian
          );
          
          if (! p)
            io::throw_error_read(name(), file_name);
        }
        else
        {
          const int size = info.size.x * 3 * bytes;
          memory::copy(p, data->data(0, y), size);
          
          p += size;
        }
        if (_compression[1])
        {
          p = rle_load(
            p,
            end,
            data->data(0, y) + 3 * bytes,
            info.size.x,
            1,
            channels * bytes,
            endian
          );
          
          if (! p)
            io::throw_error_read(name(), file_name);
        }
        else
        {
          const int size = info.size.x * 1 * bytes;
          memory::copy(p, data->data(0, y), size);
          
          p += size;
        }
        
        break;
    }
  }

  if (frame.proxy)
  {
    info.size = data::proxy_scale(info.size, frame.proxy);
    info.proxy = frame.proxy;
    _image.set(info);
    
    data::proxy_scale(_tmp, &_image, frame.proxy);
  }
  
  DJV_DEBUG_PRINT("image = " << _image);
  
  return &_image;
}

//------------------------------------------------------------------------------

}
}
