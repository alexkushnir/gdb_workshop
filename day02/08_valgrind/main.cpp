#include <iostream>
#include <vector>

// (1) A simple leak: never freed ⇒ "definitely lost"
void definitely_lost_example() {
    int* raw = new int(42);
    // never delete raw
    std::cout << "definitely_lost_example: " << *raw << "\n";
}

// (2) A linked‑style structure: head leaked ⇒ "indirectly lost" nodes
struct ListNode {
    int data;
    ListNode* next;
    ListNode(int d) : data(d), next(nullptr) {}
};

void indirectly_lost_example() {
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);    // indirectly lost if head is leaked
    head->next->next = new ListNode(3);

    // head is never freed, so:
    // head → definitely lost
    // the two following nodes → indirectly lost
    std::cout << "indirectly_lost_example: " << head->data << "\n";
}

// (3) A pointer to interior of a block (tricky, but Memcheck can say "possibly lost")
// In practice this often shows up when you only store a pointer inside a block, not the start.
// We’ll fake this by holding a pointer‑to‑member, not the heap start, and then “leak” that.
struct BigBlock {
    char padding[64];
    int useful_data; // pretend this is some useful data
    char padding2[64];
};

// (4) Global pointer: not freed at exit, but still reachable ⇒ "still reachable"
// static std::vector<int*> interior_ptrs;
int* global = nullptr;

// Capture a pointer to interior of heap block; if you lose pointer to start Memcheck may call it "possibly lost"
void possibly_lost_example() {
    BigBlock* blk = new BigBlock();
    global = &(blk->useful_data); // Store interior pointer in a global variable
    // In real code, if you somehow only store interior_ptr and lose blk, Memcheck may classify it as "possibly lost"
    // Here we just leak blk itself; Valgrind may still show it as "definitely lost" or "possibly lost" depending on tool heuristics.
    // The intent is to show a pattern that can trigger "possibly lost".

    // DO NOT delete blk to simulate leak
   // std::cout << "possibly_lost_example: " << *interior_ptr << "\n";
}

// (4) Global pointer: not freed at exit, but still reachable ⇒ "still reachable"
int* global_leak = nullptr;

void still_reachable_example() {
    global_leak = new int[55];
    // This leak is *not* freed, and no pointer to it is stored globally, so it will be "definitely lost"
    std::cout << "global_leak_example: " << *global_leak << "\n";
}

// (5) Suppressed leaks (Valgrind decides via suppression rules, not code).
// This function is just a dummy site; you would need a suppression file for its leak.
// Valgrind will show "suppressed" only if you have an actual suppression rule.
void suppressed_leak_example() {
    int* suppr = new int(999);
    // This leak is *not* freed.
    // To see it as "suppressed", you must add a suppression rule in a .supp file
    // and run valgrind --suppressions=your.supp ...
    std::cout << "suppressed_leak_example: " << *suppr << "\n";
}

void call_suppressed_leak_example() {
    suppressed_leak_example();
}

template<typename T, typename M>
T* object_from_member_ptr(M* member_ptr, M T::*member) {
    const std::ptrdiff_t offset = (char*)&(static_cast<T*>(nullptr)->*member) - (char*)nullptr;
    return reinterpret_cast<T*>(reinterpret_cast<char*>(member_ptr) - offset);
}

int main() {
    std::cout << "=== Creating different leak scenarios ===\n";

    definitely_lost_example();
    indirectly_lost_example();
    possibly_lost_example();
    still_reachable_example();
    suppressed_leak_example();
    call_suppressed_leak_example();

    // No explicit delete for any of the above ⇒ leaks

    // Valgrind will report:
    //   definitely lost    : raw, head, blk, suppr (if not suppressed)
    //   indirectly lost    : ListNode 2 & 3 (if they are reachable only via head)
    //   possibly lost      : possibly interior pointer pattern (if tool heuristics trigger it)
    //   still reachable    : *still_reachable_ptr (because global pointer still points to it)
    //   suppressed         : if a suppression rule covers the leak site

    // BigBlock* obj_ptr = object_from_member_ptr(global, &BigBlock::useful_data);
    // delete obj_ptr; // Clean up the block to avoid actual leak in this demo
    std::cout << "=== Program finished, leaks will be reported by Valgrind ===\n";
    return 0;
}

