#define DJV_DEBUG

#include <djv_base/math.h>

using namespace djv_base;

int main(int argc, char ** argv)
{
  DJV_DEBUG("math_range");
  
  typedef math::Range<int64_t> Range;
  
  const struct Data
  {
    List<int64_t> a;
    List<Range> b;
  }
  data [] =
  {
    {
      List<int64_t>() << 1,
      List<Range>() << Range(1, 1)
    },
    {
      List<int64_t>() << 1 << 2,
      List<Range>() << Range(1, 2)
    },
    {
      List<int64_t>() << 1 << 2 << 3,
      List<Range>() << Range(1, 3)
    },
    {
      List<int64_t>() << 1 << 3,
      List<Range>() << Range(1) << Range(3)
    },
    {
      List<int64_t>() << 1 << 2 << 4,
      List<Range>() << Range(1, 2) << Range(4)
    },
    {
      List<int64_t>() << 1 << 2 << 4 << 5,
      List<Range>() << Range(1, 2) << Range(4, 5)
    }
  };
  const int data_size = sizeof(data) / sizeof(Data);

  for (int i = 0; i < data_size; ++i)
  {
    const List<Range> tmp = math::range(data[i].a);
    DJV_DEBUG_PRINT(tmp << " = " << data[i].b);
    DJV_ASSERT(tmp == data[i].b);
  }
  
  return 0;
}
