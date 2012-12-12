//#define DJV_DEBUG

#include <djv_ui/application.h>
#include <djv_ui/color.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/window.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);
  
  ~Application();

private:

  DJV_CALLBACK(Application, widget_callback, const djv_image::Color &);
  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_ui::Color_Swatch * _swatch;
  djv_ui::Window * _window;
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("color_widget", argc, argv),
  _window(0)
{
  using namespace djv_ui;

  is_valid(true);
  
  Color_Widget * widget = new Color_Widget;
  _swatch = new Color_Swatch;
  _swatch->size(layout_item::STRETCH, layout_item::STRETCH);
  _window = new djv_ui::Window(name());
  
  Layout_H * layout = new Layout_H(_window);
  layout->add(_swatch);
  Layout_V * layout_v = new Layout_V(layout);
  layout_v->margin(0);
  layout->stretch(layout_v);
  layout_v->add(widget);

  //widget::debug_children(widget);
  
  _window->size(_window->size_hint());
  _window->show();
  
  widget->signal.set(this, widget_callback);
  _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
  if (_window)
    _window->del();
}

void Application::widget_callback(const djv_image::Color & in)
{
  _swatch->set(in);
  
  print(djv_base::string::Format("value = %%").
    arg(djv_base::string::label(in)));
}

int main(int argc, char ** argv)
{
  return Application(argc, argv).exec();
}

