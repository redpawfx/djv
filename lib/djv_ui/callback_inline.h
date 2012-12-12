/*------------------------------------------------------------------------------
 lib/djv_ui/callback_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_ui {
namespace callback {

//------------------------------------------------------------------------------
// Signal
//------------------------------------------------------------------------------

template <class T>
inline Signal<T>::Signal(Callback_Base * parent) :
  _parent(parent)
{
  _parent->signal_add(this);
}

template <class T>
inline Signal<T>::~Signal()
{
  del();

  _parent->signal_del(this);
}

template <class T>
inline void Signal<T>::set(Callback_Base * in, Callback * callback)
{
  in->connect_add(this);
  
  _callback_list.push_back(Pair(in, (void *)callback));
}

template <class T>
inline void Signal<T>::del(Callback_Base * in)
{
  List<Pair>::iterator poo;
  
  for (
    List<Pair>::iterator i = _callback_list.begin();
    i != _callback_list.end();
    ++i
  )
    if (in == i->first)
    {
      in->connect_del(this);
      
      _callback_list.erase(i);
      
      break;
    }
}

template <class T>
inline void Signal<T>::del()
{
  for (
    List<Pair>::iterator i = _callback_list.begin();
    i != _callback_list.end();
    ++i
  )
    i->first->connect_del(this);
  
  _callback_list.clear();
}

template <class T>
inline void Signal<T>::emit(T in)
{
  // XXX
  
  /*for (
    List<Pair>::iterator i = _callback_list.begin();
    i != _callback_list.end();
    ++i
  )
    ((Callback *)i->second)(i->first, in);*/
  
  for (list::size_type i = 0; i < _callback_list.size(); ++i)
    ((Callback *)_callback_list[i].second)(_callback_list[i].first, in);
}

template <class T>
inline void Signal<T>::release(Callback_Base * in)
{
  for (
    List<Pair>::iterator i = _callback_list.begin();
    i != _callback_list.end();
    ++i
  )
    if (in == i->first)
    {
      _callback_list.erase(i);
      
      break;
    }
}

template <class T>
inline list::size_type Signal<T>::size() const
{
  return _callback_list.size();
}

//------------------------------------------------------------------------------

}
}

