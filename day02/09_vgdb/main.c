#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void TriggerBug()
{
    printf("[Program] Triggering a heap overflow...\n");
    char* ptr = malloc(10);
    // Writing significantly past the end to ensure detection
    for (int i = 0; i < 20; i++)
    {
        ptr[i] = 'A';
    }
    printf("[Program] Bug triggered. Now freeing (this might crash or be caught by Valgrind).\n");
    free(ptr);
}

int main()
{
    printf("Valgrind + GDB (vgdb) Demo\n");
    printf("==========================\n");
    printf("PID: %d\n", getpid());
    
    TriggerBug();

    printf("[Program] Demo finished.\n");
    return 0;
}
