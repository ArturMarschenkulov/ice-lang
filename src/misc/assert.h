#pragma once
#include <iostream>
#include <string>
#include <cassert>

#define ICE_ASSERT(expr, msg)                                                                                          \
    do {                                                                                                               \
        if (expr) {                                                                                                    \
        } else {                                                                                                       \
            /*std::string t = fmt::v8::format("{}: {}: {}", __FILE__,                                                  \
             * __LINE__, std::string(text));*/                                                                         \
                                                                                                                       \
            std::cout << msg << std::endl;                                                                             \
            (void)assert(false);                                                                                       \
        }                                                                                                              \
    } while (0)

#define ICE_UNIMPLEMENTED() ICE_ASSERT(false, "unimplemented")
#define ICE_UNREACHABLE()   ICE_ASSERT(false, "unreachable")
#define ICE_PANIC(text)     ICE_ASSERT(false, text)