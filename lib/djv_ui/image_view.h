/*------------------------------------------------------------------------------
 lib/djv_ui/image_view.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_IMAGE_VIEW_H
#define DJV_UI_IMAGE_VIEW_H

#include <djv_ui/image_view_def.h>

#include <djv_image/gl_data.h>
#include <djv_ui/gl.h>

namespace djv_ui {
namespace image {

//------------------------------------------------------------------------------
//!\class View
//
//! An image view widget.
//------------------------------------------------------------------------------

class _DJV_UI View : public gl::Gl
{
public:
  
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  View();
  
  virtual ~View();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Image Data
  //@{

  virtual void set(const djv_image::data::Data *);
  
  const djv_image::data::Data * get() const;
  const djv_image::gl_data::Data & gl_data() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Options
  //@{
  
  void option(const djv_image::gl_data::Option &);
  
  const djv_image::gl_data::Option & option() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name View
  //@{
  
  virtual void view(const V2i &);
  
  const V2i & view() const;
  
  virtual void zoom(double);
  virtual void zoom(double, const V2i & focus);
  
  double zoom() const;

  virtual void view_zero();
  virtual void view_center();
  virtual void view_fit();

  callback::Signal<bool> signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void draw();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  Box2f bbox() const;

  //@}
  
private:

  const djv_image::data::Data * _data;
  djv_image::gl_data::Option _option;
  V2i _view;
  double _zoom;
  djv_image::gl_data::Data _gl;
};

//------------------------------------------------------------------------------

}
}

#endif

