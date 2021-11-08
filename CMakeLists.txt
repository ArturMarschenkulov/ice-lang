cmake_minimum_required(VERSION 3.13.4)

project(ice-lang) # "${PROJECT_NAME}"

set(CMAKE_CXX_STANDARD 20)

set(BUILD_ARCH "-m64")
set(TARGET_64 ON)

# import LLVM
set(LLVM_DIR "C:/DEV/llvm-project/build/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM: ${LLVM_DIR} (found version ${LLVM_PACKAGE_VERSION})")
message(STATUS "Extra: LLVM_DIR ${LLVM_DIR}")
message(STATUS "Extra: LLVM_DIR ${LLVM_INCLUDE_DIRS}")
message(STATUS "Extra: LLVM_DIR ${LLVM_DEFINITIONS}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
# message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
# message(STATUS "LLVM definitions dirs: ${LLVM_DEFINITIONS}")


# add the executable
add_executable(${PROJECT_NAME} 
    main.cpp
    ice_lang.h
    ice_lang.cpp
    tokenizer.h
    tokenizer.cpp
    token.h
    token.cpp
    parser.h
    parser.cpp
    expr.h
    ast_printer.h
)


# execute_process(COMMAND llvm-config --libs core support
#                 OUTPUT_VARIABLE llvm_libs)

llvm_map_components_to_libnames(llvm_libs
    #analysis
    core
    #executionEngine
    #instCombine
    #object
    #orcJIT
    #runtimeDyld
    #scalarOpts
    support
    #native
)

target_link_libraries(${PROJECT_NAME} ${llvm_libs})