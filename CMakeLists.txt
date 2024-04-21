# CMakeList.txt : CMake project for sc_t-2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Set the project name
project ("sc_t-2" LANGUAGES CXX)

# Add source files to the project's executable.
add_executable (sc_t-2 "sc_t-2.cpp" "sc_t-2.h")

# Set C++ standard to C++20 if supported
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10.0)
    target_compile_features(sc_t-2 PUBLIC cxx_std_20)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29)
    target_compile_options(sc_t-2 PUBLIC /std:c++latest)
endif()

# TODO: Add tests and install targets if needed.
