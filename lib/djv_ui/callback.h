/*------------------------------------------------------------------------------
 lib/djv_ui/callback.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_CALLBACK_H
#define DJV_UI_CALLBACK_H

#include <djv_ui/callback_def.h>

#include <djv_ui/base.h>

class Fl_Widget;

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::callback
//
//! Callbacks.
//------------------------------------------------------------------------------

namespace callback {

//------------------------------------------------------------------------------
//!\class Callback_Base
//
//! A base class for objects that use signals.
//------------------------------------------------------------------------------

class _DJV_UI Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Callback_Base();
  
  virtual ~Callback_Base() = 0;
  
  void del();

  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  void signal_add(Signal_Base *);
  void signal_del(Signal_Base *);
  
  list::size_type signal_size() const;
  
  void connect_add(Signal_Base *);
  void connect_del(Signal_Base *);
  
  list::size_type connect_size() const;
  
  void callbacks(bool);
  
  bool callbacks() const;
  
  //@}
  
private:

  List<Signal_Base *> _signal_list;
  List<Signal_Base *> _connect_list;
  bool _callbacks;
};

#define DJV_CALLBACK(TYPE, NAME, ARG) \
  \
  static void NAME(Callback_Base * that, ARG data) \
  { \
    if (that->callbacks()) \
      static_cast<TYPE *>(that)->NAME(data); \
  } \
  \
  void NAME(ARG in)

//------------------------------------------------------------------------------
//!\class Signal_Base
//
//! A base class for signals.
//------------------------------------------------------------------------------

class _DJV_UI Signal_Base
{
public:

  virtual ~Signal_Base();
  
  virtual void release(Callback_Base *) = 0;
  virtual void del() = 0;
};

//------------------------------------------------------------------------------
//!\class Signal
//
//! A signal that triggers a callback.
//------------------------------------------------------------------------------

template <class T>
class Signal : public Signal_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  inline Signal(Callback_Base * parent);
  
  inline ~Signal();

  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{

  typedef void (Callback)(Callback_Base *, T);
  
  inline void set(Callback_Base *, Callback *);
  
  inline void del(Callback_Base *);
  inline void del();
  
  inline void emit(T);
  
  inline void release(Callback_Base *);

  inline list::size_type size() const;
  
  //@}

private:

  typedef std::pair<Callback_Base *, void *> Pair;
  
  List<Pair> _callback_list;
  
  Callback_Base * _parent;
};

//------------------------------------------------------------------------------
// FLTK Callbacks
//------------------------------------------------------------------------------

#define DJV_FL_CALLBACK(TYPE, NAME) \
  \
  static void NAME(void * that) \
  { \
    if (static_cast<TYPE *>(that)->callbacks()) \
      static_cast<TYPE *>(that)->NAME(); \
  } \
  \
  void NAME()

#define DJV_FL_WIDGET_CALLBACK(TYPE, NAME) \
  \
  static void NAME(Fl_Widget *, void * that) \
  { \
    if (static_cast<TYPE *>(that)->callbacks()) \
      static_cast<TYPE *>(that)->NAME(); \
  } \
  \
  void NAME()

//------------------------------------------------------------------------------

}
}

#include <djv_ui/callback_inline.h>
#endif

