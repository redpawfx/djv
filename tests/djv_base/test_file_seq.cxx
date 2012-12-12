#define DJV_DEBUG

#include <djv_base/file_seq.h>

using namespace djv_base;

void seq_to_string()
{
  DJV_DEBUG("seq_to_string");
  
  const struct Data { file_seq::Seq a; String b; } data [] =
  {
    { file_seq::Seq(List<int64_t>()), "" },
    { file_seq::Seq(List<int64_t>() << 1), "1" },
    { file_seq::Seq(List<int64_t>() << 1 << 2), "1-2" },
    { file_seq::Seq(List<int64_t>() << 1 << 2 << 3), "1-3" },
    { file_seq::Seq(List<int64_t>() << 1 << 2 << 3, 4), "0001-0003" },
    { file_seq::Seq(List<int64_t>() << 3 << 2 << 1), "3-1" },
    { file_seq::Seq(List<int64_t>() << 1 << 2 << 3 << 5), "1-3,5" },
    { file_seq::Seq(List<int64_t>() << 1 << 2 << 3 << 5 << 6), "1-3,5-6" },
    { file_seq::Seq(List<int64_t>() << 1 << 2 << 3 << 5 << 6 << 7), "1-3,5-7" },
    { file_seq::Seq(List<int64_t>() << 1 << 2 << 3 << 3 << 2 << 1), "1-3,3-1" }
  };
  const int data_size = sizeof(data) / sizeof(Data);

  for (int i = 0; i < data_size; ++i)
  {
    const String tmp = file_seq::seq_to_string(data[i].a);
    
    DJV_DEBUG_PRINT(data[i].a.list << " = " << tmp);
    
    DJV_ASSERT(tmp == data[i].b);
  }
}

void string_to_seq()
{
  DJV_DEBUG("string_to_seq");
  
  const struct Data { String a; file_seq::Seq b; } data [] =
  {
    { "", file_seq::Seq(List<int64_t>() << 0) },
    { "1", file_seq::Seq(List<int64_t>() << 1) },
    { "1-2", file_seq::Seq(List<int64_t>() << 1 << 2) },
    { "1-3", file_seq::Seq(List<int64_t>() << 1 << 2 << 3) },
    { "0001-0003", file_seq::Seq(List<int64_t>() << 1 << 2 << 3, 4) },
    { "3-1", file_seq::Seq(List<int64_t>() << 3 << 2 << 1) },
    { "1,2,3", file_seq::Seq(List<int64_t>() << 1 << 2 << 3) },
    { "1-3,5", file_seq::Seq(List<int64_t>() << 1 << 2 << 3 << 5) },
    { "1-3,5-6", file_seq::Seq(List<int64_t>() << 1 << 2 << 3 << 5 << 6) },
    { "1-3,5-7", file_seq::Seq(List<int64_t>() << 1 << 2 << 3 << 5 << 6 << 7) },
    { "1-3,3-1", file_seq::Seq(List<int64_t>() << 1 << 2 << 3 << 3 << 2 << 1) }
  };
  const int data_size = sizeof(data) / sizeof(Data);

  for (int i = 0; i < data_size; ++i)
  {
    const file_seq::Seq tmp = file_seq::string_to_seq(data[i].a);
    
    DJV_DEBUG_PRINT(data[i].a << " = " << tmp);
    
    DJV_ASSERT(tmp == data[i].b);
  }
}

int main(int argc, char ** argv)
{
  seq_to_string();
  string_to_seq();
  
  return 0;
}

