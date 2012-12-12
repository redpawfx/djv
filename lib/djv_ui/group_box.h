/*------------------------------------------------------------------------------
 lib/djv_ui/group_box.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_GROUP_BOX_H
#define DJV_UI_GROUP_BOX_H

#include <djv_ui/group_box_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::group_box
//
//! Group box widgets.
//------------------------------------------------------------------------------

namespace group_box {

//------------------------------------------------------------------------------
//!\class Box
//
//! A group box widget.
//------------------------------------------------------------------------------

class _DJV_UI Box : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Box(const String & label = String());
  
  virtual ~Box();
  
  virtual void del();

  //@}
  //----------------------------------------------------------------------------
  //!\name Label
  //@{

  void label(const String &);
  
  const String & label() const;

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
  
  String _label;
  V2i _size_label;
};

//------------------------------------------------------------------------------
//!\class Box_Small
//
//! A small group box widget.
//------------------------------------------------------------------------------

class _DJV_UI Box_Small : public Box
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Box_Small(const String & label = String());
  
  //@}
};

//------------------------------------------------------------------------------
//!\class Box_Large
//
//! A large group box widget.
//------------------------------------------------------------------------------

class _DJV_UI Box_Large : public Box
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Box_Large(const String & label = String());
  
  //@}
};

//------------------------------------------------------------------------------

}
}

#endif

