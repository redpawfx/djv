#define DJV_DEBUG

#include <djv_base/directory.h>

using namespace djv_base;

int main(int argc, char ** argv)
{
  DJV_DEBUG("directory_fnc");
  
  DJV_DEBUG_PRINT("desktop = " << directory::desktop());
  DJV_DEBUG_PRINT("home = " << directory::home());
  DJV_DEBUG_PRINT("drive list = " << directory::drive_list());
  DJV_DEBUG_PRINT("root = " << directory::root());

  return 0;
}

