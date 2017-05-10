cmake_minimum_required (VERSION 3.1)


set (CMAKE_CXX_STANDARD 11)

project (Rebuild)


find_package(BISON)
BISON_TARGET(BasicParser "rebuild/Parser/quickbasic.ypp"  ${PROJECT_BINARY_DIR}/quickbasic.y.cpp)


find_package(FLEX)
FLEX_TARGET(BasicScanner "rebuild/Parser/quickbasic.lpp"  ${PROJECT_BINARY_DIR}/quickbasic.l.cpp)

ADD_FLEX_BISON_DEPENDENCY( BasicScanner BasicParser)

file(GLOB SOURCES "rebuild/linenoise/*.cpp" "rebuild/linenoise/*.c" "rebuild/*.cpp"  "rebuild/Parser/*.cpp" )

include_directories( ${CMAKE_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/rebuild )



add_executable(rebuildexe ${SOURCES}  ${BISON_BasicParser_OUTPUTS} ${FLEX_BasicScanner_OUTPUTS} )
