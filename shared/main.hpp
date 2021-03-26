#pragma once

#include <dlfcn.h>
#include <optional>
#include <string_view>
#include "utils.hpp"

namespace CondDep {

    #ifndef EXPOSE_API
    // Note: Cannot be used to return or pass pure C++ types (ex: reference types)
    // Please use pointers/C types instead.
    #define EXPOSE_API(name, retVal, ...) extern "C" retVal  __ ##name(__VA_ARGS__)
    #endif

    template<class R, class... TArgs>
    std::optional<R (*)(TArgs...)> Find(std::string_view id, std::string_view name) {
        std::string partial("lib" + std::string(id) + ".so");
        auto searchPath = CondDeps::__Internal::cond_getPath() + partial;
        if (!CondDeps::__Internal::cond_fileexists(searchPath)) {
            return std::nullopt;
        }
        // Clear dlerror first
        dlerror();
        auto* handle = dlopen(searchPath.c_str(), RTLD_LOCAL | RTLD_NOW);
        auto* err = dlerror();
        if (handle == nullptr || err != nullptr) {
            return std::nullopt;
        }
        auto* sym = dlsym(handle, (std::string("__") + name.data()).c_str());
        err = dlerror();
        if (sym == nullptr || err != nullptr) {
            return std::nullopt;
        }
        return reinterpret_cast<R (*)(TArgs...)>(sym);
    }

}