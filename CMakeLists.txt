cmake_minimum_required(VERSION 3.16)

project(cppmicro
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cppmicro
    main.cpp)

target_link_libraries(cppmicro
    PRIVATE
        cpplog::cpplog
        httplib::httplib
        nlohmann_json::nlohmann_json)

include(FetchContent)

FetchContent_Declare(cpplog
    URL https://github.com/vldrus/cpplog/archive/refs/tags/v0.1.3.tar.gz)
FetchContent_MakeAvailable(cpplog)

set(HTTPLIB_COMPILE ON)

FetchContent_Declare(httplib
    URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.10.9.tar.gz)
FetchContent_MakeAvailable(httplib)

FetchContent_Declare(nlohmann_json
    URL https://github.com/nlohmann/json/archive/refs/tags/v3.10.5.tar.gz)
FetchContent_MakeAvailable(nlohmann_json)
