# CMakeList.txt : CMake project for CMAKE_THREE, include source and define
# project specific logic here.

# Add source to this project's executable.
add_executable (main  "main.cpp" "Pieces.cpp"  "UI.cpp"  "Camera.cpp"  "Input.cpp" "Bag.cpp" "PieceType.cpp"  "Board.cpp" "raygui_impl.c")

set_source_files_properties("raygui_impl.c" PROPERTIES
    CMAKE_C_CLANG_TIDY ""
    CMAKE_CXX_CLANG_TIDY ""
)

if(MSVC)
    target_compile_options(main PRIVATE
        /W4
        /permissive-
    )
else()
    target_compile_options(main PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -pedantic
    )
endif()

# Link Raylib
target_link_libraries(main PRIVATE raylib)

# Include Raylib and Raygui
target_include_directories(main PRIVATE SYSTEM  
    "${CMAKE_SOURCE_DIR}/external/raylib/src"
    "${CMAKE_SOURCE_DIR}/external/raygui/src"
)

# Include Dear ImGui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(main PRIVATE imgui::imgui)

# Include headers 
target_include_directories(main PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_SOURCE_DIR}/libs/include"
)

# Include libraries from libs/lib
# target_link_directories (main PRIVATE
# 	"${CMAKE_SOURCE_DIR}/libs/lib"
# )

# Clone game assets to build directory
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:main>/assets
)


