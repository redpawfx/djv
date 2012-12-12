/*------------------------------------------------------------------------------
 lib/djv_image/io_iff_load.cxx

 Copyright (c) 2008-2009 Mikael Sundell
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_iff.h"

namespace djv_image {
namespace io_iff {

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
  _tiles(0),
  _compression(false)
{
  _io.endian(memory::endian() != memory::MSB);
}

plugin::Plugin * Load::copy() const { return new Load; }

String Load::name() const { return io_iff::name; }

List<String> Load::extension_list() const
{
  return io_iff::extension_list;
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

void Load::_open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::_open");
  DJV_DEBUG_PRINT("in = " << in);
 
  _io.open(in, file_io::READ);
 
  info.file_name = in;
  info_load(_io, info, &_tiles, &_compression);
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

const Image * Load::get(const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Load::get");
  DJV_DEBUG_PRINT("frame = " << frame);
  
  uint8_t type[4];
  uint8_t pixels[32];
  
  uint32_t size;
  uint32_t chunk_size;
  uint32_t tiles_rgba;
  
  // Open.
  
  const String file_name =
    _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());
  
  DJV_DEBUG_PRINT("file name = " << file_name);
  
  io::Info info;
  _open(file_name, info);
  _image.tag = info.tag;
  
  // Information.
  
  const int channels = pixel::channels(info.pixel); 
  const int channel_bytes = pixel::channel_bytes(info.pixel); 
  const int bytes = pixel::bytes(info.pixel);  
  
  DJV_DEBUG_PRINT("channels = " << channels);  
  DJV_DEBUG_PRINT("channel_bytes = " << channel_bytes);  
  DJV_DEBUG_PRINT("bytes = " << bytes);  
  
  // Read.
  
  _io.read_ahead();
  
  Data * data = frame.proxy ? &_tmp : &_image;
  
  data->set(info);
  
  tiles_rgba = _tiles;
  
  // Read FOR4 <size> TBMP block
  for (;;)
  {
    // Get type.
    _io.get(&type, 4);
    
    // Get length.
    _io.get_u32(&size, 1);
    chunk_size = align_size(size, 4);
    
    if (type[0] == 'A' &&
        type[1] == 'U' &&
        type[2] == 'T' &&
        type[3] == 'H')   
    {  
      DJV_DEBUG_PRINT("type = AUTH");
      const uint8_t * p = _io.mmap_p();
      
      // Get tag.
      info.tag[tag::label_tag()[tag::CREATOR]] =
        String((const char *)p, chunk_size);

      // Skip to the next block.
      _io.seek(chunk_size);  
      
      // Set tag.
      _image.tag = info.tag;
    }
    else if (type[0] == 'F' &&
             type[1] == 'O' &&
             type[2] == 'R' &&
             type[3] == '4')
    {
      DJV_DEBUG_PRINT("type = FOR4");

      // Get type.
      _io.get(&type, 4);
      
      // Check if TBMP.
      if (type[0] == 'T' &&
          type[1] == 'B' &&
          type[2] == 'M' &&
          type[3] == 'P')
      {
        DJV_DEBUG_PRINT("type = TBMP");
        
        // Read RGBA and ZBUF block.
        for (;;)
        {
          // Get type.
          _io.get(&type, 4);

          // Get length.
          _io.get_u32(&size, 1);
          chunk_size = align_size(size, 4);

          // Tiles and RGBA.
          if (tiles_rgba && 
              type[0] == 'R' &&
              type[1] == 'G' &&
              type[2] == 'B' &&
              type[3] == 'A')
          {
            // Set image size.
            uint32_t image_size = size;

            // Get tile coordinates.
            uint16_t xmin, xmax, ymin, ymax;
            _io.get_u16 (&xmin, 1);
            _io.get_u16 (&ymin, 1);
            _io.get_u16 (&xmax, 1);
            _io.get_u16 (&ymax, 1);

            if (xmin > xmax || 
                ymin > ymax || 
                xmax >= info.size.x || 
                ymax >= info.size.y)
            {
              io::throw_error_unsupported(name(), file_name);
            }

            // NOTE: tile w = xmax - xmin + 1 
            //       tile h = ymax - ymin + 1
           
            uint32_t tw = xmax - xmin + 1;
            uint32_t th = ymax - ymin + 1;

            if (!tw || !th)
            {
              io::throw_error_unsupported(name(), file_name);
            }

            bool tile_compress = false;
         
            // If tile compression fails to be less than 
            // image data stored uncompressed, the tile
            // is written uncompressed.
         
            // Set channels.
            uint8_t channels = pixel::channels(info.pixel);
            
            // Set tile pixels.
            
            // Append xmin, xmax, ymin and ymax.
            uint32_t tile_size = tw * th * channels * pixel::channel_bytes(info.pixel) + 8;
                                  
            // Test compressed.
            if (tile_size > image_size)
            {
              tile_compress = true;
            } 
            
            // Handle 8-bit data.
            if (info.pixel == pixel::RGB_U8 || info.pixel == pixel::RGBA_U8)
            {
            
              // Tile compress.
              
              if (tile_compress)
              {
                // Set bytes.
                const uint8_t * p = _io.mmap_p();
                
                // Map: RGB(A)8 BGRA to RGBA
                for (int c =(channels * channel_bytes) - 1; c>=0; --c)
                {
                  memory::Buffer<uint8_t> in(tw * th);
                  uint8_t * in_p = in ();
                  
                  // Uncompress.

                  p += rle_load (p, in(), tw * th);
                  
                  for (uint16_t py=ymin; py<=ymax; py++)
                  {
                    uint8_t * out_dy = data->data(0, py);
                    for (uint16_t px=xmin; px<=xmax; px++)
                    {
                      uint8_t * out_p = out_dy + px * bytes + c;
                      *out_p++ = *in_p++;
                    }
                  }                  
                }
                
                // Seek
                
                _io.seek(image_size - 8);
                
                // Test.
                
                if (p != _io.mmap_p())
                {
                  io::throw_error_unsupported(name(), file_name);
                }
              }
              else
              {
                for (uint16_t py=ymin; py<=ymax; py++)
                {
                  uint8_t * out_dy = data->data(xmin, py);
                 
                  // Tile scanline. 
                 
                  memory::Buffer<uint8_t> scanline(tw * bytes);
                  uint8_t * out_p = scanline();
                  
                  // Set bytes.
                  
                  for (uint16_t px=xmin; px<=xmax; px++)
                  {
                    // Get pixels.
                    _io.get(&pixels, bytes);
                    
                    if (size < bytes)
                      io::throw_error_read(name(), file_name);
                    size -= bytes;
                    
                    // Map: RGB(A)8 ABGR to ARGB
                    for (int c=channels - 1; c>=0; --c)
                    {
                      uint8_t pixel;
                      uint8_t * in_p = pixels + c * channel_bytes;
                      memory::copy(in_p, &pixel, 1);
                      *out_p++ = pixel;
                    }
                  }
                  // Copy data.
                  memory::copy(scanline(), out_dy, tw * bytes);
                }
              }
            }
            // Handle 16-bit data.
            else if (info.pixel == pixel::RGB_U16 || info.pixel == pixel::RGBA_U16)
            {
              if (tile_compress)
              {
              
                // Set bytes.
                const uint8_t * p = _io.mmap_p();
                
                // Set map.
                int* map = NULL;
                if (memory::endian () == memory::LSB)
                {
                  int rgb16[] = { 0, 2, 4, 1, 3, 5 };
                  int rgba16[] = { 0, 2, 4, 7, 1, 3, 5, 6 };
                  if (info.pixel == pixel::RGB_U16)
                    map = rgb16;
                  else
                    map = rgba16;
                }
                else
                {
                  int rgb16[] = { 1, 3, 5, 0, 2, 4 };
                  int rgba16[] = { 1, 3, 5, 7, 0, 2, 4, 6 };
                  if (info.pixel == pixel::RGB_U16)
                    map = rgb16;
                  else
                    map = rgba16;
                }
                
                // Map: RGB(A)8 BGRA to RGBA
                for (int c =(channels * channel_bytes) - 1; c>=0; --c)
                {
                  int mc = map[c];
                
                  memory::Buffer<uint8_t> in(tw * th);
                  uint8_t * in_p = in ();
                  
                  // Uncompress.

                  p += rle_load (p, in(), tw * th);
                  
                  for (uint16_t py=ymin; py<=ymax; py++)
                  {
                    uint8_t * out_dy = data->data(0, py);
                    for (uint16_t px=xmin; px<=xmax; px++)
                    {
                      uint8_t * out_p = out_dy + px * bytes + mc;
                      *out_p++ = *in_p++;
                    }
                  }                  
                }
                
                // Seek
                
                _io.seek(image_size - 8);
                
                // Test.
                
                if (p != _io.mmap_p())
                {
                  io::throw_error_unsupported(name(), file_name);
                }
              }
              else
              {   
                for (uint16_t py=ymin; py<=ymax; py++)
                {
                  uint8_t * out_dy = data->data(xmin, py);
                  
                  // Tile scanline.
                  
                  memory::Buffer<uint16_t> scanline(tw * bytes);
                  uint16_t * out_p = scanline();
                  
                  // Set bytes.
                  
                  for (uint16_t px=xmin; px<=xmax; px++)
                  {
                    // Get pixels.
                    _io.get(&pixels, bytes);
                    
                    if (size   < bytes)
                        io::throw_error_read(name(), file_name);
                      size -= bytes;
                      
                      // Map: RGB8 ABGR to ARGB
                      for (int c=channels - 1; c>=0; --c)
                      {
                        uint16_t pixel;
                        uint8_t * in = pixels + c * channel_bytes;
                        if (memory::endian () == memory::LSB)
                          memory::endian(in, &pixel, 1, 2);
                        else
                          memory::copy(in, &pixel, 2);
                        // Set pixel.
                        *out_p++ = pixel;
                    }
                  }
                  // Copy data.
                  memory::copy(scanline(), out_dy, tw * bytes);
                }
              }
            }
            else
            {
              _io.seek(chunk_size);
            }

            // Seek to align to chunksize.
            size = chunk_size - image_size;
            if (size)
            {
              _io.seek(size);
            }
            tiles_rgba--;
          }
          else
          {
            _io.seek(chunk_size);  
          }
          
          if (tiles_rgba)
            continue; 
          else
            break; 

        }
           
        // TBMP done, break.
        break;
      }
     
    }
    else
    {
      // Skip to the next block.
      _io.seek(chunk_size);  
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

