#define DJV_DEBUG

#include <djv_image/pixel.h>

using namespace djv_base;
using namespace djv_image;

void convert()
{
  DJV_DEBUG("convert");
  
  DJV_ASSERT(0 == pixel::u8_to_u10(0));
  DJV_ASSERT(1023 == pixel::u8_to_u10(255));
  
  DJV_ASSERT(0 == pixel::u10_to_u8(0));
  DJV_ASSERT(255 == pixel::u10_to_u8(1023));
}

int main(int argc, char ** argv)
{
  convert();
  
  return 0;
}

