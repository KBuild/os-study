#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "types.h"

void kmemset(void* dest, BYTE data, int size);
int kmemcpy(void* dest, const void* src, int size);
int kmemcmp(const void* dest, const void* src, int size);

#endif
