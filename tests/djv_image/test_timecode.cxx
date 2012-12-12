#define DJV_DEBUG

#include <djv_image/time.h>

using namespace djv_base;
using namespace djv_image;

void convert()
{
  DJV_DEBUG("convert");

  const struct Data
  {
    String a, b;
  }
  data [] =
  {
    { "00:00:00:00", "00:00:00:00" },
    { "12:59:59:119", "12:59:59:119" },
    { "1", "00:00:00:1" },
    { "1:2", "00:00:01:02" },
    { "1:2:3", "00:01:02:03" },
    { "1:2:3:4", "01:02:03:04" }
  };
  const int data_size = sizeof(data) / sizeof(Data);

  for (int i = 0; i < data_size; ++i)
  {
    const uint32_t a(djv_image::time::string_to_timecode(data[i].a));
    const uint32_t b(djv_image::time::string_to_timecode(data[i].b));
    DJV_DEBUG_PRINT(a << " = " << b);
    DJV_ASSERT(a == b);
  }
}

int main(int argc, char ** argv)
{
  convert();

  return 0;
}

