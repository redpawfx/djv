/*------------------------------------------------------------------------------
 lib/djv_ui/dialog_fnc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "dialog.h"

namespace djv_ui {
namespace dialog {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  label_message = "Message Dialog",
  label_question = "Question Dialog",
  label_input = "Input Dialog",
  label_choice = "Choice Dialog",
  label_multi_choice = "Multiple Choice Dialog",
  label_progress = "Progress Dialog",
  label_progress_estimate = "Estimated: %% (%% Frames/Second)",
  label_progress_elapsed = "Elapsed: %%",
  label_color = "Color Dialog",
  label_ok = "&Ok",
  label_close = "Clos&e",
  label_cancel = "&Cancel",
  label_yes = "&Yes",
  label_no = "&No",
  label_show = "Show",
  label_pin = "&Pin";

extern const String
  tooltip_pin = "Don't close the dialog after choosing a file";

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() :
  _message(new Message),
  _question(new Question),
  _input(new Input),
  _choice(new Choice),
  _multi_choice(new Multi_Choice),
  _progress(new Progress),
  _color(new Color)
{}

Global::~Global()
{}

void Global::message(const String & label) const
{
  _message->label(label);
  _message->show();
}

void Global::question(
  const String & label,
  Callback_Base * in,
  callback::Signal<bool>::Callback * callback
) const {
  _question->signal.del();
  
  _question->label(label);
  
  _question->signal.set(in, callback);
  
  _question->show();
}

void Global::input(
  const String & label,
  const String & value,
  Callback_Base * in,
  callback::Signal<const String &>::Callback * callback
) const {
  _input->signal.del();
  
  _input->set(value);
  _input->label(label);
  
  _input->signal.set(in, callback);
  
  _input->show();
}

void Global::choice(
  const String & label,
  const List<String> & list,
  Callback_Base * in,
  callback::Signal<int>::Callback * callback
) const {
  _choice->signal.del();
  
  _choice->list(list);
  _choice->label(label);
  
  _choice->signal.set(in, callback);
  
  _choice->show();
}

void Global::multi_choice(
  const String & label,
  const List<String> & list,
  Callback_Base * in,
  callback::Signal<const List<bool> &>::Callback * callback
) const {
  _multi_choice->signal.del();
  _multi_choice->index_signal.del();
  
  _multi_choice->list(list);
  _multi_choice->label(label);
  
  _multi_choice->signal.set(in, callback);
  
  _multi_choice->show();
}

void Global::multi_choice(
  const String & label,
  const List<String> & list,
  Callback_Base * in,
  callback::Signal<const List<int> &>::Callback * callback
) const {
  _multi_choice->signal.del();
  _multi_choice->index_signal.del();
  
  _multi_choice->list(list);
  _multi_choice->label(label);
  
  _multi_choice->index_signal.set(in, callback);
  
  _multi_choice->show();
}

void Global::progress(
  const String & label,
  int value,
  Callback_Base * in,
  callback::Signal<int>::Callback * callback,
  callback::Signal<bool>::Callback * del_callback
) const {
  _progress->signal.del();
  _progress->del_signal.del();
  
  _progress->set(value);
  _progress->label(label);
  
  _progress->signal.set(in, callback);
  _progress->del_signal.set(in, del_callback);
  
  _progress->show();
}

void Global::progress_del(Callback_Base * in) const
{
  _progress->signal.del(in);
  _progress->hide();
}

void Global::color(
  const djv_image::Color & value,
  Callback_Base * in,
  callback::Signal<const djv_image::Color &>::Callback * callback
) const {
  _color->signal.del();
  
  _color->set(value);

  if (callback)
    _color->signal.set(in, callback);

  _color->show();
}

//------------------------------------------------------------------------------

}
}
