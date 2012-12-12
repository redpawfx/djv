#define DJV_DEBUG

#include <djv_base/file.h>

using namespace djv_base;

void split()
{
  DJV_DEBUG("split");

  const struct Data { String in, path, base, number, extension; } data [] =
  {
    { "/tmp/image.1-100.tiff", "/tmp/", "image.", "1-100", ".tiff" },
    { "image.1-100.tiff", "", "image.", "1-100", ".tiff" },
    { "image.001-100.tiff", "", "image.", "001-100", ".tiff" },
    { "image1-100.tiff", "", "image", "1-100", ".tiff" },
    { "image001-100.tiff", "", "image", "001-100", ".tiff" },
    { "image_01-100.tiff", "", "image_01-", "100", ".tiff" },
    { "1-100.tiff", "", "", "1-100", ".tiff" },
    { "-1.tiff", "", "", "-1", ".tiff" },
    { "1-100", "", "", "1-100", "" },
    { "1", "", "", "1", "" },
    { ".hidden", "", ".hidden", "", "" },
    { "/", "/", "", "", "" },
    { "//", "//", "", "", "" },
    { "/.", "/", "", "", "." },
    { "./", "./", "", "", "" },
    { "C:\\Documents and Settings\\darby\\Desktop\\movie.mov",
        "C:\\Documents and Settings\\darby\\Desktop\\", "movie", "", ".mov" }
  };
  const int data_size = sizeof(data) / sizeof(Data);
  
  for (int i = 0; i < data_size; ++i)
  {
    const File tmp(data[i].in);
    DJV_DEBUG_PRINT(data[i].in << " = " <<
      string::Format("'%%' '%%' '%%' '%%'").
        arg(tmp.path()).
        arg(tmp.base()).
        arg(tmp.number()).
        arg(tmp.extension())
    );
    DJV_ASSERT(
      tmp.path() == data[i].path &&
      tmp.base() == data[i].base &&
      tmp.number() == data[i].number &&
      tmp.extension() == data[i].extension
    );
  }
}

int main(int argc, char ** argv)
{
  split();

  return 0;
}

