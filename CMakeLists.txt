cmake_minimum_required(VERSION 3.12)

project("base64" LANGUAGES CXX VERSION 3.0.0
        DESCRIPTION "C++ Base64 encoding and decoding"
        HOMEPAGE_URL "https://github.com/heifner/base64")
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE .)

option(ENABLE_TESTING OFF)
if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory("tests")
endif()
