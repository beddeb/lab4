#include <memory>
#include <cxxabi.h>
#include "demangle.hpp"


std::string demangle(const char *mangled_name) {
    int status = 0;
    std::unique_ptr<char, void (*)(void *)> res(
            abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status),
            std::free
    );
    return (status == 0) ? res.get() : mangled_name;
}