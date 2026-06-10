####### toml++
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(tomlplusplus)

####### Google Test
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

####### IXWebSocket
# CRITICAL FIX: This tells IXWebSocket to enable #ifdef IXWEBSOCKET_USE_TLS
set(USE_TLS 1 CACHE BOOL "" FORCE)

FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG        v11.4.6  # Pin to a stable version
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(ixwebsocket)

if(TARGET ixwebsocket)
    # Check if we are using Visual Studio (MSVC) or GCC/Clang
    if(MSVC)
        # /W0 disables all warnings for MSVC
        target_compile_options(ixwebsocket PRIVATE /W0)
    else()
        # -w (lowercase) disables all warnings for GCC/Clang
        target_compile_options(ixwebsocket PRIVATE -w)
    endif()
endif()

####### simdjson (Fastest JSON Parser)
set(SIMDJSON_ENABLE_STATIC_REFLECTION OFF CACHE BOOL "" FORCE)
set(SIMDJSON_ENABLE_THREADS ON CACHE BOOL "" FORCE)
set(SIMDJSON_SANITIZERS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    simdjson
    GIT_REPOSITORY https://github.com/simdjson/simdjson.git
    GIT_TAG        v4.2.4 # Pin to a stable version
    GIT_SHALLOW ON
)
# simdjson has a good CMake setup, so MakeAvailable works perfectly
FetchContent_MakeAvailable(simdjson)

if(TARGET simdjson)
    if(MSVC)
        target_compile_options(simdjson PRIVATE /W0)
    else()
        target_compile_options(simdjson PRIVATE -w)
    endif()

    # 3. Mark Headers as SYSTEM
    # This fixes warnings in YOUR code when you #include <simdjson.h>
    # We retrieve the existing include dirs and re-apply them as SYSTEM INTERFACE
    get_target_property(SIMDJSON_INC simdjson INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(simdjson SYSTEM INTERFACE ${SIMDJSON_INC})
endif()

####### uWebSockets
# Uncomment the following lines to include uWebSockets as a dependency

# find_package(OpenSSL REQUIRED)
# find_package(ZLIB REQUIRED)
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(LIBUV REQUIRED IMPORTED_TARGET libuv)

# FetchContent_Declare(
#     usockets
#     GIT_REPOSITORY https://github.com/uNetworking/uSockets.git
#     GIT_TAG        v0.8.8 # Pin a compatible version
#     GIT_SHALLOW ON
# )
# FetchContent_MakeAvailable(usockets)

# # uSockets is a raw C repo, so we must manually define the library target
# if (NOT TARGET uSockets)
#     file(GLOB_RECURSE USOCKETS_SOURCES 
#         "${usockets_SOURCE_DIR}/src/*.c"
#         "${usockets_SOURCE_DIR}/src/eventing/*.c"
#         "${usockets_SOURCE_DIR}/src/eventing/*.cpp"
#         "${usockets_SOURCE_DIR}/src/crypto/*.c"
#         "${usockets_SOURCE_DIR}/src/crypto/*.cpp"
#     )

#     add_library(uSockets STATIC ${USOCKETS_SOURCES})

#     # Disable warnings
#     if(MSVC)
#         target_compile_options(uSockets PRIVATE /W0)
#     else()
#         target_compile_options(uSockets PRIVATE -w)
#     endif()

#     # Definitions
#     # REMOVED: LIBUS_USE_LIBUV=0 (Defining it at all triggers checks in some versions)
#     # ADDED: LIBUS_USE_OPENSSL
#     target_compile_definitions(uSockets PRIVATE LIBUS_USE_OPENSSL LIBUS_USE_LIBUV=1)

#     # Includes
#     target_include_directories(uSockets PUBLIC "${usockets_SOURCE_DIR}/src")
    
#     # Link dependencies
#     target_link_libraries(uSockets PUBLIC OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB PkgConfig::LIBUV)
#     target_link_libraries(uSockets PRIVATE PkgConfig::LIBUV)
# endif()

# FetchContent_Declare(
#     uwebsockets
#     GIT_REPOSITORY https://github.com/uNetworking/uWebSockets.git
#     GIT_TAG v20.74.0
#     GIT_SHALLOW ON
# )
# FetchContent_MakeAvailable(uwebsockets)

# if (NOT TARGET uWebSockets)
#     add_library(uWebSockets INTERFACE)
#     target_include_directories(uWebSockets SYSTEM INTERFACE "${uwebsockets_SOURCE_DIR}/src")
#     # uWebSockets depends on uSockets
#     target_link_libraries(uWebSockets INTERFACE uSockets)
# endif()
