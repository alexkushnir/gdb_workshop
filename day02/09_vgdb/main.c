#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void trigger_bug() {
    printf("[Program] Triggering a heap overflow...\n");
    char *ptr = malloc(10);
    // Writing significantly past the end to ensure detection
    for (int i = 0; i < 20; i++) {
        ptr[i] = 'A';
    }
    printf("[Program] Bug triggered. Now freeing (this might crash or be caught by Valgrind).\n");
    free(ptr);
}

int main() {
    printf("Valgrind + GDB (vgdb) Demo\n");
    printf("==========================\n");
    printf("PID: %d\n", getpid());
    printf("\n[Program] I will wait for 10 seconds to allow you to connect GDB.\n");
    printf("[Program] Alternatively, Valgrind will stop at the first detected error.\n");
    
    for (int i = 10; i > 0; i--) {
        printf("[Program] Waiting... %d\r", i);
        fflush(stdout);
        sleep(1);
    }
    printf("\n");

    trigger_bug();

    printf("[Program] Demo finished.\n");
    return 0;
}
