#include <djv_ui/application.h>
#include <djv_ui/check_button.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/push_button.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>
#include <djv_ui/tool_button.h>
#include <djv_ui/window.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);

  ~Application();

private:

  DJV_CALLBACK(Application, push_callback, bool)
  {
    print("push");
  }
  DJV_CALLBACK(Application, check_callback, bool)
  {
    print(djv_base::string::Format("check = %%").arg(in));
  }
  DJV_CALLBACK(Application, radio_callback, int)
  {
    print(djv_base::string::Format("radio = %%").arg(in));
  }
  DJV_CALLBACK(Application, tool_callback0, bool)
  {
    print("tool");
  }
  DJV_CALLBACK(Application, tool_callback1, bool)
  {
    print(djv_base::string::Format("tool = %%").arg(in));
  }
  DJV_CALLBACK(Application, tool_callback2, bool)
  {
    print("tool repeat");
  }
  
  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_ui::window::Window * _window;
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("buttons", argc, argv),
  _window(0)
{
  using namespace djv_base;
  using namespace djv_ui;

  is_valid(true);
  
  // Widgets.
  
  Push_Button * push_button = new Push_Button("&Push Button");
  
  Check_Button * check_button = new Check_Button("&Check button");
  
  Radio_Button_Group * radio_group = new Radio_Button_Group(
    "Radio Buttons",
    List<String>() << "&1" << "&2" << "&3");
  
  Tool_Button * tool_button0 = new Tool_Button("file");
  tool_button0->type(tool_button::PUSH);
  Tool_Button * tool_button1 = new Tool_Button("directory");
  tool_button1->type(tool_button::TOGGLE);
  Tool_Button * tool_button2 = new Tool_Button("magnify");
  tool_button2->type(tool_button::REPEAT);
  
  Frame_Group_H * tool_group = new Frame_Group_H(
    List<Frame *>() <<
    tool_button0 <<
    tool_button1 <<
    tool_button2);

  _window = new djv_ui::Window(name());

  // Layout.
  
  Layout_V * layout = new Layout_V(_window);
  layout->spacing(style::global()->spacing_large());
  layout->add(push_button);
  layout->add(check_button);
  layout->add(radio_group);
  layout->add(tool_group);
  
  layout->add_stretch();
  
  // Initialize.
  
  radio_group->set(0);
  
  _window->size(_window->size_hint());
  _window->show();

  // Callbacks.
  
  push_button->signal.set(this, push_callback);
  check_button->signal.set(this, check_callback);
  radio_group->signal.set(this, radio_callback);
  tool_button0->signal.set(this, tool_callback0);
  tool_button1->signal.set(this, tool_callback1);
  tool_button2->signal.set(this, tool_callback2);
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

