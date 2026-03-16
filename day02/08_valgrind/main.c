#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void memory_leak() {
    // printf("--- Triggering Memory Leak ---\n");
    int *ptr = (int *)malloc(10 * sizeof(int));
    // ptr is never freed
}

void uninitialized_read() {
    // printf("--- Triggering Uninitialized Read ---\n");
    int a[10];
    if (a[5] == 42) { // 'a' is on the stack and uninitialized
        printf("Unexpected value!\n");
    }
}

void heap_out_of_bounds() {
    // printf("--- Triggering Heap Out-of-Bounds ---\n");
    char *ptr = (char *)malloc(10);
    ptr[10] = 'X'; // Writing 1 byte past the end
    free(ptr);
}

void invalid_free() {
    // printf("--- Triggering Invalid Free ---\n");
    int *ptr = (int *)malloc(sizeof(int));
    free(ptr);
    free(ptr); // Double free
}

int main(int argc, char **argv) {
    // printf("Valgrind Demo\n");
    // printf("======================\n\n");

    if (argc < 2) {
        printf("Usage: %s <leak|uninit|bounds|free|all>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "leak") == 0 || strcmp(argv[1], "all") == 0) {
        memory_leak();
    }
    if (strcmp(argv[1], "uninit") == 0 || strcmp(argv[1], "all") == 0) {
        uninitialized_read();
    }
    if (strcmp(argv[1], "bounds") == 0 || strcmp(argv[1], "all") == 0) {
        heap_out_of_bounds();
    }
    if (strcmp(argv[1], "free") == 0 || strcmp(argv[1], "all") == 0) {
        invalid_free();
    }

    // printf("\nDemo finished.\n");
    return 0;
}
