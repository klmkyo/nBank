cmake_minimum_required(VERSION 3.15)

project(nBank VERSION 0.0.5
                DESCRIPTION "nBank - better bank"
                LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/cmake/prevent_in_source_builds.cmake)

add_library(project_warnings INTERFACE)
include(${CMAKE_SOURCE_DIR}/cmake/compiler_warings.cmake)
set_project_warnings(project_warnings)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#c++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(ftxui CONFIG REQUIRED)

target_link_libraries(nBank PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(nBank PRIVATE ftxui::dom ftxui::screen ftxui::component)
