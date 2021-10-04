cmake_minimum_required(VERSION 3.1)

PROJECT ( hgyparser )

file (GLOB PROJECT_SOURCES  "src/*.cpp")
file (GLOB PROJECT_HEADERS  "inc/*.h")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

FIND_PACKAGE(Matio REQUIRED)

ADD_DEFINITIONS( -D_USE_MATH_DEFINES )

ADD_SUBDIRECTORY ( deps )

source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Headers" FILES ${PROJECT_HEADERS})

IF (WIN32)
     SET ( GIT_EXECUTABLE "git.exe" )
ELSE (WIN32)
    SET ( GIT_EXECUTABLE "git" )
ENDIF(WIN32)

execute_process(COMMAND
    "${GIT_EXECUTABLE}"
    describe --match=NeVeRmAtCh --always --abbrev=40 --dirty
    WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE
    GIT_SHA1
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
    "${GIT_EXECUTABLE}"
    log -1 --format=%ad --date=local
    WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE
    GIT_DATE
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
    "${GIT_EXECUTABLE}" log -1 --format=%s
    WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE
    GIT_COMMIT_SUBJECT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file("${PROJECT_SOURCE_DIR}/src/version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp" @ONLY)

SET ( PROJECT_SOURCES ${PROJECT_SOURCES} "${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp" )


IF (MSVC)
    ADD_DEFINITIONS(-DDOWAIT -D_CRT_SECURE_NO_WARNINGS)

    INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}
        "${PROJECT_SOURCE_DIR}/deps/hlib/inc"
        "${PROJECT_SOURCE_DIR}/deps/getopt"
        ${LIBMATIO_INCLUDE_DIR}
        "${PROJECT_SOURCE_DIR}/inc"
        )

    SET ( PROJECT_SOURCES ${PROJECT_SOURCES} "${PROJECT_SOURCE_DIR}/deps/getopt/getopt.c")

ELSE (MSVC)

    INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}
        "${PROJECT_SOURCE_DIR}/deps/hlib/inc"
        ${LIBMATIO_INCLUDE_DIR}
        "${PROJECT_SOURCE_DIR}/inc"
        )

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=gnu++11")
    ADD_DEFINITIONS(-std=gnu++11)
ENDIF (MSVC)

link_directories(
    "/usr/local/lib"
    )

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
