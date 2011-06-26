#-------------------------------------------------------------------
# This file is part of the CMake build system for GolfSim project
#-------------------------------------------------------------------

######################################################################
# GOLFSIM BUILD SYSTEM
# Welcome to the CMake build system for GolfSim
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################


cmake_minimum_required(VERSION 2.6.2)

# Use relative paths
# This is mostly to reduce path size for command-line limits on windows
if(WIN32)
  # This seems to break Xcode projects so definitely don't enable on Apple builds
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()

project(GS)

# Include necessary submodules
set(CMAKE_MODULE_PATH 
  "${PROJECT_SOURCE_DIR}/CMake"
  "${PROJECT_SOURCE_DIR}/CMake/Utils" 
  "${PROJECT_SOURCE_DIR}/CMake/Packages"
)

set(GS_WORK_DIR ${PROJECT_BINARY_DIR})

#####################################################################
# Set up the basic build environment
#####################################################################

# Find dependencies
find_package(OGRE REQUIRED)
find_package(Bullet REQUIRED)
set(DEPENDENCIES_OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIRS} ${OGRE_Terrain_INCLUDE_DIR})
set(DEPENDENCIES_BULLET_INCLUDE_DIR ${Bullet_INCLUDE_DIR})
set(DEPENDENCIES_INCLUDE_DIR ${DEPENDENCIES_OGRE_INCLUDE_DIR} ${DEPENDENCIES_BULLET_INCLUDE_DIR})
set(DEPENDENCIES_OGRE_LIBRARY ${OGRE_LIBRARY} ${OGRE_Terrain_LIBRARY})
set(DEPENDENCIES_BULLET_LIBRARY ${Bullet_LIBRARY})
set(DEPENDENCIES_LIBRARY ${DEPENDENCIES_OGRE_LIBRARY} ${DEPENDENCIES_BULLET_LIBRARY})

set(GSMAIN_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/gs_main/include")

# include search path
include_directories(${DEPENDENCIES_INCLUDE_DIR})


if (WIN32 OR APPLE)
  if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    # We don't want to install in default system location, install is really for the SDK, so call it that
    set(CMAKE_INSTALL_PREFIX
	  "${GS_BINARY_DIR}/sdk" CACHE PATH "GS install prefix" FORCE
    )
  endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(WIN32 OR APPLE)

######################################################################
# Provide user options to customise the build process
######################################################################


##################################################################
# Now setup targets
##################################################################

# Setup gs_main project
add_subdirectory(gs_main)
add_subdirectory(gs_designer)

# Setup tests
#add_subdirectory(tests)

# Setup tools
#add_subdirectory(tools)

# Setup apps
#add_subdirectory(gs_app)

# Setup docs
#add_subdirectory(docs)