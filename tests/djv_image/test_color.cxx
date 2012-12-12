#define DJV_DEBUG

#include <djv_image/color.h>

using namespace djv_base;
using namespace djv_image;

void convert()
{
  DJV_DEBUG("convert");
  
  const struct Data
  {
    String in;
    PIXEL pixel;
    String out;
  }
  data [] =
  {
    { "l u8 0", pixel::L_U8, "l u8 0" },
    { "l u8 255", pixel::L_U8, "l u8 255" },
    { "l u8 0", pixel::RGB_U10, "rgb u10 0 0 0" },
    { "l u8 255", pixel::RGB_U10, "rgb u10 1023 1023 1023" },
    { "l u8 0", pixel::L_U16, "l u16 0" },
    { "l u8 255", pixel::L_U16, "l u16 65535" },    
    { "l u16 0", pixel::L_U8, "l u8 0" },
    { "l u16 65535", pixel::L_U8, "l u8 255" },
    { "l u16 0", pixel::RGB_U10, "rgb u10 0 0 0" },
    { "l u16 65535", pixel::RGB_U10, "rgb u10 1023 1023 1023" },
    { "l u16 0", pixel::L_U16, "l u16 0" },
    { "l u16 65535", pixel::L_U16, "l u16 65535" },
    { "l f32 -1.0", pixel::L_U8, "l u8 0" },
    { "l f32 0.0", pixel::L_U8, "l u8 0" },
    { "l f32 0.5", pixel::L_U8, "l u8 128" },
    { "l f32 1.0", pixel::L_U8, "l u8 255" },
    { "l f32 2.0", pixel::L_U8, "l u8 255" },
    { "l f32 -1.0", pixel::L_U16, "l u16 0" },
    { "l f32 0.0", pixel::L_U16, "l u16 0" },
    { "l f32 0.5", pixel::L_U16, "l u16 32768" },
    { "l f32 1.0", pixel::L_U16, "l u16 65535" },
    { "l f32 2.0", pixel::L_U16, "l u16 65535" }
  };
  const int data_size = sizeof(data) / sizeof(Data);
  
  for (int i = 0; i < data_size; ++i)
  {
    Color in;
    String tmp = data[i].in;
    tmp >> in;

    Color out(data[i].pixel);
    color::convert(in, out);
    
    DJV_DEBUG_PRINT("str: " << in << " = " << out);
    DJV_ASSERT(string::compare_no_case(string::label(out), data[i].out));
  }
}

template<typename T>
inline String label2(const T & in)
{
  List<String> out;
  
  String value;
  value << in;

  try
  {
    while (value.size())
    {
      String tmp;
      value >> tmp;
      out += tmp;
    }
  }
  catch (String) {}
  
  return string::join(out, ' ');
}

int main(int argc, char ** argv)
{
  convert();
  
  return 0;
}

