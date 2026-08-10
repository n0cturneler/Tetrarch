# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.25)
project (ProgramApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
    set(CLANG_TIDY_CHECKS
        "-checks=-*,
        bugprone-*,
        -bugprone-easily-swappable-parameters,
        -bugprone-throwing-static-initialization,
        -bugprone-reserved-identifier,
        -clang-diagnostic-*"
    )

    set(CMAKE_CXX_CLANG_TIDY
        "${CLANG_TIDY_EXE};${CLANG_TIDY_CHECKS}"
    )

    set(CMAKE_C_CLANG_TIDY
        "${CLANG_TIDY_EXE};${CLANG_TIDY_CHECKS}"
    )
endif()

# Include sub-projects.
add_subdirectory (src)
add_subdirectory(external/raylib)

