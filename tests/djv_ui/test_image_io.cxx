#include <djv_ui/application.h>
#include <djv_ui/image_io.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/tabs.h>
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
  djv_ui::application::Application_Base("image_io", argc, argv),
  _window(0)
{
  using namespace djv_ui;

  is_valid(true);
  
  // Widgets.
  
  tabs::Tabs * tabs_widget = new tabs::Tabs;

  tabs::Tabs * load_tabs_widget = new tabs::Tabs;
  tabs_widget->tab_add("Load", load_tabs_widget);
  const List<djv_image::io::Load *> & load_list =
    djv_image::io::global()->load()->list();
  for (list::size_type i = 0; i < load_list.size(); ++i)
  {
    image_io::Load * widget = image_io::global()->load()->get(load_list[i]);
    if (! widget) continue;
    widget->set(load_list[i]);
    load_tabs_widget->tab_add(load_list[i]->name(), widget);
  }

  tabs::Tabs * save_tabs_widget = new tabs::Tabs;
  tabs_widget->tab_add("Save", save_tabs_widget);
  const List<djv_image::io::Save *> & save_list =
    djv_image::io::global()->save()->list();
  for (list::size_type i = 0; i < save_list.size(); ++i)
  {
    image_io::Save * widget = image_io::global()->save()->get(save_list[i]);
    if (! widget) continue;
    widget->set(save_list[i]);
    save_tabs_widget->tab_add(save_list[i]->name(), widget);
  }

  _window = new djv_ui::Window(name());

  // Layout.
  
  Layout_V * layout = new Layout_V(_window);
  layout->margin(0);
  layout->add(tabs_widget);
  
  // Initialize.
  
  _window->size(V2i(500, 400));
  _window->show();

  // Callbacks.
  
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

