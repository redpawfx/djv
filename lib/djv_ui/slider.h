/*------------------------------------------------------------------------------
 lib/djv_ui/slider.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_SLIDER_H
#define DJV_UI_SLIDER_H

#include <djv_ui/slider_def.h>

#include <djv_ui/frame.h>
#include <djv_ui/value.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::slider
//
//! Slider widgets.
//------------------------------------------------------------------------------

namespace slider {

//------------------------------------------------------------------------------
//!\class Slider_Base
//
//! A base class for slider widgets.
//------------------------------------------------------------------------------

class _DJV_UI Slider_Base : public Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Slider_Base();
  
  virtual ~Slider_Base();

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{

  virtual int handle(int);
  virtual void draw();

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void handle_size(int);
  
  virtual int handle_size() const;
  virtual Box2i slider_geom() const;

  virtual void dirty();
  
  //@}
  
protected:

  virtual void handle_draw(double value, const Box2i &, bool inside);
  
private:
  
  int _handle_size;
};

//------------------------------------------------------------------------------
//!\class Int_Slider
//
//! An integer value slider widget.
//------------------------------------------------------------------------------

class _DJV_UI Int_Slider :
  public Slider_Base,
  public value::Int_Input_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Int_Slider();
  
  virtual ~Int_Slider();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  callback::Signal<int> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{

  virtual int handle(int);
  virtual void draw();

  //@}
  
private:

  void value_callback();
};

//------------------------------------------------------------------------------
//!\class Float_Slider
//
//! A floating-point value slider widget.
//------------------------------------------------------------------------------

class _DJV_UI Float_Slider :
  public Slider_Base,
  public value::Float_Input_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Float_Slider();
  
  virtual ~Float_Slider();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  callback::Signal<double> signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
 
  virtual int handle(int);
  virtual void draw();

  //@}
  
private:

  void value_callback();
};

//------------------------------------------------------------------------------

}
}

#endif

