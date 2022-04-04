#pragma once
#include <vector>
#include <string>


class IceContext {
    struct State {
        bool print_token_stream = false;
        bool print_ast = false;
        bool file_needed = true;
    };

public:
    auto main(const std::vector<std::string>& cl_args) -> bool;

    State state;
};