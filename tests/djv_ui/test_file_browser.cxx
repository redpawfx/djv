#include <djv_ui/application.h>
#include <djv_ui/file_browser.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);

private:

  DJV_CALLBACK(Application, widget_callback, const djv_base::File &)
  {
    print(in);
  }
  
  DJV_CALLBACK(Application, close_callback, bool) { exit(); }
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("file_browser", argc, argv)
{
  using namespace djv_ui;

  is_valid(true);
  
  file_browser::set(this, widget_callback);
  file_browser::global()->close_signal.set(this, close_callback);
}

int main(int argc, char ** argv)
{
  return Application(argc, argv).exec();
}

