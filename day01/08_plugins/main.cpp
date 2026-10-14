// main.cpp — Plugin demo: executable exports a callback that the .so calls back.
//
// Demonstrates:
//  - Building a shared library (plugin) that references a symbol in the host
//    executable (requires ENABLE_EXPORTS / -rdynamic).
//  - Loading the plugin at runtime with dlopen / dlsym.
//  - GDB can set breakpoints both in the executable and inside the .so.

#include <dlfcn.h>

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

// The plugin calls this function; it must be visible outside the executable.
// 'extern "C"' prevents name-mangling so dlsym can also find it if needed.
extern "C" void HostCallback(std::string_view message)
{
    std::cout << "[host] HostCallback called with: \"" << message << "\"\n";
}

// The plugin exports exactly one entry-point with this signature.
using PluginRunFn = void (*)();

class DlHandle
{
public:
    explicit DlHandle(const std::filesystem::path& path, int flags = RTLD_NOW | RTLD_GLOBAL)
        : m_handle{::dlopen(path.c_str(), flags)}
    {
        if (!m_handle)
        {
            throw std::runtime_error(std::string{"dlopen failed: "} + ::dlerror());
        }
        std::cout << "[host] Loaded plugin: " << path.string() << "\n";
    }

    ~DlHandle()
    {
        if (m_handle)
        {
            ::dlclose(m_handle);
        }
    }

    DlHandle(const DlHandle&) = delete;
    DlHandle& operator=(const DlHandle&) = delete;
    DlHandle(DlHandle&& other) noexcept : m_handle{other.m_handle}
    {
        other.m_handle = nullptr;
    }
    DlHandle& operator=(DlHandle&& other) noexcept
    {
        if (this != &other)
        {
            if (m_handle)
            {
                ::dlclose(m_handle);
            }

            m_handle = other.m_handle;
            other.m_handle = nullptr;
        }

        return *this;
    }

    template <typename FnPtr>
    [[nodiscard]] FnPtr Resolve(const char* symbol) const
    {
        ::dlerror(); // clear previous errors
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        auto* fn = reinterpret_cast<FnPtr>(::dlsym(m_handle, symbol));
        if (const char* err = ::dlerror(); err != nullptr)
        {
            throw std::runtime_error(std::string{"dlsym("} + symbol + ") failed: " + err);
        }
        return fn;
    }

private:
    void* m_handle{nullptr};
};

int main(int argc, char* argv[])
{
    std::filesystem::path pluginPath =
        std::filesystem::path{argv[0]}.parent_path() / "libplugin.so";

    if (argc >= 2)
    {
        pluginPath = argv[1];
    }

    std::cout << "[host] Plugin demo starting\n";
    std::cout << "[host] Loading plugin from: " << pluginPath.string() << "\n";

    try
    {
        DlHandle plugin{pluginPath};

        auto pluginRun = plugin.Resolve<PluginRunFn>("PluginRun");

        std::cout << "[host] Calling PluginRun()...\n";
        pluginRun();
        std::cout << "[host] PluginRun() returned\n";
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[host] ERROR: " << ex.what() << "\n";
        return 1;
    }

    std::cout << "[host] Done.\n";
    return 0;
}
