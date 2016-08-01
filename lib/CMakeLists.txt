cmake_minimum_required(VERSION 2.8)

PROJECT ( hgyfileformatlib )

#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

file (GLOB LIB_SOURCES  "src/*.cpp" )
file (GLOB LIB_HEADERS  "inc/*.h" "private/*.h" )

ADD_DEFINITIONS( -D_USE_MATH_DEFINES )

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/inc"
    "${PROJECT_SOURCE_DIR}/private"
    )

source_group("Sources" FILES ${LIB_SOURCES})
source_group("Headers" FILES ${LIB_HEADERS})

IF (MSVC)
    ADD_DEFINITIONS(-DDOWAIT)
ELSE (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=gnu++11")
    ADD_DEFINITIONS(-std=gnu++11)
ENDIF (MSVC)


ADD_LIBRARY(
    hgyfileformatlib
    ${LIB_SOURCES}
    ${LIB_HEADERS}
    )
