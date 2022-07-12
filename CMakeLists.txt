cmake_minimum_required(VERSION 3.16)

project(resource_monitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Quick)

qt_standard_project_setup()

add_subdirectory(src)


