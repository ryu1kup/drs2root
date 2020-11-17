#
# CMakeLists.txt
#

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(drs2root)
add_executable(drs2root drs2root.cpp)

target_compile_options(drs2root PRIVATE -Wall -Wextra)
target_compile_features(drs2root PRIVATE cxx_std_17)
target_link_libraries(drs2root PRIVATE stdc++fs)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT CONFIG REQUIRED)
target_link_libraries(drs2root PRIVATE ROOT::Tree)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(TARGETS drs2root DESTINATION bin)
