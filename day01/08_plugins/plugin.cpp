// plugin.cpp — Shared library that calls back into the host executable.
//
// The only exported symbol is PluginRun().  It references HostCallback()
// which is defined in the host executable (linked with -rdynamic /
// ENABLE_EXPORTS so its symbols are exported into the dynamic symbol table).

#include <iostream>
#include <string_view>

// Declared here so the linker knows the signature; the definition lives in
// main.cpp and is resolved at load-time because the executable was built with
// ENABLE_EXPORTS (CMake) / -rdynamic (GCC/Clang).
extern "C" void HostCallback(std::string_view message);

extern "C" void PluginRun()
{
    std::cout << "[plugin] PluginRun() entered\n";

    // Call the function that lives in the host executable.
    HostCallback("hello from the plugin!");

    std::cout << "[plugin] PluginRun() calling HostCallback a second time\n";
    HostCallback("plugin is done");

    std::cout << "[plugin] PluginRun() exiting\n";
}
