cmake_minimum_required(VERSION 3.16)

project(cppmicro)

configure_file(config.json config.json COPYONLY)

include(FetchContent)

FetchContent_Declare(cpplog 
    URL      https://github.com/vldrus/cpplog/archive/refs/tags/v0.1.2.tar.gz
    URL_HASH SHA256=6d539823eaaaf5260e4a34ac2fe8cf0a2307ddaf0d58706eedc3a9cb2fbf1773)
FetchContent_MakeAvailable(cpplog)

FetchContent_Declare(httplib 
    URL      https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.10.9.tar.gz
    URL_HASH SHA256=95ac0740ef760829a079c01a44164fd74af3fdc0748a40fc6beefd0276fd2345)
FetchContent_MakeAvailable(httplib)

FetchContent_Declare(nlohmann_json
    URL      https://github.com/nlohmann/json/archive/refs/tags/v3.10.5.tar.gz
    URL_HASH SHA256=5daca6ca216495edf89d167f808d1d03c4a4d929cef7da5e10f135ae1540c7e4)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(cppmicro main.cpp)

target_link_libraries(cppmicro
    PRIVATE
        cpplog::cpplog
        httplib::httplib
        nlohmann_json::nlohmann_json)
