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
# gs_designer core DLL
############################################################

PROJECT(gsDesignerCore)

set(GSD_EXPORTS TRUE)

include_directories(${DEPENDENCIES_OGRE_INCLUDE_DIR})
include_directories(${DESIGNER_CORE_INCLUDE})
include_directories(${GSMAIN_INCLUDE})

set(HEADER_FILES 
./include/gsDesigner.h
./include/gsDesignerExport.h
./include/gsDesignerDef.h
./include/gsDesignerEvent.h
./include/gsDesignerPrerequisites.h
./include/gsDesignerRoot.h
./include/gsEditorVisualHelper.h
./include/gsProperty.h
./include/gsSingleton.h

./include/gsBaseEditor.h
./include/gsCameraEditor.h
./include/gsViewportEditor.h
./include/gsSceneNodeEditor.h
./include/gsEntityEditor.h
./include/gsSceneManagerEditor.h
./include/gsMultiSelEditor.h

./include/gsSurfaceEditor.h
./include/gsRoughSurfaceEditor.h
./include/gsBunkerEditor.h
./include/gsCourseEditor.h

./include/gsBaseSerializer.h

)

set(SOURCE_FILES
./src/gsDesignerEvent.cpp
./src/gsDesignerRoot.cpp
./src/gsEditorVisualHelper.cpp
./src/gsProperty.cpp

./src/gsBaseEditor.cpp
./src/gsCameraEditor.cpp
./src/gsViewportEditor.cpp
./src/gsSceneNodeEditor.cpp
./src/gsEntityEditor.cpp
./src/gsSceneManagerEditor.cpp
./src/gsMultiSelEditor.cpp

./src/gsBaseSerializer.cpp

./src/gsSurfaceEditor.cpp
./src/gsRoughSurfaceEditor.cpp
./src/gsBunkerEditor.cpp
./src/gsCourseEditor.cpp

)

# Specify build path (under /build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${GS_WORK_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GS_WORK_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GS_WORK_DIR}/bin")

# Build 
add_library(DesignerCore SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(DesignerCore ${DEPENDENCIES_LIBRARY} GsMain)








