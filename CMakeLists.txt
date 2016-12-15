cmake_minimum_required(VERSION 2.8)

PROJECT ( hgyparser )

file (GLOB PROJECT_SOURCES  "src/*.cpp")
file (GLOB PROJECT_HEADERS  "inc/*.h")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

FIND_PACKAGE(Matio REQUIRED)

ADD_DEFINITIONS( -D_USE_MATH_DEFINES )

ADD_SUBDIRECTORY ( deps )

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/deps/hlib/inc"
    ${LIBMATIO_INCLUDE_DIR}
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
    ${PROJECT_HEADERS}
    )

IF (MSVC)
TARGET_LINK_LIBRARIES (
    hgyparser
    debug
    ${LIBMATIO_LIBRARIESD}
    hg
    optimized
    ${LIBMATIO_LIBRARIES}
    hg
    )
ELSE (MSVC)
 TARGET_LINK_LIBRARIES (
    hgyparser
    ${LIBMATIO_LIBRARIES}
    hg
    )
ENDIF(MSVC)

ADD_DEPENDENCIES (
    hgyparser
    hg
    )
