/*------------------------------------------------------------------------------
 plugin/djv_vlut/save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "save.h"

#include <djv_image/gl_data.h>
#include <FL/x.h>

namespace djv_plugin {
namespace vlut {

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

plugin::Plugin * Save::copy() const
{
  return new Save;
}

String Save::name() const
{
  return vlut::name;
}

List<String> Save::extension_list() const
{
  return vlut::extension_list;
}

bool Save::is_seq() const
{
  return false;
}

//------------------------------------------------------------------------------
// Save::open()
//------------------------------------------------------------------------------

void Save::open(const File & in, const io::Info &) throw (Error)
{
  DJV_DEBUG("Save::open");
  
  // Open.
  
#if defined(DJV_WINDOWS)

  _handle.open();

#endif // DJV_WINDOWS

  // Information.
  
  int size = 0;
  
#if defined(DJV_WINDOWS)

  size = 256;

#elif defined(DJV_IRIX)

  int maps = 0, precision = 0, attr = 0;

  if (! XSGIvcQueryScreenGammaMaps(fl_display, fl_screen, &maps))
    djv_image::io::throw_error_open(name(), in);
  if (maps <= 0)
    djv_image::io::throw_error_open(name(), in);

  if (! XSGIvcQueryGammaMap(fl_display, fl_screen, 0,
    &size, &precision, &attr)
  )
    djv_image::io::throw_error_open(name(), in);
  if (attr & XSGIVC_GMWriteLock)
    djv_image::io::throw_error_open(name(), in);

#else // DJV_WINDOWS

  if (! XF86VidModeGetGammaRampSize(fl_display, fl_screen, &size))
    djv_image::io::throw_error_open(name(), in);

#endif // DJV_WINDOWS

  DJV_DEBUG_PRINT("vlut size = " << size);
  
  _info = Info();
  _info.file_name = in;
  _info.size = V2i(size, 1);
  _info.pixel = pixel::RGB_U16;
  
  DJV_DEBUG_PRINT("info = " << _info);
  
  _image.set(_info);
  _tmp.set(_info);
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

void Save::set(const Image & in, const io::Frame_Info &) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);

  // Write.
  
  const Data * p = &in;
  
  if (in.info() != _info)
  {
    DJV_DEBUG_PRINT("convert = " << _image);
    
    _image.zero();
    
    gl_data::Option option;
    option.xform.scale = V2f(in.size()) / V2f(_image.size());
    gl_data::copy(in, &_image, option);
    
    p = &_image;
  }
  
  data::planar_deinterleave(*p, &_tmp);
  pixel::U16_T * data_p = reinterpret_cast<pixel::U16_T *>(_tmp.data());
  
#if defined(DJV_DEBUG)
  //for (int i = 0; i < _info.size.x; ++i)
  //{
  //  DJV_DEBUG_PRINT(i << " = " << data_p[i]);
  //{
#endif
  
#if defined(DJV_WINDOWS)

  ::SetDeviceGammaRamp(_handle(), data_p);

#elif defined(DJV_IRIX)

  // XXX
  
  XSGIvcStoreGammaColors16(
    fl_display,
    fl_screen,
    0,
    _info.size.x,
    XSGIVC_MComponentRed | XSGIVC_MComponentGreen | XSGIVC_MComponentBlue,
    data_p
  );

#else // DJV_WINDOWS

  if (
    ! XF86VidModeSetGammaRamp(
      fl_display,
      fl_screen,
      _info.size.x,
      data_p,
      data_p + _info.size.x,
      data_p + _info.size.x * 2
    )
  )
    djv_image::io::throw_error_write(name(), _info.file_name);
  
#endif // DJV_WINDOWS
}

//------------------------------------------------------------------------------

}
}

