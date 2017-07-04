#ifndef ___SERIALIZATION_READER_H___
#define ___SERIALIZATION_READER_H___

int serializer_get_file_size(const char* filename);
int serializer_read_from_file(char* readInto, int bufferSize, const char* filename);

char serializer_read_char(char* src, int* pointer);
char serializer_read_wchar(char* src, int* pointer);
short serializer_read_short(char* src, int* pointer);
int serializer_read_bool(char* src, int* pointer);
int serializer_read_int(char* src, int* pointer);
char* serializer_read_string(char* src, int* pointer);

#endif //___SERIALIZATION_READER_H___
