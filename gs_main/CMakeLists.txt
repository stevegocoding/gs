#-------------------------------------------------------------------
# This file is part of the CMake build system for GolfSim project
#-------------------------------------------------------------------

######################################################################
# GOLFSIM BUILD SYSTEM
# Welcome to the CMake build system for GolfSim
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################

############################################################
# gs_main core library
############################################################

PROJECT(gsMain)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

# Header Files
set (HEADER_FILES 
include/cmake_test.h
include/gsEngineDef.h
include/gsEnginePrerequisites.h
include/gsSingleton.h
)

# Source Files
set (SOURCE_FILES
src/cmake_test.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set target properties
SET(CMAKE_DEBUG_POSTFIX "_d")

# Specify build path (under /build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${GS_WORK_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GS_WORK_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GS_WORK_DIR}/bin")

# Setup gsMain target
#add_executable( GsMain ${HEADER_FILES} ${SOURCE_FILES} )
add_library( GsMain SHARED ${HEADER_FILES} ${SOURCE_FILES} )

# library link
MESSAGE(STATUS ${OGRE_LIBRARY})
target_link_libraries(GsMain ${DEPENDENCIES_LIBRARY})

