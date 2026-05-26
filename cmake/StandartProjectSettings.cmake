# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui, ccmake
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug"
            "Release"
            "Debug-ASAN"
            "Debug-TSAN"
            "Debug-MSAN"
            "Release-PERF"
    )
endif ()

find_program(CCACHE ccache)
if (CCACHE)
    message(STATUS "Using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else ()
    message(STATUS "Ccache not found")
endif ()

# Generate compile_commands.json to make it easier to work with clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(DEACTIVATE_LOGGING "Disable logging" OFF)
if (DEACTIVATE_LOGGING)
    add_compile_definitions(APP_DEACTIVATE_LOGGING)
endif ()

option(DEBUG "Enable debug statements and asserts" OFF)
if (DEBUG OR CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG APP_ENABLE_ASSERTS APP_PROFILE)
endif ()


# Sanitizers
set(SUPPORTED_SANITIZERS "OFF" "ASAN" "TSAN" "MSAN")
set(SANITIZE "OFF" CACHE STRING "Enable sanitizers: OFF / ASAN / TSAN / MSAN")
set_property(CACHE SANITIZE PROPERTY STRINGS ${SUPPORTED_SANITIZERS})

if (SANITIZE STREQUAL "ASAN")
    message(STATUS "ASAN: using AddressSanitizer (leak detection via ASAN_OPTIONS) ")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
elseif (SANITIZE STREQUAL "TSAN")
    message(STATUS "Sanitizer: Thread + Undefined")
    add_compile_options(-fsanitize=thread,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=thread,undefined)
elseif (SANITIZE STREQUAL "MSAN")
    message(STATUS "Sanitizer: Memory + Undefined")
    add_compile_options(-fsanitize=memory,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=memory,undefined)
endif ()

option(PERF "Enable performance build mode (-O3 + profiling flags) " OFF)
if (PERF)
    message(STATUS " Performance mode enabled")
    add_compile_options(-O3 -fno-omit-frame-pointer)
endif ()
