//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_callback.h

#ifndef DJV_CALLBACK_H
#define DJV_CALLBACK_H

#include <djv_gui_export.h>

#include <djv_list.h>

class Fl_Widget;

namespace djv
{

class Abstract_Signal;

//------------------------------------------------------------------------------
//! \class Callback
//!
//! This class provides the base functionality for callbacks.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Callback
{
public:

    //! Constructor.

    Callback();

    //! Destructor.

    virtual ~Callback() = 0;

    //! Destroy the callback.

    void del();

    //! Add a signal to the callback.

    void signal_add(Abstract_Signal *);

    //! Remove a signasl from the callback.

    void signal_del(Abstract_Signal *);

    //! Get the number of signals in the callback.

    size_t signal_size() const;

    //! Add a connection to the callback.

    void connect_add(Abstract_Signal *);

    //! Remove a connection from the callback.

    void connect_del(Abstract_Signal *);

    //! Get the number of connections in the callback.

    size_t connect_size() const;

    //! Set whether callbacks are enabled.

    void callbacks(bool);

    //! Get whether callbacks are enabled.

    bool callbacks() const;

private:

    List<Abstract_Signal *> _signals;
    List<Abstract_Signal *> _connections;
    bool _callbacks;
};

#define DJV_CALLBACK(TYPE, NAME, ARG) \
  \
  static void NAME(Callback * that, ARG data) \
  { \
    if (that->callbacks()) \
      static_cast<TYPE *>(that)->NAME(data); \
  } \
  \
  void NAME(ARG in)

//------------------------------------------------------------------------------
//! \class Abstract_Signal
//!
//! This class provides the base functionality for signals.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Abstract_Signal
{
public:

    //! Destructor.

    virtual ~Abstract_Signal() = 0;

    //! Release the callback.

    virtual void release(Callback *) = 0;

    //! Destroy the signal.

    virtual void del() = 0;
};

//------------------------------------------------------------------------------
//! \class Signal
//!
//! This class provides a signal that triggers a callback.
//------------------------------------------------------------------------------

template <class T>
class Signal : public Abstract_Signal
{
public:

    //! Constructor.

    inline Signal(Callback * parent);

    //! Destructor.

    inline virtual ~Signal();

    //! Callback function.

    typedef void (Callback_Fnc)(Callback *, T);

    //! Set a callback.

    inline void set(Callback *, Callback_Fnc *);

    //! Remove a callback.

    inline void del(Callback *);

    //! Remove all callbacks.

    inline void del();

    //! Emit a signal.

    inline void emit(T);

    //! Release a callback.

    inline void release(Callback *);

    //! Get the number of callbacks connected to the signal.

    inline size_t size() const;

private:

    typedef std::pair<Callback *, void *> Pair;

    List<Pair> _callbacks;
    Callback * _parent;
};

//------------------------------------------------------------------------------

//! FLTK callback macro.

#define DJV_FL_CALLBACK(TYPE, NAME) \
  \
  static void NAME(void * that) \
  { \
    if (static_cast<TYPE *>(that)->callbacks()) \
      static_cast<TYPE *>(that)->NAME(); \
  } \
  \
  void NAME()

//! FLTK widget callback macro.

#define DJV_FL_WIDGET_CALLBACK(TYPE, NAME) \
  \
  static void NAME(Fl_Widget *, void * that) \
  { \
    if (static_cast<TYPE *>(that)->callbacks()) \
      static_cast<TYPE *>(that)->NAME(); \
  } \
  \
  void NAME()

} // djv

#include <djv_callback_inline.h>

#endif // DJV_CALLBACK_H

