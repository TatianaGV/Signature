cmake_minimum_required(VERSION 3.14)
project(VeeamSignature)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.72.0 COMPONENTS filesystem)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(VeeamSignature main.cpp Hasher.cpp Hasher.h)
    target_link_libraries(VeeamSignature ${Boost_LIBRARIES})
endif()