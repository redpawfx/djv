#include <djv_ui/application.h>
#include <djv_ui/color.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/tabs.h>
#include <djv_ui/tool_button.h>
#include <djv_ui/window.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);
  
  ~Application();

private:

  void add();
  void del();
  
  DJV_CALLBACK(Application, widget_callback, int)
  {
    print(djv_base::string::Format("tab = %%").arg(in));
  }
  DJV_CALLBACK(Application, add_callback, bool) { add(); }
  DJV_CALLBACK(Application, del_callback, bool) { del(); }
  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_ui::tabs::Tabs * _widget;
  djv_ui::Window * _window;
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("tab", argc, argv),
  _window(0)
{
  using namespace djv_base;
  using namespace djv_ui;

  is_valid(true);
  
  // Widgets.
   
  _widget = new tabs::Tabs;

  Tool_Button * add_button = new Tool_Button("tool_add");
  add_button->type(tool_button::REPEAT);
  Tool_Button * del_button = new Tool_Button("tool_x");
  del_button->type(tool_button::REPEAT);

  _window = new djv_ui::Window(name());
  
  // Layout.
  
  Layout_V * layout = new Layout_V(_window);
  layout->spacing(0);
  layout->margin(0);
  layout->add(_widget);
  layout->stretch(_widget);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout_h->add_stretch();
  layout_h->add(add_button);
  layout_h->add(del_button);
  
  // Initialize.
  
  add();
  
  _window->size(V2i(300, 300));
  _window->show();
  
  // Callbacks.
  
  _widget->signal.set(this, widget_callback);
  add_button->signal.set(this, add_callback);
  del_button->signal.set(this, del_callback);
  _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
  if (_window)
    _window->del();
}

void Application::add()
{
  _widget->tab_add(
    djv_base::string::Format("tab %%").arg(_widget->tab_size()),
    new djv_ui::Color_Widget
  );
}

void Application::del()
{
  if (! _widget->tab_size())
    return;
  
  _widget->tab_del(_widget->get());
}

int main(int argc, char ** argv)
{
  return Application(argc, argv).exec();
}

