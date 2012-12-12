/*------------------------------------------------------------------------------
 lib/djv_ui/label.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_LABEL_H
#define DJV_UI_LABEL_H

#include <djv_ui/label_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::label
//
//! Label widgets.
//------------------------------------------------------------------------------

namespace label {

//------------------------------------------------------------------------------
//!\class Label
//
//! A label widget.
//------------------------------------------------------------------------------

class _DJV_UI Label : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Label(const String & = String());
  
  virtual ~Label();

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

  virtual int handle(int);
  virtual void draw();
  
  callback::Signal<bool> push_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void size_string(const String &);
  
  virtual const String & size_string() const;

  virtual void dirty();
  
  virtual Layout_Item * group_align();
  virtual Layout_Item * label_align();

  //@}
  
private:

  String _value;
  List<String> _lines;
  int _lines_max_width;
  String _size_string;
};

//------------------------------------------------------------------------------
//!\class Multiline_Label
//
//! A multi-line label widget.
//------------------------------------------------------------------------------

class _DJV_UI Multiline_Label : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Multiline_Label(const String & = String());
  
  virtual ~Multiline_Label();

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

  virtual int handle(int);
  virtual void draw();
  
  callback::Signal<bool> push_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;
  virtual const V2i & size_min() const;
  virtual Layout_Item * group_align();
  virtual Layout_Item * label_align();

  //@}
  
private:

  void text_measure();
  
  V2i text_hint(const V2i &) const;

  String _value;
  List<String> _word_list;
  List<int> _word_size_list;
  int _word_size_max;
  int _space_size;
  int _text_height;
  V2i _size_min;
  V2i _text_hint;
  int _font_size;
};

//------------------------------------------------------------------------------
//!\class Image_Label
//
//! An image label widget.
//------------------------------------------------------------------------------

class _DJV_UI Image_Label : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Image_Label(Fl_Image * = 0);
  
  virtual ~Image_Label();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{

  void set(Fl_Image *);
  
  Fl_Image * get() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{

  virtual int handle(int);
  virtual void draw();
  
  callback::Signal<bool> push_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{

  virtual void dirty();
  
  virtual Layout_Item * group_align();
  virtual Layout_Item * label_align();

  //@}
  
private:

  Fl_Image * _value;
};

//------------------------------------------------------------------------------

}
}

#endif

