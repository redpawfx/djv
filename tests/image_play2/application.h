#ifndef TEST_IMAGE_PLAY2_APPLICATION_H
#define TEST_IMAGE_PLAY2_APPLICATION_H

#include "widget.h"
#include <djv_ui/application.h>
#include <djv_ui/window.h>
#include <djv_base/file.h>

struct Image_Ptr
{
  Image_Ptr() :
    p(0),
    frame(0),
    next(0)
  {}
  
  Image * p;
  int frame;
  Image_Ptr * next;
};

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int argc, char ** argv);
  
  ~Application();
  
  void update();

private:

  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  File _file;
  int _frame;
  
  Image_Ptr * _image0, * _image1;
  List<Image_Ptr> _image_buf;

  djv_ui::Window * _window;
  Widget * _widget;
};

#endif

