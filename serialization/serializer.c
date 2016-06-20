#include "serializer.h"

void serializer_write_char(char* dest, int* pointer, char value)
{
  dest[(*pointer)] = value;
  (*pointer) += 1;
}

void serializer_write_short(char* dest, int* pointer, short value)
{
  dest[(*pointer)] = ((value >> 0) & 0xff);
  dest[(*pointer) + 1] = (value >> 8) & 0xff;
  (*pointer) += 2;
}

void serializer_write_int(char* dest, int* pointer, int value)
{
  dest[(*pointer)] = ((value >> 0) & 0xFF);
  dest[(*pointer) + 1] = ((value >> 8) & 0xFF);
  dest[(*pointer) + 2] = ((value >> 16) & 0xFF);
  dest[(*pointer) + 3] = ((value >> 24) & 0xFF);

  (*pointer) += 4;
}

void serializer_write_bool(char* dest, int* pointer, int bool_value)
{
  dest[(*pointer)] = (bool_value ? 1 : 0);
  (*pointer)++;
}

int serializer_write_to_file(char* bytesToWrite, int bufferSize, const char* filename)
{
  FILE* file = fopen(filename, "wb");
  if(file == NULL)
    return SERIALIZER_ERROR_FOPEN;
  fwrite(bytesToWrite, 1, bufferSize, file);
  fclose(file);

  return 0;
}

void serializer_write_string(char* dest, int* pointer, char* value)
{
  //write short with length of string
  short length = strlen(value);
  serializer_write_short(dest, pointer, length);
  short i;
  for(i = 0; i < length; i++)
  {
    //dest[(*pointer) + i] = value[i];
    serializer_write_char(dest, pointer, value[i]);
    (*pointer)++;
  }
  serializer_write_char(dest, pointer, '\0');
}
