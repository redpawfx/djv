/*------------------------------------------------------------------------------
 lib/djv_image/io_iff_save.cxx

 Copyright (c) 2008 Mikael Sundell
 Copyright (c) 2004-2008 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_iff.h"

#include <djv_image/gl_data.h>

namespace djv_image {
namespace io_iff {

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save()
{
  _io.endian(memory::endian() != memory::MSB);
}

plugin::Plugin * Save::copy() const
{
  Save * out = new Save;
  out->_option = _option;
  return out;
}

String Save::name() const
{
  return io_iff::name;
}

List<String> Save::extension_list() const
{
  return io_iff::extension_list;
}

//------------------------------------------------------------------------------
// Save::option(), Save::option_list(), Save::option_value(),
// Save::option_default()
//------------------------------------------------------------------------------

bool Save::option(const String & in, String * data)
{
  try
  {
    if (string::compare_no_case(in, option_list()[SAVE_COMPRESSION]))
      *data >> _option.compression;
  }
  catch (String)
  {
    return false;
  }
  
  return true;
}

String Save::option(const String & in) const
{
  String out;
  
  if (string::compare_no_case(in, option_list()[SAVE_COMPRESSION]))
    out << _option.compression;
  
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
      case SAVE_COMPRESSION: out = label_compression(); break;
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
      case SAVE_COMPRESSION: out << _option.compression; break;
    }
  
  return out;
}

//------------------------------------------------------------------------------
// Save::open(), Save::_open()
//------------------------------------------------------------------------------

void Save::open(const File & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::open");
  DJV_DEBUG_PRINT("in = " << in);
  
  _file = in;
  if (info.time.list.size() > 1)
    _file.type(file::SEQ);
  
  _info = Info();
  _info.size = info.size;
  _info.endian = memory::MSB;
  
  pixel::FORMAT format = pixel::format(info.pixel);
  switch (format)
  {
    case pixel::L: format = pixel::RGB; break;
    case pixel::LA: format = pixel::RGBA; break;
    default: break;
  }

  pixel::TYPE type = pixel::type(info.pixel);
  switch (type)
  {
    case pixel::U10:
#if defined (DJV_OPENEXR)
    case pixel::F16:
#endif
    case pixel::F32: type = pixel::U16; break;
    default: break;
  }
  
  _info.pixel = pixel::pixel(format, type);
  _info.endian = memory::MSB;
  
  DJV_DEBUG_PRINT("info = " << _info);

  _image.set(_info);
}

void Save::_open(const String & in) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);

  _io.open(in, file_io::WRITE);

  info_save(_io, _info, _option.compression != COMPRESSION_NONE);

}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Open.

  _open(_file.get(frame.frame));
  
  // Information.
  
  const Data * p = &in;
  
  if (in.info() != _info)
  {
    DJV_DEBUG_PRINT("convert = " << _image);
    
    _image.zero();
    
    gl_data::copy(in, &_image);
    
    p = &_image;
  }
  
  const int w = p->w(), h = p->h();
  const int channels = pixel::channels(p->info().pixel); 
  const int channel_bytes = pixel::channel_bytes(p->info().pixel); 
  const int bytes = pixel::bytes(p->info().pixel);
  
  bool compress = _option.compression ? true : false;
  
  uint32_t length = 0;
  
  DJV_DEBUG_PRINT("channels = " << channels);  
  DJV_DEBUG_PRINT("channel_bytes = " << channel_bytes);  
  DJV_DEBUG_PRINT("bytes = " << bytes);  
  
  // Write.

  size_t pos = 0;
  pos = _io.position();

  DJV_DEBUG_PRINT("position = " << static_cast<int>(pos));
  
  // 'FOR4' type
  _io.set_u8('F');
  _io.set_u8('O');
  _io.set_u8('R');
  _io.set_u8('4');
  
  // 'FOR4' length
  // NOTE: only reserved for now.
  _io.set_u32(length);
  
  // 'TBMP' type
  _io.set_u8('T');
  _io.set_u8('B');
  _io.set_u8('M');
  _io.set_u8('P');
  
  // Write tiles.
  
  V2i size = tile_size (w, h);
  
  // Y order.
  
  for (int y=0; y<size.y; y++)
  {
    // X order.
    
    for (int x=0; x<size.x; x++)
    {
      // Set tile coordinates.
      uint16_t xmin, xmax, ymin, ymax;
      
      // Set xmin and xmax.
      xmin = x * tile_width();
      xmax = math::min(xmin + tile_width(), w) - 1;
      
      // Set ymin and ymax.
      ymin = y * tile_height();
      ymax = math::min(ymin + tile_height(), h) - 1;
    
      // Set width and height.
      uint32_t tw = xmax - xmin + 1;
      uint32_t th = ymax - ymin + 1;
      
      // Set type.
      _io.set_u8('R');
      _io.set_u8('G');
      _io.set_u8('B');
      _io.set_u8('A');

      // Length.
      uint32_t length = tw * th * bytes;

      // Tile length.
      uint32_t tile_length = length;
      
      // Align.
      length = align_size(length, 4);
          
      // Append xmin, xmax, ymin and ymax.
      length += 8;
      
      // Tile compression.
      bool tile_compress = compress;
      
      // Set bytes.
      memory::Buffer<uint8_t> tile(tile_length);
      uint8_t * out_p = tile();
      bool uncomp = false;
      // Handle 8-bit data.
      if (p->info().pixel == pixel::RGB_U8 || p->info().pixel == pixel::RGBA_U8)
      {
        if (tile_compress)
        {
          int index = 0, size = 0;
          
          // Set bytes.
          // NOTE: prevent buffer overrun.
          memory::Buffer<uint8_t> tmp(tile_length * 2);
          
          // Map: RGB(A)8 RGBA to BGRA
          for (int c =(channels * channel_bytes) - 1; c>=0; --c)
          {
            memory::Buffer<uint8_t> in(tw * th);
            uint8_t * in_p = in();
          
            // Data.
          
            for (uint16_t py=ymin; py<=ymax; py++)
            {
              const uint8_t * in_dy = p->data(0, py);
              for (uint16_t px=xmin; px<=xmax; px++)
              {
                // Get pixel.
                uint8_t pixel;
                const uint8_t * in_dx = in_dy + px * bytes + c;
                memory::copy(in_dx, &pixel, 1);
                // Set pixel.
                *in_p++ = pixel;
              }
            }
            
            // Compress

            size = rle_save(in(), tmp() + index, tw * th);
            index += size;
          }
          
          // If size exceeds tile length use uncompressed.
          
          if (index < tile_length)
          {
            memory::copy(tmp(), tile(), index);
            
            // Set tile length.
            tile_length = index;

            // Append xmin, xmax, ymin and ymax.
            length = index + 8;

            // Set length.
            int align = align_size (length, 4);
            if (align > length)
            {
              out_p = tile() + index;
              // Pad.
              for (int i=0; i<align-length; i++)
              {
                *out_p++ = '\0';
                tile_length++;
              }
            }
          }
          else
          {
            tile_compress = false;
          }
        }
        if (!tile_compress) 
        {
          for (uint16_t py=ymin; py<=ymax; py++)
          {
            const uint8_t * in_dy = p->data(0, py);
            for (uint16_t px=xmin; px<=xmax; px++)
            { 
              // Map: RGB(A)8 RGBA to BGRA
              for (int c=channels - 1; c>=0; --c)
              { 
                // Get pixel.
                uint8_t pixel;
                const uint8_t * in_dx = in_dy + px * bytes + c * channel_bytes;
                memory::copy(in_dx, &pixel, 1);
                // Set pixel.
                *out_p++ = pixel;
              }
            }
          }
        }
      }
      // Handle 16-bit data.
      else if (p->info().pixel == pixel::RGB_U16 || p->info().pixel == pixel::RGBA_U16)
      { 
        if (tile_compress)
        {
          int index = 0, size = 0;
          
          // Set bytes.
          // NOTE: prevent buffer overrun.
          memory::Buffer<uint8_t> tmp(tile_length * 2);
          
          // Set map.
          int* map = NULL;
          if (memory::endian () == memory::LSB)
          {
            int rgb16[] = { 0, 2, 4, 1, 3, 5 };
            int rgba16[] = { 0, 2, 4, 7, 1, 3, 5, 6 };
            if (_info.pixel == pixel::RGB_U16)
              map = rgb16;
            else
              map = rgba16;
          }
          else
          {
            int rgb16[] = { 1, 3, 5, 0, 2, 4 };
            int rgba16[] = { 1, 3, 5, 7, 0, 2, 4, 6 };
            if (_info.pixel == pixel::RGB_U16)
              map = rgb16;
            else
              map = rgba16;
          }
          
          // Map: RGB(A)16 RGBA to BGRA
          for (int c =(channels * channel_bytes) - 1; c>=0; --c)
          {
            int mc = map[c];
            memory::Buffer<uint8_t> in(tw * th);
            uint8_t * in_p = in();
          
            // Data.
          
            for (uint16_t py=ymin; py<=ymax; py++)
            {
              const uint8_t * in_dy = p->data(0, py);
              for (uint16_t px=xmin; px<=xmax; px++)
              {
                // Get pixel.
                uint8_t pixel;
                const uint8_t * in_dx = in_dy + px * bytes + mc;
                memory::copy(in_dx, &pixel, 1);
                // Set pixel.
                *in_p++ = pixel;
              }
            }
            
            // Compress

            size = rle_save(in(), tmp() + index, tw * th);
            index += size;
          }
          
          // If size exceeds tile length use uncompressed.
          
          if (index < tile_length)
          {
            memory::copy(tmp(), tile(), index);
            
            // Set tile length.
            tile_length = index;

            // Append xmin, xmax, ymin and ymax.
            length = index + 8;

            // Set length.
            int align = align_size (length, 4);
            if (align > length)
            {
              out_p = tile() + index;
              // Pad.
              for (int i=0; i<align-length; i++)
              {
                *out_p++ = '\0';
                tile_length++;
              }
            }
          }
          else
          {
            tile_compress = false;
          }
        }
        if (!tile_compress) 
        {
          for (uint16_t py=ymin; py<=ymax; py++)
          {
            const uint8_t * in_dy = p->data(0, py);
            for (uint16_t px=xmin; px<=xmax; px++)
            {
              // Map: RGB(A)16 RGBA to BGRA
              for (int c=channels - 1; c>=0; --c)
              { 
                uint16_t pixel;
                const uint8_t * in_dx = in_dy + px * bytes + c * channel_bytes;
                if (memory::endian () == memory::LSB)
                  memory::endian(in_dx, &pixel, 1, 2);
                else
                  memory::copy(in_dx, &pixel, 2);
                // Set pixel.
                *out_p++ = pixel;
                *out_p++;
              }
            }
          }
        }
      }
        
      // Set length.
      _io.set_u32(length);
      
      // Set xmin, xmax, ymin and ymax.
      _io.set_u16(xmin);
      _io.set_u16(ymin);
      _io.set_u16(xmax);
      _io.set_u16(ymax);

      // Write.
      _io.set(tile(), tile_length); 
    }
  }

  // Set FOR4 CIMG and FOR4 TBMP size
  uint32_t p0 = _io.position() - 8;
  uint32_t p1 = p0 - pos;
  
  // NOTE: FOR4 <size> CIMG
  _io.position (4);
  _io.set_u32 (p0);
  
  // NOTE: FOR4 <size> TBMP
  _io.position (pos + 4);
  _io.set_u32 (p1);
  _io.close();
}

//------------------------------------------------------------------------------

}
}
