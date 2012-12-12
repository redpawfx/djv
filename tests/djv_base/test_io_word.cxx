//#define DJV_DEBUG

#include <djv_base/file_io.h>
#include <stdio.h>

using namespace djv_base;

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    printf("usage: test_io_word (input)\n");
    return 1;
  }
  
  try
  {
    File_Io io;
    io.open(argv[1], file_io::READ);
    
    while (io.is_valid())
    {
      char buf [string::cstring_size];
      
      try
      {
        file_io::word(io, buf, string::cstring_size);
      }
      catch (Error)
      {}
      
      printf("%s\n", buf);
    }
  }
  catch (Error in)
  {
    djv_base::error::print(in);
  }
  
  return 0;
}

