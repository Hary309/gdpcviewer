cmake_minimum_required(VERSION 3.10)

project(gdpcviewer)

# deps
find_package(unofficial-godot-cpp CONFIG REQUIRED)

# iOS requires a static library; other platforms use shared
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    add_library(gdpcviewer STATIC lib/main.cpp)
else()
    add_library(gdpcviewer SHARED lib/main.cpp)
endif()

target_link_libraries(gdpcviewer PRIVATE unofficial::godot::cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    install(
        TARGETS gdpcviewer
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib   # needed for STATIC (.a)
    )
else()
    install(
        TARGETS gdpcviewer
        LIBRARY DESTINATION lib
    )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    install(FILES $<TARGET_FILE:unofficial::godot::cpp> DESTINATION lib)
endif()
