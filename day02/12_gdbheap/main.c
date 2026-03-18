#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CHUNKS 100

void* smallChunks[MAX_CHUNKS];
int smallCount = 0;
void* mediumChunks[MAX_CHUNKS];
int mediumCount = 0;
void* largeChunks[MAX_CHUNKS];
int largeCount = 0;

void AllocateSmall()
{
    if (smallCount >= MAX_CHUNKS)
        return;
    void* ptr = malloc(32);
    smallChunks[smallCount++] = ptr;
    printf("Allocated 32 bytes at %p\n", ptr);
}

void FreeSmall()
{
    if (smallCount <= 0)
        return;
    void* ptr = smallChunks[--smallCount];
    free(ptr);
    printf("Freed 32 bytes at %p\n", ptr);
}

void AllocateMedium()
{
    if (mediumCount >= MAX_CHUNKS)
        return;
    void* ptr = malloc(512);
    mediumChunks[mediumCount++] = ptr;
    printf("Allocated 512 bytes at %p\n", ptr);
}

void FreeMedium()
{
    if (mediumCount <= 0)
        return;
    void* ptr = mediumChunks[--mediumCount];
    free(ptr);
    printf("Freed 512 bytes at %p\n", ptr);
}

void AllocateLarge()
{
    if (largeCount >= MAX_CHUNKS)
        return;
    void* ptr = malloc(2048);
    largeChunks[largeCount++] = ptr;
    printf("Allocated 2048 bytes at %p\n", ptr);
}

void FreeLarge()
{
    if (largeCount <= 0)
        return;
    void* ptr = largeChunks[--largeCount];
    free(ptr);
    printf("Freed 2048 bytes at %p\n", ptr);
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    int choice;
    int running = 1;

    while (running)
    {
        printf("\n--- gdb-heap Demo (PID: %d) ---\n", getpid());
        printf("1. Allocate Small (32B)\n");
        printf("2. Free Small\n");
        printf("3. Allocate Medium (512B)\n");
        printf("4. Free Medium\n");
        printf("5. Allocate Large (2048B)\n");
        printf("6. Free Large\n");
        printf("0. Exit\n");
        printf("Select an option: ");

        if (scanf("%d", &choice) != 1)
        {
            // Clear input buffer
            while (getchar() != '\n')
                ;
            printf("Invalid input.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                AllocateSmall();
                break;
            case 2:
                FreeSmall();
                break;
            case 3:
                AllocateMedium();
                break;
            case 4:
                FreeMedium();
                break;
            case 5:
                AllocateLarge();
                break;
            case 6:
                FreeLarge();
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("Unknown choice.\n");
                break;
        }
    }

    // Cleanup
    for (int i = 0; i < smallCount; i++)
        free(smallChunks[i]);
    for (int i = 0; i < mediumCount; i++)
        free(mediumChunks[i]);
    for (int i = 0; i < largeCount; i++)
        free(largeChunks[i]);

    return 0;
}
