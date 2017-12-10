
#ifndef NEW_h
#define NEW_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
}

void* operator new[](size_t size){
    return malloc(size);
 
}
void operator delete[](void * ptr){
  free(ptr);

}

#endif