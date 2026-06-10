cmake_minimum_required(VERSION 3.22)
project(CryptoBook VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# Force all libraries (yours and dependencies) to be static
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force static libraries" FORCE)

# include(FetchContent)
include(cmake/StandardProjectSettings.cmake)
include(cmake/CompilerWarnings.cmake)

# Find required packages
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# Add subdirectories for include, source, tests, and vendor libraries
add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(tests)
