/*------------------------------------------------------------------------------
 lib/djv_ui/widget.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_WIDGET_H
#define DJV_UI_WIDGET_H

#include <djv_ui/widget_def.h>

#include <djv_ui/callback.h>
#include <djv_ui/layout.h>
#include <FL/Fl_Group.H>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::widget
//
//! Base widgets.
//------------------------------------------------------------------------------

namespace widget {

//------------------------------------------------------------------------------
//!\class Widget_Base
//
//! A base class for widgets.
//------------------------------------------------------------------------------

class _DJV_UI Widget_Base :
  public layout_item::Layout_Item,
  public callback::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Widget_Base();
  
  virtual ~Widget_Base() = 0;
  
  virtual void del();

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{

  virtual void text_font(Fl_Font);
  virtual void text_color(Fl_Color);
  
  inline virtual Fl_Font text_font() const;
  inline virtual Fl_Color text_color() const;

  virtual void tooltip(const String &);
  
  inline virtual const String & tooltip() const;
  
  virtual int handle(int) = 0;
  void arrow_navkeys(bool);
  virtual void show() = 0;
  virtual void draw() = 0;
  
  inline bool inside() const;
  bool arrow_navkeys() const;
  
  virtual const Fl_Widget * fltk_widget() const = 0;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  virtual void layout(layout::Layout *);
  virtual void geom(const Box2i &) = 0;
  virtual void resize(int, int, int, int) = 0;
  
  inline virtual const Box2i & geom() const;
  inline virtual layout::Layout * layout() const;
  virtual int visible() const = 0;

  //@}
  
private:

  Fl_Font _text_font;
  Fl_Color _text_color;
  
  String _tooltip;
  
  bool _inside;
  bool _arrow_navkeys;
  
  std::auto_ptr<layout::Layout> _layout;
  
  bool _del;
};

template<class T>
class Auto_Ptr
{
public:

  inline Auto_Ptr();
  inline Auto_Ptr(T *);
  
  inline ~Auto_Ptr();
  
  inline void set(T *);
  inline void del();
  
  inline T * get() const;
  inline T * operator -> () const;

private:

  T * _p;
};

//------------------------------------------------------------------------------
//!\class Widget
//
//! A widget.
//------------------------------------------------------------------------------

class _DJV_UI Widget : public Fl_Group, public Widget_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Widget();
  
  virtual ~Widget();

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void tooltip(const String &);
  
  inline virtual const String & tooltip() const;
  
  virtual int handle(int);
  
  virtual void show();
  
  virtual void draw();
  
  void highlight(bool);

  bool highlight() const;

  virtual const Fl_Widget * fltk_widget() const { return this; }
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  virtual void geom(const Box2i &);
  inline virtual void resize(int, int, int, int);
  
  inline virtual const Box2i & geom() const;
  inline virtual int visible() const;

  virtual void size(layout_item::SIZE, layout_item::SIZE);
  virtual void align(layout_item::ALIGN, layout_item::ALIGN);
  
  inline const layout_item::SIZE * size() const;
  inline const layout_item::ALIGN * align() const;

  //@}
  
private:
  
  memory::Buffer<char> _tooltip_cstr;
  
  bool _visible;
  
  bool _highlight;
};

//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

void debug_children(const Fl_Group * in, int indent = 0);

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_ui/widget_inline.h>
#endif

