cmake_minimum_required(VERSION 2.8)

PROJECT ( hgyparser )

#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

file (GLOB PROJECT_SOURCES  "src/*.cpp")
file (GLOB PROJECT_HEADERS  "inc/*.h")

ADD_DEFINITIONS( -D_USE_MATH_DEFINES )

ADD_SUBDIRECTORY (	lib	)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/lib/inc"
    "${PROJECT_SOURCE_DIR}/inc"
    )

source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Headers" FILES ${PROJECT_HEADERS})

IF (MSVC)
    ADD_DEFINITIONS(-DDOWAIT)
ELSE (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=gnu++11")
    ADD_DEFINITIONS(-std=gnu++11)
ENDIF (MSVC)


ADD_EXECUTABLE(
    hgyparser
    ${PROJECT_SOURCES}
    )

TARGET_LINK_LIBRARIES (
    hgyparser
    hgyfileformatlib
    )

ADD_DEPENDENCIES (
    hgyparser
    hgyfileformatlib
    )
