#ifndef ___CUSTOM_STRING_H____
#define ___CUSTOM_STRING_H____

#include <string.h>

static inline char* string_append(const char* a, const char* b)
{
    int size_a = strlen(a);
    int size_b = strlen(b);

    int combined_length = sizeof(char) * (size_a + size_b + 1);
    char* combined = malloc(combined_length); //allocate enough memory for the string + 1 for null
    memset(combined, 0, combined_length);
    strcat(combined, a);
    strcat(combined, b);

    return combined;
};

#endif //___CUSTOM_STRING_H____