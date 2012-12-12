#include <djv_ui/layout_row.h>
#include <djv_ui/window.h>
#include <djv_ui/image_view.h>
#include <djv_ui/application.h>
#include <djv_image/io.h>
#include <djv_base/file.h>
#include <FL/Fl.H>

using namespace djv_base;

class Widget : public djv_ui::image::View
{
public:

  int handle(int in)
  {
    const V2i mouse(
      Fl::event_x(),
      geom().h - 1 - Fl::event_y()
    );
    
    switch (in)
    {
      case FL_SHORTCUT:
        switch (Fl::event_key())
        {
          case '-': zoom(zoom() * 0.5, mouse); redraw(); return 1;
          case '=': zoom(zoom() * 2.0, mouse); redraw(); return 1;
          case FL_BackSpace: view_zero(); redraw(); return 1;
        }
        break;
      
      case FL_PUSH:
        _view_tmp = view();
        _mouse_push = mouse;
        return 1;
      
      case FL_DRAG:
        view(_view_tmp + mouse - _mouse_push);
        redraw();
        return 1;
    }
    
    return djv_ui::image::View::handle(in);
  }
  
private:

  V2i _view_tmp;
  V2i _mouse_push;
};

/*class Widget : public djv_ui::gl::Gl
{
public:

  void draw()
  {
    //Gl::draw();

    glClearColor(1, 0.5, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
  }
};*/

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);
  
  ~Application();
  
private:

  void open(const File &);
  
  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_image::io::Load * _load;
  djv_ui::Window * _window;
  Widget * _widget;
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("image_view", argc, argv),
  _window(0)
{
  /*djv_image::gl::fixed(true);
  djv_image::gl_data::texture_tile(V2i(64));
  djv_image::gl_data::texture_debug(true);*/
  
  // Command line.
  
  if (argc != 2)
  {
    print("usage: test_image_view (input)");
    djv_image::application::Application_Base::exit(application::EXIT_ERROR);
    return;
  }

  const djv_image::Image * image = 0;
  File file(argv[1]);
  try
  {
    _load = djv_image::io::global()->load(file);
    image = _load->get();
  }
  catch (Error error)
  {
    this->error(error);
    djv_image::application::Application_Base::exit(application::EXIT_ERROR);
    return;
  }

  is_valid(true);
  
  // Widgets.

  _window = new djv_ui::Window(name());
  
  _widget = new Widget;
  _widget->set(image);
  
  if (image)
  {
    djv_image::gl_data::Option option;
    option.color_profile = image->color_profile;
    _widget->option(option);
  }

  // Layout.

  djv_ui::Layout_V * layout = new djv_ui::Layout_V(_window);
  layout->spacing(0);
  layout->margin(0);
  layout->add(_widget);
  layout->stretch(_widget);
  
  // Initialize.
  
  if (image)
    _window->size(image->size());
  _window->show();

  // Callbacks.
  
  _window->close_signal.set(this, exit_callback );
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

