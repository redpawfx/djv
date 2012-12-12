#define DJV_DEBUG

#include <djv_base/file.h>

using namespace djv_base;

void absolute()
{
  DJV_DEBUG("absolute");
  
  const struct Data { String path; } data [] =
  {
    { "." },
    { ".." },
    { "/" }
  };
  const int data_size = sizeof(data) / sizeof(Data);

  for (int i = 0; i < data_size; ++i)
  {
    const String tmp = file::path_absolute(data[i].path);
    DJV_DEBUG_PRINT(data[i].path << " = " << tmp);
  }
}

void fix()
{
  DJV_DEBUG("fix");
  
  const struct Data { String path; } data [] =
  {
    { file::path_absolute(".") },
    { "/Documents and Settings/darby/Desktop" }
  };
  const int data_size = sizeof(data) / sizeof(Data);

  for (int i = 0; i < data_size; ++i)
    DJV_DEBUG_PRINT(data[i].path << " = " << file::path_fix(data[i].path));
}

int main(int argc, char ** argv)
{
  absolute();
  fix();

  return 0;
}

