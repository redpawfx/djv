/*------------------------------------------------------------------------------
 plugin/djv_vlut/load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "load.h"

#include <FL/x.h>

namespace djv_plugin {
namespace vlut {

//------------------------------------------------------------------------------
// djv_image_load()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_load()
{
  return new Load;
}

}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
  _size(0)
{}

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return vlut::name;
}

List<String> Load::extension_list() const
{
  return vlut::extension_list;
}

bool Load::is_seq() const
{
  return false;
}

//------------------------------------------------------------------------------
// Load::open()
//------------------------------------------------------------------------------

void Load::open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::open");
  
  // Open.
  
#if defined(DJV_WINDOWS)

  _handle.open();

#endif // DJV_WINDOWS
  
  // Information.

  _size = 0;

#if defined(DJV_WINDOWS)

  _size = 256;

#elif defined(DJV_IRIX)

  int maps = 0, precision = 0, attr = 0;
  if (! XSGIvcQueryScreenGammaMaps(fl_display, fl_screen, &maps))
    djv_image::io::throw_error_open(name(), in);
  if (maps <= 0)
    djv_image::io::throw_error_read(name(), in);

  if (
    ! XSGIvcQueryGammaMap(fl_display, fl_screen, 0, &size, &precision, &attr)
  )
    djv_image::io::throw_error_open(name(), in);
  if (attr & XSGIVC_GMReadLock)
    djv_image::io::throw_error_open(name(), in);

#else // DJV_WINDOWS

  if (! XF86VidModeGetGammaRampSize(fl_display, fl_screen, &_size))
    djv_image::io::throw_error_open(name(), in);
  
#endif // DJV_WINDOWS

  info.file_name = in;
  info.size = V2i(_size, 1);
  info.pixel = pixel::RGB_U16;

  _image.set(info);
  _tmp.set(info);
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

const Image * Load::get(const io::Frame_Info &) throw (Error)
{
  DJV_DEBUG("Load::get");
  
  // Read.

  const int size = _size;
  pixel::U16_T * p = reinterpret_cast<pixel::U16_T *>(_tmp.data());

#if defined(DJV_WINDOWS)
  
  ::GetDeviceGammaRamp(_handle(), p);

#elif defined(DJV_IRIX)

  uint16_t * tmp [] = { 0, 0, 0 };
  int _size = 0;
  
  XSGIvcQueryGammaColors(
    fl_display, fl_screen, 0, XSGIVC_COMPONENT_RED, &_size, tmp);
  XSGIvcQueryGammaColors(
    fl_display, fl_screen, 0, XSGIVC_COMPONENT_GREEN, &_size, tmp + 1);
  XSGIvcQueryGammaColors(
    fl_display, fl_screen, 0, XSGIVC_COMPONENT_BLUE, &_size, tmp + 2);
  
  memory::copy(tmp[0], p, size * 2);
  memory::copy(tmp[1], p + size, size * 2);
  memory::copy(tmp[2], p + size * 2, size * 2);
  
  if (tmp[0]) XFree(tmp[0]);
  if (tmp[1]) XFree(tmp[1]);
  if (tmp[2]) XFree(tmp[2]);

#else // DJV_WINDOWS

  XF86VidModeGetGammaRamp(
    fl_display, fl_screen, size, p, p + size, p + size * 2
  );
  
#endif // DJV_WINDOWS
  
#if defined(DJV_DEBUG)
  //for (int i = 0; i < size; ++i)
  //{
  //  DJV_DEBUG_PRINT(i << " = " << p[i]);
  //}
#endif
  
  data::planar_interleave(_tmp, &_image);
  
  DJV_DEBUG_PRINT("image = " << _image);
  
  return &_image;
}

//------------------------------------------------------------------------------

}
}

