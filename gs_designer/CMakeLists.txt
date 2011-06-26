#-------------------------------------------------------------------
# This file is part of the CMake build system for GolfSim Designer project
#-------------------------------------------------------------------

######################################################################
# GOLFSIM BUILD SYSTEM
# Welcome to the CMake build system for GolfSim
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################


############################################################
# gs_designer root CMakeLists.txt
############################################################

PROJECT(gsDesigner)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

SET(CMAKE_DEBUG_POSTFIX "_d")

# subdirectory path
set(DESIGNER_CORE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/gs_designer_core/include")
set(DESIGNER_CORE_LIB  "${GS_WORK_DIR}/lib")
set(QT_PROPERTY_BROWSER_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/gs_designer_qt/qtpropertybrowser-2.6/src")
set(QT_PROPERTY_BROWSER_LIB "${GS_WORK_DIR}/lib")
set(DESIGNER_QT_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/gs_designer_qt/include")

add_subdirectory(gs_designer_core)
add_subdirectory(gs_designer_qt/qtpropertybrowser-2.6)
add_subdirectory(gs_designer_qt)


