/*------------------------------------------------------------------------------
 lib/djv_ui/callback.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "callback.h"

#include <algorithm>

namespace djv_ui {
namespace callback {

//------------------------------------------------------------------------------
// Callback_Base
//------------------------------------------------------------------------------

Callback_Base::Callback_Base() :
  _callbacks(true)
{}

Callback_Base::~Callback_Base()
{
  del();
}

void Callback_Base::del()
{
  for (list::size_type i = 0; i < _signal_list.size(); ++i)
    _signal_list[i]->del();
  _signal_list.clear();
  
  for (list::size_type i = 0; i < _connect_list.size(); ++i)
    _connect_list[i]->release(this);
  _connect_list.clear();
}

void Callback_Base::signal_add(Signal_Base * in)
{
  _signal_list += in;
}

void Callback_Base::signal_del(Signal_Base * in)
{
  const List<Signal_Base *>::iterator i = std::find(
    _signal_list.begin(),
    _signal_list.end(),
    in
  );
  if (i != _signal_list.end())
    _signal_list.erase(i);
}

list::size_type Callback_Base::signal_size() const
{
  return _signal_list.size();
}

void Callback_Base::connect_add(Signal_Base * in)
{
  _connect_list += in;
}

void Callback_Base::connect_del(Signal_Base * in)
{
  const List<Signal_Base *>::iterator i = std::find(
    _connect_list.begin(),
    _connect_list.end(),
    in
  );
  if (i != _connect_list.end())
    _connect_list.erase(i);
}

list::size_type Callback_Base::connect_size() const
{
  return _connect_list.size();
}
  
void Callback_Base::callbacks(bool in)
{
  _callbacks = in;
}

bool Callback_Base::callbacks() const
{
  return _callbacks;
}

//------------------------------------------------------------------------------
// Signal_Base
//------------------------------------------------------------------------------

Signal_Base::~Signal_Base()
{}

//------------------------------------------------------------------------------

}
}

