/*------------------------------------------------------------------------------
 lib/djv_ui/frame.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_FRAME_H
#define DJV_UI_FRAME_H

#include <djv_ui/frame_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::frame
//
//! Frame widgets.
//------------------------------------------------------------------------------

namespace frame {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Style.

enum STYLE
{
  STYLE_NORMAL,
  STYLE_H1,
  STYLE_H2,
  STYLE_H3,
  STYLE_V1,
  STYLE_V2,
  STYLE_V3,
  STYLE_THIN,
  STYLE_BORDER,
  STYLE_FLAT,
  STYLE_EMPTY
};

//@}
//------------------------------------------------------------------------------
//!\class Frame
//
//! A frame widget.
//------------------------------------------------------------------------------

class _DJV_UI Frame : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Frame(STYLE = STYLE_NORMAL, bool down = false);
  
  virtual ~Frame();

  //@}
  //----------------------------------------------------------------------------
  //!\name Frame
  //@{
  
  void style(STYLE);
  void down(bool);
  void margin(int);
  
  STYLE style() const;
  bool down() const;
  int margin() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
    
  virtual int handle(int);
  virtual void draw();
  
  callback::Signal<const Frame *> select_signal;
  callback::Signal<const Frame *> select_off_signal;
  callback::Signal<const Frame *> highlight_signal;
  callback::Signal<const Frame *> highlight_off_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual int frame_size() const;
  virtual Box2i frame_geom() const;
  
  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;

  //@}

protected:

  static Fl_Boxtype box_type(STYLE, bool down);
  
private:

  STYLE _style;
  bool _down;
  int _margin;
};

//------------------------------------------------------------------------------
//!\class Frame_Group_Base
//
//! A base class for frame groups.
//------------------------------------------------------------------------------

class _DJV_UI Frame_Group_Base : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Frame_Group_Base(
    bool horizontal,
    const List<Frame *> & = List<Frame *>()
  );
  
  virtual ~Frame_Group_Base() = 0;

  //@}
  //----------------------------------------------------------------------------
  //!\name Group
  //@{
  
  void group(const List<Frame *> &);
  
  const List<Frame *> & group() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{

  virtual void draw();
  
  //@}

private:

  DJV_CALLBACK(Frame_Group_Base, draw_select_callback, const Frame *);
  DJV_CALLBACK(Frame_Group_Base, draw_select_off_callback, const Frame *);
  DJV_CALLBACK(Frame_Group_Base, draw_highlight_callback, const Frame *);
  DJV_CALLBACK(Frame_Group_Base, draw_highlight_off_callback, const Frame *);

  void widget_update();

  bool _horizontal;
  
  List<Frame *> _group;
  
  const Frame * _draw_select;
  const Frame * _draw_highlight;
  
  struct _Frame_Group_Base;
  std::auto_ptr<_Frame_Group_Base> _p;
};

//------------------------------------------------------------------------------
//!\class Frame_Group_Horizontal
//
//! A horizontal frame group.
//------------------------------------------------------------------------------

class _DJV_UI Frame_Group_Horizontal : public Frame_Group_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Frame_Group_Horizontal(const List<Frame *> & = List<Frame *>());

  //@}
};

//------------------------------------------------------------------------------
//!\class Frame_Group_Vertical
//
//! A vertical frame group.
//------------------------------------------------------------------------------

class _DJV_UI Frame_Group_Vertical : public Frame_Group_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Frame_Group_Vertical(const List<Frame *> & = List<Frame *>());

  //@}
};

//------------------------------------------------------------------------------

}
}

#endif

