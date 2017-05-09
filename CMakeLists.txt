cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)

project (Rebuild)


file(GLOB SOURCES "rebuild/linenoise/*.cpp" "rebuild/*.cpp"  "rebuild/Parser/*.cpp" )

include_directories( ${CMAKE_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/rebuild )


add_executable(rebuild ${SOURCES})
