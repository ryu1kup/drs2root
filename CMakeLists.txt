#
# CMakeLists.txt
#

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(drs2root)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
set(CMAKE_CXX_STANDARD 17)
link_libraries(stdc++fs)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT CONFIG REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

add_executable(drs2root drs2root.cpp)
target_link_libraries(drs2root PUBLIC ROOT::Tree)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(TARGETS drs2root DESTINATION bin)
