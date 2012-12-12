#include <djv_ui/application.h>
#include <djv_ui/choice.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/window.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);
  
  ~Application();

private:

  DJV_CALLBACK(Application, callback, int)
  {
    print(djv_base::string::Format("choice = %%").arg(in));
  }
  DJV_CALLBACK(Application, callback2, int)
  {
    print(djv_base::string::Format("choice 2 = %%").arg(in));
  }
  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_ui::Window * _window;
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("choice", argc, argv),
  _window(0)
{
  using namespace djv_base;
  using namespace djv_ui;

  is_valid(true);
  
  const List<String> data = List<String>() <<
    "MacReady" <<
    "Blair" <<
    "Nauls" <<
    "Palmer" <<
    "Clark";

  // Widgets.
  
  Choice * widget = new Choice(data);
  
  Choice * widget2 = new Choice(data);

  _window = new djv_ui::Window(name());
  
  // Layout.
  
  layout_item::group(List<Layout_Item *>() << widget << widget2);

  Layout_V * layout = new Layout_V(_window);
  layout->add(widget);
  layout->add(widget2);
  
  // Initialize.

  _window->size(_window->size_hint());
  _window->show();

  // Callbacks.
  
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

