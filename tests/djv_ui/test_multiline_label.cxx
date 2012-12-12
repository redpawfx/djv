#include <djv_ui/application.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/label.h>
#include <djv_ui/window.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);
  
  ~Application();

private:

  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_ui::Window * _window;
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("multiline_label", argc, argv),
  _window(0)
{
  DJV_DEBUG("Application::Application");
  
  using namespace djv_ui;

  is_valid(true);
  
  Multiline_Label * label = new Multiline_Label(
    "one two three four five six seven eight nine ten\n"
    "\n"
    "ten nine eight seven six five four three two one");

  _window = new djv_ui::Window(name());

  Layout_V * layout = new Layout_V(_window);
  layout->add(label);
  
  _window->size(_window->size_hint());
  _window->show();

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

