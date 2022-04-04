cmake_minimum_required(VERSION 3.13.4)

# project(ice-lang) # "${PROJECT_NAME}"
project(
    ice-lang
    VERSION 0.0.1
    DESCRIPTION "Ice-Lang is a programming language."
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

set(BUILD_ARCH "-m64")
set(TARGET_64 ON)

add_subdirectory(src)