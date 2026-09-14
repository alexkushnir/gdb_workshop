//
// g++ -std=c++20 -g -O0 -fno-omit-frame-pointer vtble_demo.cpp -o vtble_demo
//
//
// 
#include <iostream>
#include <string>

struct Widget {
	Widget(int id, double      weight, std::string name, char        tag) : id(id), weight(weight), name(name), tag(tag) {}
    int         id;      // 4 bytes  + 4 padding
    double      weight;  // 8 bytes
    std::string name;    // 32 bytes (libstdc++ C++11 ABI)
    char        tag;     // 1 byte   + 7 padding
    virtual ~Widget() {}
    virtual void foo() const { std::cout << "I'm basic!" << std::endl; }   
};

struct WiWi : Widget {
	~WiWi() override {}
	using Widget::Widget;
	void foo() const override { std::cout << "I'm more advaned!" << std::endl; } 
};

int main() {
    Widget w1{7, 1.5, "a name long enough to force a heap allocation", 'X'};
    WiWi w2{4, 2.2, "I'm ok!", 'Y'};
    
   // Try info vtble w2 
    
    
    
    return 0;
}
