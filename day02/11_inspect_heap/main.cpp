#include <print>
#include <scn/scan.h>
#include <unistd.h>
#include <vector>

int main()
{
    std::println("Heap Inspection Demo");
    std::println("====================");
    std::println("PID: {}", getpid());

    // Perform some heap allocations of various sizes
    std::println("\n[Program] Allocating memory on the heap...");

    void* small = malloc(0x20);        // 32 bytes
    void* medium = malloc(0x100);      // 256 bytes
    void* large = malloc(0x1000);      // 4096 bytes (usually a page)
    void* veryLarge = malloc(0x21000); // 132KB (should trigger mmap if over threshold)

    std::println("[Program] Allocated addresses:");
    std::println("  Small (32B):      {:p}", small);
    std::println("  Medium (256B):    {:p}", medium);
    std::println("  Large (4KB):      {:p}", large);
    std::println("  Very Large (132K):{:p}", veryLarge);

    std::println("\n[Program] Now you can attach GDB and inspect the heap.");
    std::println("[Program] Commands to try:");
    std::println("  (gdb) info proc mappings");
    std::println("  (gdb) p main_arena");

    std::println("\nPress Enter to free memory and exit...");
    auto result = scn::prompt<std::string>("", "{}");

    free(small);
    free(medium);
    free(large);
    free(veryLarge);

    std::println("[Program] Memory freed. Exiting.");

    return 0;
}
