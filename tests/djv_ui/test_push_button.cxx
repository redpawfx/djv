//#define DJV_DEBUG

#include <djv_ui/application.h>
#include <djv_ui/push_button.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/window.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);
  
  ~Application();

private:

  DJV_CALLBACK(Application, callback, bool) { print("button"); }
  DJV_CALLBACK(Application, callback2, bool) { print("button 2"); }
  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_ui::Window * _window;
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("push_button", argc, argv),
  _window(0)
{
  using namespace djv_ui;
  
  DJV_DEBUG("Application::Application");

  is_valid(true);
  
  Push_Button * widget = new Push_Button("Button");
  Push_Button * widget2 = new Push_Button("Button 2");
  _window = new djv_ui::Window(name());

  Layout_V * layout = new Layout_V(_window);
  layout->add(widget);
  layout->add(widget2);
  
  _window->size(_window->size_hint());

  DJV_DEBUG_PRINT("widget = " << widget->size_hint());
  DJV_DEBUG_PRINT("widget2 = " << widget2->size_hint());
  DJV_DEBUG_PRINT("window = " << _window->size_hint());
  
  _window->show();

  widget->signal.set(this, callback);
  widget2->signal.set(this, callback2);
  _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
  if (_window)
    _window->del();
}

int main(int argc, char ** argv)
{
  return Application(argc, argv).exec();
}

