/*------------------------------------------------------------------------------
 lib/djv_ui/text.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_TEXT_H
#define DJV_UI_TEXT_H

#include <djv_ui/text_def.h>

#include <djv_ui/frame.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::text
//
//! Text widgets.
//------------------------------------------------------------------------------

namespace text {

//------------------------------------------------------------------------------
//!\class Text_Base
//
//! A base class for text widgets.
//------------------------------------------------------------------------------

class _DJV_UI Text_Base : public Frame
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Text_Base();
  
  virtual ~Text_Base();

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual int handle(int);
  virtual void draw();
  virtual void draw_select(bool);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  
  //@}
  
protected:

  bool _draw_select;
};

//------------------------------------------------------------------------------
//!\class Text_Edit
//
//! A widget for editing text.
//------------------------------------------------------------------------------

class _DJV_UI Text_Edit : public Text_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Text_Edit(const String & = String());
  
  virtual ~Text_Edit();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void set(const String &);
  
  const String & get() const;
  
  callback::Signal<const String &> signal;
  callback::Signal<const String &> action_signal;
  callback::Signal<const String &> change_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{

  virtual void color(Fl_Color);
  
  virtual Fl_Color color() const;

  virtual void text_font(Fl_Font);
  virtual void text_color(Fl_Color);
  
  virtual Fl_Font text_font() const;
  virtual Fl_Color text_color() const;
  
  virtual int handle(int);

  virtual const Fl_Widget * fltk_widget() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void size_string(const String &);
  
  virtual const String & size_string() const;
  
  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;

  //@}
  
private:

  DJV_FL_WIDGET_CALLBACK(Text_Edit, widget_callback);

  void widget_update();

  String _value;
  
  String _size_string;
  
  struct _Text_Edit;
  std::auto_ptr<_Text_Edit> _p;
};

//------------------------------------------------------------------------------
//!\class Text_Display
//
//! A widget for displaying text. Unlike a label, this works with copy and
//! paste.
//------------------------------------------------------------------------------

class _DJV_UI Text_Display : public Text_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Text_Display(const String & = String());
  
  virtual ~Text_Display();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(const String &);
  
  const String & get() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void text_font(Fl_Font);
  virtual void text_color(Fl_Color);
  
  virtual Fl_Font text_font() const;
  virtual Fl_Color text_color() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void size_string(const String &);
  
  virtual const String & size_string() const;

  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;

  //@}
  
private:

  void widget_update();

  String _value;
  String _size_string;
  
  class Widget;
  Widget * _widget;
};

//------------------------------------------------------------------------------
//!\class Text_Multiline_Display
//
//! A widget for displaying multiple lines of text.
//------------------------------------------------------------------------------

class _DJV_UI Text_Multiline_Display : public Text_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Text_Multiline_Display(const String & = String());
  
  virtual ~Text_Multiline_Display();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(const String &);
  
  const String & get() const;

  //! Word wrap.
  
  void word_wrap(bool);
  
  bool word_wrap() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void text_font(Fl_Font);
  virtual void text_color(Fl_Color);
  
  virtual Fl_Font text_font() const;
  virtual Fl_Color text_color() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;

  //@}
  
private:

  void widget_update();

  String _value;
  bool _word_wrap;
  
  class Widget;
  Widget * _widget;
};

//------------------------------------------------------------------------------

}
}

#endif

