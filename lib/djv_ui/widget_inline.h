/*------------------------------------------------------------------------------
 lib/djv_ui/widget_inline.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_ui {
namespace widget {

//------------------------------------------------------------------------------
// Widget_Base
//------------------------------------------------------------------------------
  
inline Fl_Font Widget_Base::text_font() const
{
  return _text_font;
}

inline Fl_Color Widget_Base::text_color() const
{
  return _text_color;
}

inline const String & Widget_Base::tooltip() const
{
  return _tooltip;
}
  
inline bool Widget_Base::inside() const
{
  return _inside;
}
  
inline const Box2i & Widget_Base::geom() const
{
  return Layout_Item::geom();
}

inline layout::Layout * Widget_Base::layout() const
{
  return _layout.get();
}

//------------------------------------------------------------------------------
// Auto_Ptr
//------------------------------------------------------------------------------

template<class T>
inline Auto_Ptr<T>::Auto_Ptr() :
  _p(0)
{}

template<class T>
inline Auto_Ptr<T>::Auto_Ptr(T * in) :
  _p(in)
{}

template<class T>
inline Auto_Ptr<T>::~Auto_Ptr()
{
  del();
}

template<class T>
inline void Auto_Ptr<T>::set(T * in)
{
  _p = in;
}

template<class T>
inline void Auto_Ptr<T>::del()
{
  if (_p)
  {
    _p->del();
    _p = 0;
  }
}

template<class T>
inline T * Auto_Ptr<T>::get() const
{
  return _p;
}

template<class T>
inline T * Auto_Ptr<T>::operator -> () const
{
  return _p;
}

//------------------------------------------------------------------------------
// Widget
//------------------------------------------------------------------------------
  
inline const String & Widget::tooltip() const
{
  return Widget_Base::tooltip();
}

inline void Widget::resize(int x, int y, int w, int h)
{
  geom(Box2i(x, y, w, h));
}
  
inline const Box2i & Widget::geom() const
{
  return Widget_Base::geom();
}

inline int Widget::visible() const
{
  return _visible;
}
  
inline const layout_item::SIZE * Widget::size() const
{
  return Widget_Base::size();
}

inline const layout_item::ALIGN * Widget::align() const
{
  return Widget_Base::align();
}

//------------------------------------------------------------------------------

}
}
