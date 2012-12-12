//#define DJV_DEBUG

#include "application.h"

#include "op_color.h"
#include "op_levels.h"
#include "op_exposure.h"
#include "op_blur.h"
#include "op_sharpen.h"
#include "op_edge.h"
#include "op_scale.h"
#include <djv_ui/layout_row.h>
#include <djv_ui/tabs.h>
#include <djv_image/gl_offscreen.h>
#include <FL/Fl.H>

namespace glsl {

Application::Application(int argc, char ** argv) :
  djv_ui::application::Application_Base("glsl", argc, argv),
  _play(false),
  _frame(0),
  _op(0),
  _window_image(0),
  _window_widget(0)
{
  DJV_DEBUG("Application::Application");
  
  djv_image::gl_offscreen::global()->buffer(djv_image::gl_offscreen::PBUFFER);

  if (argc != 2)
  {
    print("usage: test_glsl (image)");
    return;
  }
  
  try
  {
    File file(argv[1]);
    if (file.is_seq_valid())
      file.type(djv_base::file::SEQ);
    
    _load = std::auto_ptr<djv_image::io::Load>(
      djv_image::io::global()->load(file, &_info)
    );
  }
  catch (Error error)
  {
    this->error(error);
    return;
  }
  
  is_valid(true);
  
  _op_list += new Op_Null();
  _op_list += new op_color::Op();
  _op_list += new op_levels::Op();
  _op_list += new op_exposure::Op();
  _op_list += new op_blur::Op();
  _op_list += new op_sharpen::Op();
  _op_list += new op_edge::Op();
  _op_list += new op_scale::Op();
  
  _window_image = new djv_ui::window::Window;
  _view = new Widget;
  _play_widget = new djv_ui::Tool_Button("playback_forward");
  _play_widget->type(djv_ui::tool_button::TOGGLE);
  _slider = new djv_ui::Int_Slider;
  
  _window_widget = new djv_ui::window::Window;
  djv_ui::tabs::Tabs * tabs_widget = new djv_ui::tabs::Tabs;
  for (list::size_type i = 0; i < _op_list.size(); ++i)
    tabs_widget->tab_add(_op_list[i]->name(), _op_list[i]);
  
  djv_ui::Layout_V * layout = new djv_ui::Layout_V(_window_image);
  layout->margin(0);
  layout->spacing(0);
  layout->add(_view);
  layout->stretch(_view);
  djv_ui::Layout_H * layout_h = new djv_ui::Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(_play_widget);
  layout_h->add(_slider);
  
  layout = new djv_ui::Layout_V(_window_widget);
  layout->margin(0);
  layout->spacing(0);
  layout->add(tabs_widget);

  update();
  
  _slider->range(0, static_cast<int>(_info.time.list.size()) - 1);
  _window_widget->size(V2i(300, 300));
  _window_widget->show();
  _window_image->dirty();
  _window_image->size(_info.size + V2i(0, _window_image->size_min().y));
  _window_image->show();
  tabs_widget->set(_op);
  
  _window_image->close_signal.set(this, close_callback);
  _play_widget->signal.set(this, play_callback);
  _slider->signal.set(this, frame_callback);
  tabs_widget->signal.set(this, op_callback);
  for (list::size_type i = 0; i < _op_list.size(); ++i)
    _op_list[i]->signal.set(this, update_callback);
  _window_widget->close_signal.set(this, close_callback);
}

Application::~Application()
{
  DJV_DEBUG("Application::~Application");
  
  if (_window_image)
    _window_image->del();
  if (_window_widget)
    _window_widget->del();
}

void Application::play_callback(bool in)
{
  _play = in;
  play_update();
}

void Application::frame_callback(int in)
{
  _play = false;
  _frame = in;
  play_update();
  frame_update();
  update();
}

void Application::op_callback(int in)
{
  _op = in;
  update();
}

void Application::update_callback(bool)
{
  update();
}

void Application::play_update()
{
  callbacks(false);
  
  if (_play)
    Fl::add_idle(idle_callback, this);
  else
    Fl::remove_idle(idle_callback, this);
  
  _play_widget->set(_play);
  
  callbacks(true);
}
  
void Application::frame_update()
{
  callbacks(false);
  
  _slider->set(_frame);
  
  callbacks(true);
}

void Application::update()
{
  callbacks(false);
  
  try
  {
    const int64_t frame =
      _info.time.list.size() ? _info.time.list[_frame] : -1;
    _view->set(_op_list[_op], _load->get(frame));
    _view->redraw();
  }
  catch (Error) {}
  
  callbacks(true);
}

void Application::close_callback(bool)
{
  exit();
}

void Application::idle_callback()
{
  _frame++;
  if (_frame >= static_cast<int>(_info.time.list.size()))
    _frame = 0;
  
  frame_update();
  update();
}

}

int main(int argc, char ** argv)
{
  return glsl::Application(argc, argv).exec();
}

