#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "modloader/shared/modloader.hpp"

namespace CondDeps {

    inline bool fileexists(std::string_view filename) {
        return access(filename.data(), W_OK | R_OK) != -1;
    }

    template<typename... TArgs>
    std::string cond_string_format(const std::string_view format, TArgs ... args)
    {
    #pragma GCC diagnostic ignored "-Wformat-security"
    #pragma GCC diagnostic push
        size_t size = snprintf(nullptr, 0, format.data(), args ...) + 1; // Extra space for '\0'
        if (size <= 0)
            return "";
        std::unique_ptr<char[]> buf(new char[size]); 
        snprintf(buf.get(), size, format.data(), args...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    #pragma GCC diagnostic pop
    }
    
    #ifndef MOD_TEMP_PATH_FMT
    // Must always end with a /
    #define MOD_TEMP_PATH_FMT "/data/data/%s/"
    #endif

    inline std::string getPath() {
        static auto path = cond_string_format(MOD_TEMP_PATH_FMT, Modloader::getApplicationId().c_str());
        return path;
    }
}