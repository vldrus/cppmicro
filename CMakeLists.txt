# MIT License #

cmake_minimum_required(VERSION 3.16)

project(cppmicro
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(config.json config.json COPYONLY)

add_executable(cppmicro
        cppmicro.cpp)

target_link_libraries(cppmicro
        PRIVATE
        cpplog::cpplog
        httplib::httplib
        nlohmann_json::nlohmann_json)

include(FetchContent)

FetchContent_Declare(cpplog
        URL https://github.com/vldrus/cpplog/archive/refs/tags/v0.1.0.tar.gz
        URL_HASH SHA256=4dfdcd3ee3ab426660b852f6c1fedf2254701e2d5cabc7e0d648cb1954ae1331)
FetchContent_MakeAvailable(cpplog)

set(HTTPLIB_COMPILE ON)

FetchContent_Declare(httplib
        URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.11.1.tar.gz
        URL_HASH SHA256=1ce2f0393ba779ec34885c5cd937141b4b5b730e2bc2efc34eb8554289c24d61)
FetchContent_MakeAvailable(httplib)

FetchContent_Declare(nlohmann_json
        URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.tar.gz
        URL_HASH SHA256=d69f9deb6a75e2580465c6c4c5111b89c4dc2fa94e3a85fcd2ffcd9a143d9273)
FetchContent_MakeAvailable(nlohmann_json)
