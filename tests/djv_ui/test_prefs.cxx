#define DJV_DEBUG

#include <djv_ui/application.h>
#include <djv_ui/prefs.h>

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int & argc, char ** argv);
};

Application::Application(int & argc, char ** argv) :
  djv_ui::application::Application_Base("test_prefs", argc, argv)
{
  djv_base::List<bool> list_bool = djv_base::List<bool>() <<
    true <<
    false <<
    true <<
    false;
  
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "test_prefs");
  djv_ui::prefs::set(&prefs, "list_bool", list_bool);
}

int main(int argc, char ** argv)
{
  return Application(argc, argv).exec();
}

