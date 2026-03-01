// plugin.cpp — Shared library that calls back into the host executable.
//
// The only exported symbol is PluginRun().  It references HostCallback()
// which is defined in the host executable (linked with -rdynamic /
// ENABLE_EXPORTS so its symbols are exported into the dynamic symbol table).

#include <print>
#include <string_view>

// Declared here so the linker knows the signature; the definition lives in
// main.cpp and is resolved at load-time because the executable was built with
// ENABLE_EXPORTS (CMake) / -rdynamic (GCC/Clang).
extern "C" void HostCallback(std::string_view message);

extern "C" void PluginRun()
{
    std::println("[plugin] PluginRun() entered");

    // Call the function that lives in the host executable.
    HostCallback("hello from the plugin!");

    std::println("[plugin] PluginRun() calling HostCallback a second time");
    HostCallback("plugin is done");

    std::println("[plugin] PluginRun() exiting");
}
