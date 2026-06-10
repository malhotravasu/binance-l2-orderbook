include(FetchContent)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
# Force the linker to use all available cores for LTO
add_link_options(-flto=auto)

# Use custom linker
add_link_options("-fuse-ld=lld")

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS
    "Setting build type to 'Debug' as none was specified.")
  set(
    CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build." FORCE)

  # Set possible build type values for cmake-gui and ccmake
  set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS "Debug" "Release")
endif ()

# Generate compile_commands.json to make it easier to work
# with clang based tools. Used in combination with Ninja.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Debug option that also enables asserts and profiling
option(DEBUG "Enable debug statements" OFF)
if (DEBUG OR CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(
    APP_DEBUG APP_ENABLE_ASSERTS APP_PROFILE)
endif ()
