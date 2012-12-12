#define DJV_DEBUG

#include <djv_base/application.h>

using namespace djv_base;

void cmdln(String & in) throw (Error)
{
  DJV_DEBUG("cmdln");
  DJV_DEBUG_PRINT("in = " << in);
  
  List<String> arg_list;
  String arg;
  try
  {
    while (! in.empty())
    {
      in >> arg;

      if ("-bool" == arg)
      {
        bool value = false;
        in >> value;
        DJV_DEBUG_PRINT("bool = " << value);
      }

      else arg_list += arg;
    }
  }
  catch (String)
  {
    throw Error(arg);
  }
  
  if (! arg_list.size())
    throw Error("input 1");
  DJV_DEBUG_PRINT("input 1 = " << arg_list.pop_front());
  
  if (! arg_list.size())
    throw Error("input 2");
  DJV_DEBUG_PRINT("input 2 = " << arg_list.pop_front());

  if (! arg_list.empty()) throw Error(arg_list[0]);
}

int main(int argc, char ** argv)
{
  List<String> cmdln_list;
  for (int i = 1; i < argc; ++i)
    cmdln_list += string::serialize(String(argv[i]));
  String cmdln_arg = string::join(cmdln_list, ' ');
  
  try
  {
    cmdln(cmdln_arg);
  }
  catch (Error in)
  {
    error::print(in);
    return 1;
  }
  
  return 0;
}

