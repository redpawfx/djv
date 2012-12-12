/*------------------------------------------------------------------------------
 lib/djv_ui/vector.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_VECTOR_H
#define DJV_UI_VECTOR_H

#include <djv_ui/vector_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::vector
//
//! Vector widgets.
//------------------------------------------------------------------------------

namespace vector {

//------------------------------------------------------------------------------
//!\class V2i_Edit
//
//! Two-dimensional integer vector widget.
//------------------------------------------------------------------------------

class _DJV_UI V2i_Edit : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  V2i_Edit();
  
  ~V2i_Edit();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(const V2i &);
  
  const V2i & get() const;
  
  callback::Signal<const V2i &> signal;
  
  //@}
  
private:

  DJV_CALLBACK(V2i_Edit, widget_callback, int);
  DJV_CALLBACK(V2i_Edit, widget2_callback, int);
  
  void widget_update();

  V2i _value, _tmp;

  struct _V2i_Edit;
  std::auto_ptr<_V2i_Edit> _p;
};

//------------------------------------------------------------------------------
//!\class V2f_Edit
//
//! Two-dimensional floating-point vector widget.
//------------------------------------------------------------------------------

class _DJV_UI V2f_Edit : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  V2f_Edit();
  
  ~V2f_Edit();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(const V2f &);
  
  const V2f & get() const;
  
  callback::Signal<const V2f &> signal;
  
  //@}

private:

  DJV_CALLBACK(V2f_Edit, widget_callback, double);
  DJV_CALLBACK(V2f_Edit, widget2_callback, double);
  
  void widget_update();

  V2f _value, _tmp;

  struct _V2f_Edit;
  std::auto_ptr<_V2f_Edit> _p;
};

//------------------------------------------------------------------------------

}
}

#endif

