#pragma once
#include <vector>
#include <string>


class IceContext {
public:
    auto main(const std::vector<std::string>& cl_args) -> bool;
};