/*------------------------------------------------------------------------------
 lib/djv_ui/columns.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_COLUMNS_H
#define DJV_UI_COLUMNS_H

#include <djv_ui/columns_def.h>

#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::columns
//
//! Column widgets.
//------------------------------------------------------------------------------

namespace columns {

//------------------------------------------------------------------------------
//!\class Columns
//
//! A widget for displaying columns of information.
//------------------------------------------------------------------------------

class _DJV_UI Columns : public widget::Widget
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Columns();
  
  virtual ~Columns();

  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  void name_list(const List<String> &);
  void size_list(const List<int> &);
  
  List<String> name_list() const;
  List<int> size_list() const;
  
  callback::Signal<const List<int> &> signal;
  callback::Signal<int> action_signal;

  //@}
  //----------------------------------------------------------------------------
  //!\name Sorting
  //@{
  
  void sort(int);
  
  int  sort() const;

  void sort_reverse(bool);

  bool sort_reverse() const;

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

  virtual void dirty();
  virtual void geom(const Box2i &);
  
  virtual const Box2i & geom() const;
  
  //@}
  
private:
  
  struct Column
  {
    String name;
    int column;
    int size_hint;
  };

  static int handle_push(int, const List<Column> &);
  static int handle_drag(int, const List<Column> &);
  
  void columns_update();
  void widget_update();
  
  List<Column> _column_list;
  int _sort;
  bool _sort_reverse;
  int _push, _drag;
};

//------------------------------------------------------------------------------

}
}

#endif

