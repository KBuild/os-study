#include "utility.h"
#include "asmutility.h"

void kmemset(void* dest, BYTE data, int size)
{
    int i;

    for(i = 0 ; i < size ; i++)
    {
        ((char*) dest)[i] = data;
    }
}

int kmemcpy(void* dest, const void* src, int size)
{
    int i;

    for(i = 0 ; i < size ; i++)
    {
        ((char*) dest)[i] = ((char*) src)[i];
    }

    return size;
}

int kmemcmp(const void* dest, const void* src, int size)
{
    int i;
    char temp;

    for(i = 0 ; i < size ; i++)
    {
        temp = ((char*) dest)[i] - ((char*) src)[i];
        if(temp != 0)
        {
            return (int)temp;
        }
    }

    return 0;
}
