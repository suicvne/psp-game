#ifdef __cplusplus
extern "C" {
#endif

#ifndef ___SERIALIZER_H___
#define ___SERIALIZER_H___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  SERIALIZER_ERROR_FOPEN = -1
} SERIALIZER_ERROR_TYPE;

extern void serializer_write_char(char* dest, int* pointer, char value);
extern void serializer_write_wchar(char* dest, int* pointer, char value);
extern void serializer_write_short(char* dest, int* pointer, short value);
extern void serializer_write_int(char* dest, int* pointer, int value);
//void serializer_write_float(char* dest, int* pointer, float value);
extern void serializer_write_bool(char* dest, int* pointer, int bool_value);
extern void serializer_write_string(char* dest, int* pointer, char* value);

extern int serializer_write_to_file(char* bytesToWrite, int bufferSize, const char* filename);

#endif //___SERIALIZER_H___

#ifdef __cplusplus
}
#endif
