#include <djv_ui/application.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/slider.h>
#include <djv_ui/window.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);
  
  ~Application();

private:

  DJV_CALLBACK(Application, widget_callback, int)
  {
    print(djv_base::string::Format("value = %%").arg(in));
  }
  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_ui::Window * _window;
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("slider", argc, argv),
  _window(0)
{
  using namespace djv_base;
  using namespace djv_ui;

  is_valid(true);
  
  Int_Slider * slider = new Int_Slider;
  //slider->range(0, 10);
  
  _window = new djv_ui::Window(name());
  
  Layout_V * layout = new Layout_V(_window);
  layout->add(slider);
  
  _window->size(_window->size_hint());
  _window->show();

  slider->signal.set(this, widget_callback);
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

