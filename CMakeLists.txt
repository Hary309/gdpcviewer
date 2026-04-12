cmake_minimum_required(VERSION 3.10)

project(gdpcviewer)

# deps
find_package(unofficial-godot-cpp CONFIG REQUIRED)
find_package(E57Format CONFIG REQUIRED)

add_library(gdpcviewer SHARED lib/main.cpp)
target_link_libraries(gdpcviewer PRIVATE unofficial::godot::cpp E57Format)

install(
    TARGETS gdpcviewer
    LIBRARY DESTINATION lib
)
