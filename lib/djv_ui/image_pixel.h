/*------------------------------------------------------------------------------
 lib/djv_ui/image_pixel.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_IMAGE_PIXEL_H
#define DJV_UI_IMAGE_PIXEL_H

#include <djv_ui/image_pixel_def.h>

#include <djv_image/pixel.h>
#include <djv_ui/widget.h>

namespace djv_ui {
namespace image {

//------------------------------------------------------------------------------
//!\class Pixel
//
//! An image pixel widget.
//------------------------------------------------------------------------------

class _DJV_UI Pixel : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Pixel();
  
  virtual ~Pixel();
  
  virtual void del();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(djv_image::PIXEL);
  
  djv_image::PIXEL get() const;
  
  callback::Signal<djv_image::PIXEL> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void draw();

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;

  //@}
  
private:

  DJV_CALLBACK(Pixel, widget_callback, int);
  
  void widget_update();

  djv_image::PIXEL _value;
  
  struct _Pixel;
  std::auto_ptr<_Pixel> _p;
};

//------------------------------------------------------------------------------

}
}

#endif
