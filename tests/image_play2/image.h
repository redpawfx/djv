#ifndef TEST_IMAGE_PLAY2_IMAGE_H
#define TEST_IMAGE_PLAY2_IMAGE_H

#include <djv_image/data.h>
#include <djv_image/gl.h>
#include <djv_base/file_io.h>

using namespace djv_base;

class Image
{
public:

  Image();
  
  ~Image();

  virtual void load(const String &) throw (Error) = 0;
  
  const djv_image::Info & info() const;
  
  virtual void bind() const throw (Error) = 0;
  
protected:

  void del();

  djv_image::Info _info;
  GLuint _id;
};

class Image_Ppm : public Image
{
public:

  Image_Ppm();

  void load(const String &) throw (Error);

  void bind() const throw (Error);
  
private:

  File_Io _io;
  bool _init;
  bool _load;
};

#endif

