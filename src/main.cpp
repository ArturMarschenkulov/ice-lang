
#include "ice_lang.h"

#include <vector>
#include <string>


int main(int argc, char* argv[]) {

    std::vector<std::string> cl_args;
    cl_args.resize(argc);
    for(int i = 0; i < cl_args.size(); i++) {
        cl_args[i] = argv[i];
    }

    IceContext ice_context;
    ice_context.main(cl_args);

    return 0;
}
