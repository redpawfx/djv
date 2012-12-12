#include <djv_ui/application.h>
#include <djv_ui/image_view.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/window.h>
#include <djv_image/io.h>
#include <djv_base/file.h>
#include <FL/Fl.H>

class Widget : public djv_ui::image::View
{
public:

  int handle(int in)
  {
    const djv_base::V2i mouse(Fl::event_x(), geom().h - 1 - Fl::event_y());
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

  djv_base::V2i _view_tmp, _mouse_push;
};

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int argc, char ** argv);
  
  ~Application();
  
  void update();

private:

  DJV_CALLBACK(Application, exit_callback, bool) { exit(); }

  djv_base::File _file;
  djv_image::io::Load * _load;
  djv_image::io::Info _info;
  djv_ui::Window * _window;
  Widget * _widget;
  int _frame;
};

void update_callback(void *);

Application::Application(int argc, char ** argv) :
  djv_ui::application::Application_Base("test_image_play", argc, argv),
  _load(0),
  _window(0),
  _frame(0)
{
  using namespace djv_base;
  
  /*djv_image::gl::fixed(true);
  djv_image::gl_data::texture_tile(V2i(64));
  djv_image::gl_data::texture_debug(true);*/
  
  // Command line.
  
  if (argc != 2)
  {
    print("usage: test_image_play (input)");
    djv_image::application::Application_Base::exit(application::EXIT_ERROR);
    return;
  }
  
  _file.set(argv[1]);
  if (_file.is_seq_valid())
    _file.type(file::SEQ);
  try
  {
    _load = djv_image::io::global()->load(_file, &_info);
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
  
  // Layout.
  
  djv_ui::Layout_V * layout = new djv_ui::Layout_V(_window);
  layout->margin(0);
  layout->add(_widget);
  layout->stretch(_widget);
  
  // Initialize.

  //update();
  Fl::add_idle(update_callback, this);

  //_widget->zoom(0.5);
  
  _window->size(_info.size);
  _window->show();

  // Callbacks.
  
  _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
  if (_window) _window->del();
  if (_load) delete _load;
}

void Application::update()
{
  using namespace djv_base;
  
  static djv_base::time::Timer t;
  static double average = 0.0;
  static int accum = 0;
  t.check();
  const double fps = t.fps();
  t.start();
  if (fps < 1000.0)
  {
    average += fps;
    ++accum;
  }
  print(string::Format("FPS = %% (%%)").
    arg(fps).
    arg(average / static_cast<double>(accum)));
  
  const djv_image::Image * image = 0;
  try { image = _load->get(_info.time.list[_frame]); }
  catch (Error) {}
  if (image)
  {
    _widget->set(image);
    djv_image::gl_data::Option option;
    option.color_profile = image->color_profile;
    _widget->option(option);
    _widget->redraw();
  }

  ++_frame;
  if (_frame >= static_cast<int>(_info.time.list.size()))
    _frame = 0;
}

void update_callback(void * in)
{
  reinterpret_cast<Application *>(in)->update();
}

int main(int argc, char ** argv)
{
  return Application(argc, argv).exec();
}

