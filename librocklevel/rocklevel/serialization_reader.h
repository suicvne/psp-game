#ifdef __cplusplus
extern "C" {
#endif

#ifndef ___SERIALIZATION_READER_H___
#define ___SERIALIZATION_READER_H___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

extern int serializer_get_file_size(const char* filename);
extern int serializer_read_from_file(char* readInto, int bufferSize, const char* filename);

extern char serializer_read_char(char* src, int* pointer);
extern char serializer_read_wchar(char* src, int* pointer);
extern short serializer_read_short(char* src, int* pointer);
extern int serializer_read_bool(char* src, int* pointer);
extern int serializer_read_int(char* src, int* pointer);
extern char* serializer_read_string(char* src, int* pointer);

#endif //___SERIALIZATION_READER_H___

#ifdef __cplusplus
}
#endif
