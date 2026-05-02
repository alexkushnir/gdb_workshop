#include <stdio.h>
#include <stdlib.h>

int main(void) 
{
    setbuf(stdout, NULL);

    void *a = malloc(0x10);
    void *b = malloc(0x10);
    void *c = malloc(0x1000); // large chunk to prevent consolidation of a and b./
    
    
    void* arr[65];

    for (int i = 0; i < 65; i++) {
        arr[i] = malloc(0x20);
    }
    

    free(a);
    free(b);
    free(c);
  
    
    for (auto ptr : arr) 
    {
        free(ptr);
    }

    return 0;
}
