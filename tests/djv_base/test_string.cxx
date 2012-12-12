#define DJV_DEBUG

#include <djv_base/debug.h>
#include <djv_base/string.h>

using namespace djv_base;

void format()
{
  DJV_DEBUG("format");
  
  const struct Data { String a, b; } data [] =
  {
    { "%%%%%%", "abc" },
    { "%%%%", "ab" },
    { "%%%%%%%%", "abc%%" }
  };
  const int data_size = sizeof(data) / sizeof(Data);
  
  for (int i = 0; i < data_size; ++i)
  {
    const String tmp = string::Format(data[i].a).arg("a").arg("b").arg("c");
    DJV_DEBUG_PRINT(tmp << " = " << data[i].b);
    DJV_ASSERT(tmp == data[i].b);
  }
}

void format2()
{
  DJV_DEBUG("format2");
  
  const String tmp =
    " a b c\n"
    " d e f\n"
    "\n"
    " g h i";
  
  //DJV_DEBUG_PRINT(string::split(tmp, '\n', true));
  
  DJV_DEBUG_PRINT(string::join(string::lines(tmp, 80), '|'));
}

void char_case()
{
  DJV_DEBUG("char_case");
  
  DJV_ASSERT("abc" == string::lower("ABC"));
  DJV_ASSERT("ABC" == string::upper("abc"));
}

void join()
{
  DJV_DEBUG("join");
  
  const String tmp = string::join(List<String>() << "a" << "b" << "c", " ");
  DJV_DEBUG_PRINT(tmp);
  DJV_ASSERT("a b c" == tmp);
}

void split()
{
  DJV_DEBUG("split");

  const struct Data { String a; String split; bool empty; List<String> b; } data [] =
  {
    { "", "_", false, List<String>() << "" },
    { "_", "_", false, List<String>() },
    { "_", "_", true, List<String>() << "" },
    { "__", "_", false, List<String>() },
    { "__", "_", true, List<String>() << "" << "" },
    { "a", "_", false, List<String>() << "a" },
    { "abc", "_", false, List<String>() << "abc" },
    { "a_b", "_", false, List<String>() << "a" << "b" },
    { "a_b_c", "_", false, List<String>() << "a" << "b" << "c" },
    { "_a_b_c", "_", false, List<String>() << "a" << "b" << "c" },
    { "a_b_c_", "_", false, List<String>() << "a" << "b" << "c" },
    { "__a__b__c", "__", false, List<String>() << "a" << "b" << "c" },
    { "a__b__c__", "__", false, List<String>() << "a" << "b" << "c" },
    { "a_bc_def", "_", false, List<String>() << "a" << "bc" << "def" },
    { "a__b__c", "_", true, List<String>() << "a" << "" << "b" << "" << "c" }
  };
  const int data_size = sizeof(data) / sizeof(Data);
  
  for (int i = 0; i < data_size; ++i)
  {
    const List<String> tmp = string::split(data[i].a, data[i].split, data[i].empty);
    DJV_DEBUG_PRINT(data[i].a << " = " << tmp);
    DJV_ASSERT(data[i].b == tmp);
  }
}

void replace()
{
  DJV_DEBUG("replace");
  
  const String data = "a b c";
  const String tmp = string::replace(data, ' ', '_');
  DJV_DEBUG_PRINT(data << " = " << tmp);
  DJV_ASSERT("a_b_c" == tmp);
}

void convert()
{
  DJV_DEBUG("convert");

  const struct Int_Data { String a; int b; String c; } int_data [] =
  {
    { "-100", -100, "-100" },
    { "-10", -10, "-10" },
    { "-1", -1, "-1" },
    { "0", 0, "0" },
    { ".1", 0, "0" },
    { "1", 1, "1" },
    { "1.1", 1, "1" },
    { "10", 10, "10" },
    { "100", 100, "100" },
    { "1000", 1000, "1000" }
  };
  const int int_size = sizeof(int_data) / sizeof(Int_Data);
  
  for (int i = 0; i < int_size; ++i)
  {
    const int tmp = string::string_to_int(int_data[i].a);
    DJV_DEBUG_PRINT("int: " << int_data[i].a << " = " << tmp);
    DJV_ASSERT(int_data[i].b == tmp);
    DJV_ASSERT(int_data[i].c == string::int_to_string(tmp));
  }

  const double float_data [] = { -1.0, 0.0, 1.0, 10.0, 100.0, 1000.0 };
  const int float_size = sizeof(float_data) / sizeof(double);
  
  for (int i = 0; i < float_size; ++i)
  {
    const double tmp = string::string_to_float(string::float_to_string(float_data[i]));
    DJV_DEBUG_PRINT("float: " << float_data[i] << " = " << tmp);
    DJV_ASSERT(float_data[i] == tmp);
  }

  const String string_data [] = { "abc", "a b c" };
  const int string_size = sizeof(string_data) / sizeof(String);
  
  for (int i = 0; i < string_size; ++i)
  {
    String tmp = string::serialize(string_data[i]);
    String tmp2;
    tmp >> tmp2;
    DJV_DEBUG_PRINT("str: " << string_data[i] << " = " << tmp2);
    DJV_ASSERT(string_data[i] == tmp2);
  }
}

int main(int argc, char ** argv)
{
  format();
  format2();
  char_case();
  join();
  split();
  replace();
  convert();

  return 0;
}

