#  Try to find QCP-LIB
#  Once done this will define
#  LIBMATIO_FOUND - System has LIBMATIO
#  LIBMATIO_INCLUDE_DIR - The LIBMATIO include directories
#  LIBMATIO_LIBRARIES - The libraries needed to use LIBMATIO
#  LIBMATIO_LIBRARIESD - The libraries needed to use LIBMATIO in debug mode
#  LIBMATIO_DEFINITIONS - Compiler switches required for using LIBMATIO
#  LIBMATIO_DLL - List of shared libraries

FIND_PATH( LIBMATIO_INCLUDE_DIR matio.h
               PATHS libs ${QT_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/deps/matio/include/" "${CMAKE_SOURCE_DIR}/deps/matio/include/" "${CMAKE_SOURCE_DIR}/deps/matio/include/" "/usr/include/"
               PATH_SUFFIXES matio ENV PATH)

FIND_PATH( LIBMATIO_INCLUDE_BASEDIR matio matio
               PATHS libs ${QT_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/deps"  "${CMAKE_SOURCE_DIR}/deps" "/usr/include/"
               PATH_SUFFIXES matio matiod ENV PATH)

find_library(LIBMATIO_LIBRARY NAMES matio LIBMATIO
						 PATHS libs "${PROJECT_SOURCE_DIR}/deps/matio/bin/" "${PROJECT_SOURCE_DIR}deps/matio/lib/" "${CMAKE_SOURCE_DIR}/deps/matio/lib/" "/usr/lib/"
             HINTS ${PC_LIBMATIO_LIBDIR} ${PC_LIBMATIO_LIBRARY_DIRS} )

find_library(LIBMATIO_LIBRARYD NAMES matiod LIBMATIOD
						 PATHS libs "${PROJECT_SOURCE_DIR}/deps/matio/bin/" "${PROJECT_SOURCE_DIR}/deps/matio/lib/" "${CMAKE_SOURCE_DIR}/deps/matio/lib/"
             HINTS ${PC_LIBMATIO_LIBDIR} ${PC_LIBMATIO_LIBRARY_DIRS} )

file(GLOB LIBMATIO_DLL "${LIBMATIO_INCLUDE_BASEDIR}/matio/bin/*.dll")
IF (WIN32)
set(LIBMATIO_LIBRARIES ${LIBMATIO_LIBRARY} "${LIBMATIO_INCLUDE_BASEDIR}/matio/lib/hdf5.lib" "${LIBMATIO_INCLUDE_BASEDIR}/matio/lib/zlib.lib" "${LIBMATIO_INCLUDE_BASEDIR}/matio/lib/szip.lib")
ELSE (WIN32)
set(LIBMATIO_LIBRARIES matio)
ENDIF(WIN32)
set(LIBMATIO_LIBRARIESD ${LIBMATIO_LIBRARYD})
set(LIBMATIO_INCLUDE_DIRS ${LIBMATIO_INCLUDE_DIR} )
#set(LIBMATIO_DEFINITIONS "-Dmatio_USE_LIBRARY")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBMATIO_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args( Matio  DEFAULT_MSG
                                  LIBMATIO_LIBRARIES LIBMATIO_INCLUDE_DIRS)

#message(STATUS "INCLUDE: ${LIBMATIO_INCLUDE_DIR}")
#message(STATUS "BASEDIR: ${LIBMATIO_INCLUDE_BASEDIR}")
#message(STATUS "DLLS   : ${LIBMATIO_DLL}")

#mark_as_advanced(LIBMATIO_INCLUDE_DIR LIBMATIO_LIBRARY LIBMATIO_LIBRARYD)
