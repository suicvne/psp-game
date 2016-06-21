#include "serialization_reader.h"

int serializer_get_file_size(const char* filename)
{
  struct stat st;
  stat(filename, &st);
  return st.st_size;
}

int serializer_read_from_file(char *readInto, int bufferSize, const char* filename)
{
  FILE* file = fopen(filename, "rb");
  char* buffer;
  int fileLength;

  fseek(file, 0, SEEK_END);
  fileLength = ftell(file);
  printf("file is %d bytes\n", fileLength);
  rewind(file);

  buffer = (char*)malloc(sizeof(char) * fileLength);
  fread(buffer, fileLength, 1, file);

  memcpy(readInto, buffer, bufferSize);

  fclose(file);
  return 0;
}

char serializer_read_char(char* src, int* pointer)
{
  char value = src[*pointer];
  (*pointer) += 1;
  return value;
}

char serializer_read_wchar(char* src, int* pointer)
{
  char value = src[*pointer];
  (*pointer) += 2;
  return value;
}

short serializer_read_short(char* src, int* pointer)
{
  short temp;
  temp = (src[(*pointer)] << 0)
        | (src[(*pointer) + 1] << 8);
  *pointer += 2;
  return temp;
}

int serializer_read_bool(char* src, int* pointer)
{
  unsigned char value = src[*pointer];
  (*pointer)++;
  return value == 1 ? 1 : 0;
}

int serializer_read_int(char* src, int* pointer)
{
  int temp;
  temp = (
    src[(*pointer)] << 0) |
    (src[(*pointer) + 1] << 8) |
    (src[(*pointer) + 2] << 16) |
    (src[(*pointer) + 3] << 24
  );
  *pointer += 4;
  //FUCK THIS IM OUT
  //i went through H E L L to get this to work
  return temp;
}

char* serializer_read_string(char* src, int* pointer)
{
  short string_length = serializer_read_short(src, pointer);
  printf("length of string: %d\n", string_length);
  if(string_length > 0)
  {
    char* return_value = malloc(sizeof(char) * string_length);
    memset(return_value, 0, string_length);

    short i;
    for(i = 0; i < string_length; i++)
    {
      char read_char = serializer_read_wchar(src, pointer);

      return_value[i] = read_char;
      if(i == (string_length - 1)) //EOS
        return_value[string_length] = '\0'; //C needs this. this is my first C project tbh
    }
    (*pointer)++;
    return return_value;
  }
  else
    return NULL;
}
