


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
    ast.h
    ast_printer.h
)

llvm_map_components_to_libnames(llvm_libs
    core
    support
)

target_link_libraries(${PROJECT_NAME} ${llvm_libs})