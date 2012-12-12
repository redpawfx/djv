#include <djv_base/application.h>
#include <djv_base/directory.h>
#include <djv_base/file.h>
#include <djv_base/system.h>
#include <djv_base/user.h>

struct Application : public djv_base::application::Application_Base
{
  Application(int & argc, char ** argv);
};

Application::Application(int & argc, char ** argv) :
  djv_base::application::Application_Base("test_directory", argc, argv)
{
  using namespace djv_base;
  
  if (argc != 1 && argc != 2)
  {
    print("usage: test_directory [path]");
    exit(application::EXIT_ERROR);
    return;
  }

do
{
  File::seq_extension_list(List<String>() << ".dpx" << ".tga" << ".tiff");
  
  Directory directory(2 == argc ? argv[1] : ".");
  List<File> list = directory.list();
  
  file::seq_compress(list);
  file::filter(list, file::FILTER_HIDDEN);
  file::sort(list, file::SORT_TIME, true);
  file::sort_directories_first(list);
  
  const int terminal_width = djv_base::system::terminal_width();

  for (list::size_type i = 0; i < list.size(); ++i)
  {
    const String file = list[i].get(-1, false);
    
    const String info_string = string::Format("%% %% %% %% %%").
      arg(file::label_size(list[i].size())).
      arg(user::uid_to_string(list[i].user())).
      arg(file::label_type()[list[i].type()], 4).
      arg(file::label_perm(list[i].perm()), 3).
      arg(djv_base::time::time_to_string(list[i].time())
    );

    print(
      string::Format("%% %%").
      arg(file).
      arg(info_string, terminal_width - static_cast<int>(file.size()) - 2)
    );
  }

}
while (0);
}

int main(int argc, char ** argv)
{
  return Application(argc, argv).exec();
}

