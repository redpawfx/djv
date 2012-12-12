/*------------------------------------------------------------------------------
 lib/djv_ui/value.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "value.h"

#include <djv_ui/tool_button.h>
#include <djv_ui/layout_row.h>
#include <FL/Fl.H>

namespace djv_ui {
namespace value {

//------------------------------------------------------------------------------
// Int_Input_Base
//------------------------------------------------------------------------------

Int_Input_Base::Int_Input_Base(int min, int max) :
  Value_Input_Base<int>( min, max)
{
  inc(1, 10);
}

//------------------------------------------------------------------------------
// Float_Input_Base
//------------------------------------------------------------------------------

Float_Input_Base::Float_Input_Base(double min, double max) :
  Value_Input_Base<double>(min, max),
  _precision(0)
{
  inc(0.1, 1.0);
}

void Float_Input_Base::precision(int in)
{
  if (in == _precision)
    return;
  
  _precision = in;
  
  set(get());
  
  value_callback();
}

int Float_Input_Base::precision() const
{
  return _precision;
}

//------------------------------------------------------------------------------
// Inc_Tool::_Inc_Tool
//------------------------------------------------------------------------------

struct Inc_Tool::_Inc_Tool
{
  Tool_Button * inc, * dec;
};

//------------------------------------------------------------------------------
// Inc_Tool
//------------------------------------------------------------------------------

Inc_Tool::Inc_Tool() :
  inc_signal(this),
  dec_signal(this),
  push_signal(this),
  _p(new _Inc_Tool)
{
  debug_path("value::Inc_Tool");
  
  // Create widgets.
  
  _p->inc =
    new Tool_Button("tool_inc");
  _p->inc->type(tool_button::REPEAT);
  _p->inc->debug_path("inc");

  _p->dec =
    new Tool_Button("tool_dec");
  _p->dec->type(tool_button::REPEAT);
  _p->dec->debug_path("dec");
  
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  layout->spacing(0);
  layout->margin(0);
  layout->add(new Frame_Group_V(List<Frame *>() <<
    _p->inc <<
    _p->dec));

  // Callbacks.
  
  _p->inc->signal.set(this, inc_callback);
  _p->inc->push_signal.set(this, push_callback);
  _p->dec->signal.set(this, dec_callback);
  _p->dec->push_signal.set(this, push_callback);
}

Inc_Tool::~Inc_Tool()
{}

void Inc_Tool::inc_callback(bool)
{
  inc_signal.emit(true);
}

void Inc_Tool::dec_callback(bool)
{
  dec_signal.emit(true);
}

void Inc_Tool::push_callback(bool in)
{
  push_signal.emit(in);
}

//------------------------------------------------------------------------------

}
}

