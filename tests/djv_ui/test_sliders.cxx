#include <djv_ui/application.h>
#include <djv_ui/float_value.h>
#include <djv_ui/int_value.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/window.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);
  
  ~Application();

private:

  DJV_CALLBACK(Application, int_callback, int)
  {
    print(djv_base::string::Format("value = %%").arg(in));
  }
  DJV_CALLBACK(Application, float_callback, double)
  {
    print(djv_base::string::Format("value = %%").arg(in));
  }
  
  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_ui::Window * _window;
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("sliders", argc, argv),
  _window(0)
{
  using namespace djv_base;
  using namespace djv_ui;

  is_valid(true);
  
  // Widgets.
  
  Int_Edit_Slider * int_widget = new Int_Edit_Slider;
  int_widget->range(0, 1000);
  int_widget->inc(10, 100);
  //int_widget->range(0, 10);
  
  Float_Edit_Slider * float_widget = new Float_Edit_Slider;
  float_widget->range(0.0, 100.0);
  float_widget->inc(1.0, 10.0);
  //float_widget->range(0.0, 3.0);
  
  _window = new djv_ui::Window(name());
  
  // Layout.
  
  Layout_V * layout = new Layout_V(_window);
  layout->add(int_widget);
  layout->add(float_widget);

  //widget::debug_children(_window);
  
  // Initialize.
  
  _window->size(_window->size_hint());
  _window->show();

  // Callbacks.
  
  int_widget->signal.set(this, int_callback);
  float_widget->signal.set(this, float_callback);
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

