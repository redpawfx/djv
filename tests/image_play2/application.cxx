//#define DJV_DEBUG

#include "application.h"

#include <djv_ui/layout_row.h>
#include <FL/Fl.H>

void update_callback(void *);

Application::Application(int argc, char ** argv) :
  djv_ui::application::Application_Base("test_image_play2", argc, argv),
  _frame(0),
  _image0(0),
  _image1(0),
  _window(0)
{    
  // Command line.
  
  if (argc != 2)
  {
    print("usage: test_image_play2 (input)");
    djv_image::application::Application_Base::exit(application::EXIT_ERROR);
    return;
  }
  
  _file.set(argv[1]);
  
  if (_file.is_seq_valid())
    _file.type(file::SEQ);

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
  
  _image_buf.resize(20);
  int i = 0;
  for (; i < _image_buf.size(); ++i)
    _image_buf[i].p = new Image_Ppm;
  i = 0;
  for (; i < _image_buf.size() - 1; ++i)
    _image_buf[i].next = &_image_buf[i + 1];
  _image_buf[i].next = &_image_buf[0];
  
  _image0 = &_image_buf[0];
  _image0->p->load(_file.get(_file.seq().list[_frame]));
  _image0->frame = _frame;
  
  _image1 = &_image_buf[0];
  
  //update();
  Fl::add_idle(update_callback, this);

  _window->size(_image0->p->info().size);
  _window->show();

  // Callbacks.
  
  _window->close_signal.set(this, exit_callback);
}

Application::~Application()
{
  if (_window) _window->del();
  
  for (list::size_type i = 0; i < _image_buf.size(); ++i)
    if (_image_buf[i].p)
      delete _image_buf[i].p;
}

void Application::update()
{
  DJV_DEBUG("Application::update");
  
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
  
  try
  {
    //_image->load(_file.get(_file.seq().list[_frame]));
    
    _widget->set(_image0->p);
    _widget->redraw();
    
    int frame = _image1->frame + 1;
    if (_image_buf.size() > 1)
    {
      while (_image1->next != _image0 && frame < _file.seq().list.size())
      {
        DJV_DEBUG_PRINT("advance = " << frame);
      
        _image1 = _image1->next;
        _image1->p->load(_file.get(_file.seq().list[frame]));
        _image1->frame = frame;

        ++frame;
      }
    }
    else
    {
      if (frame < _file.seq().list.size())
      {
        _image1 = _image1->next;
        _image1->p->load(_file.get(_file.seq().list[frame]));
        _image1->frame = frame;
      }
    }
    
    _image0 = _image0->next;
  }
  catch (Error error)
  {
    djv_base::application::Application_Base::error(error);
  }

  ++_frame;
  if (_frame >= static_cast<int>(_file.seq().list.size()))
    //_frame = 0;
    exit();
}

void update_callback(void * in)
{
  reinterpret_cast<Application *>(in)->update();
}

int main(int argc, char ** argv)
{
  return Application(argc, argv).exec();
}

