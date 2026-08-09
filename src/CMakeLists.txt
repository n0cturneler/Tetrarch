# CMakeList.txt : CMake project for CMAKE_THREE, include source and define
# project specific logic here.

# Add source to this project's executable.
add_executable (main  "main.cpp" "Pieces.cpp"  "UI.cpp"  "Camera.cpp"  "Input.cpp" "Bag.cpp" "PieceType.cpp"  "Board.cpp")

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

# Include Raylib
find_package(raylib CONFIG REQUIRED)
target_link_libraries(main PRIVATE raylib)

# Include Dear ImGui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(main PRIVATE imgui::imgui)

# rlImGui wrapper
target_sources(main PRIVATE
    ${CMAKE_SOURCE_DIR}/external/rlImGui/rlImGui.cpp
)

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/external/rlImGui
)

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

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:main>/assets
)


