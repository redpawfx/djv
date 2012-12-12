#ifndef APPLICATION_H
#define APPLICATION_H

#include "op.h"
#include "widget.h"
#include <djv_ui/application.h>
#include <djv_ui/slider.h>
#include <djv_ui/tool_button.h>
#include <djv_ui/window.h>
#include <djv_image/io.h>

namespace glsl {
using namespace djv_base;

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int argc, char ** argv);
  
  ~Application();
  
private:

  void play(bool);

  DJV_CALLBACK(Application, play_callback, bool);
  DJV_CALLBACK(Application, frame_callback, int);
  DJV_CALLBACK(Application, op_callback, int);
  DJV_CALLBACK(Application, update_callback, bool);
  DJV_CALLBACK(Application, close_callback, bool);
  DJV_FL_CALLBACK(Application, idle_callback);
  
  void play_update();
  void frame_update();
  void update();

  std::auto_ptr<djv_image::io::Load> _load;
  djv_image::io::Info _info;
  bool _play;
  int _frame;
  List<Op *> _op_list;
  int _op;
  
  djv_ui::window::Window * _window_image;
  Widget * _view;
  djv_ui::Tool_Button * _play_widget;
  djv_ui::Int_Slider * _slider;
  djv_ui::window::Window * _window_widget;
};

}

#endif
