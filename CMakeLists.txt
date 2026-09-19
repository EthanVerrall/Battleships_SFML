cmake_minimum_required(VERSION 3.21)
project(Battleships LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------

include(FetchContent)

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    )

FetchContent_MakeAvailable(SFML)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    )

add_executable(Battleships ${SRC_FILES})

target_include_directories(Battleships PRIVATE "${CMAKE_SOURCE_DIR}/src")

target_link_libraries(Battleships PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    )

target_link_libraries(Battleships PRIVATE freetype)

target_compile_features(Battleships PRIVATE cxx_std_23)

if(MSVC)
    target_compile_options(Battleships PRIVATE /W4)
else()
    target_compile_options(Battleships PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------------------------
# Assets
# ---------------------------------------------------------------------------

add_custom_command(TARGET Battleships POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Battleships>/assets"
    COMMENT "Copying assets/ next to the executable"
    )

if(MINGW)
    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    foreach(_mingw_dll libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll)
        if(EXISTS "${MINGW_BIN_DIR}/${_mingw_dll}")
            add_custom_command(TARGET Battleships POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MINGW_BIN_DIR}/${_mingw_dll}"
                    "$<TARGET_FILE_DIR:Battleships>"
                )
        endif()
    endforeach()
endif()