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
# gsDesignerQt CMakeLists.txt
############################################################

PROJECT(gs_designer_qt)
set(QT_MIN_VERSION "4.5.0")
FIND_PACKAGE(Qt4 COMPONENTS QtSvg REQUIRED)

if(NOT QT4_FOUND)
    
	MESSAGE(SEND_ERROR "Failed to find Qt 4.5 or greater.")

else(NOT QT4_FOUND)

	include(${QT_USE_FILE})
	
	SET(CMAKE_PACKAGE_QTGUI TRUE)
    
	file(GLOB UI_FILES *.ui)
    
	set(SOURCE_FILES
		./include/mainwindow.hxx
		./src/mainwindow.cpp
		./src/mainwindowactions.cpp
		./include/propertiesview.hxx
		./src/propertiesview.cpp
		#./include/ogrewidget.hxx
		#./src/ogrewidget.cpp
		./include/ogrewidgetdebug.hxx
		./src/ogrewidgetdebug
		./src/main.cpp
	)
	
	set(MOC_SOURCE_FILES
		./include/mainwindow.hxx
		./include/propertiesview.hxx
		#./include/ogrewidget.hxx
		./include/ogrewidgetdebug.hxx
	)	

	QT4_WRAP_CPP(MOC_SRCS ${MOC_SOURCE_FILES})
	QT4_WRAP_UI(UI_SRCS ${UI_FILES})

	SOURCE_GROUP("Moc Files" FILES ${MOC_SRCS})
	SOURCE_GROUP("Ui Files" FILES ${UI_FILES})

	# if we don't include this CMake will not include ui headers properly:
	
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	
	include_directories(${DEPENDENCIES_OGRE_INCLUDE_DIR})
	include_directories(${GSMAIN_INCLUDE})
	include_directories(${DESIGNER_CORE_INCLUDE})
	include_directories(${QT_PROPERTY_BROWSER_INCLUDE})
	include_directories(${DESIGNER_QT_INCLUDE})

	QT4_ADD_RESOURCES(RESOURCE_SOURCE ./res/gsDesignerQt.qrc)
	SOURCE_GROUP("Qrc Files" FILES ${RESOURCE_SRCS})

	SET (SRCS ${SOURCE_FILES} ${MOC_SRCS} ${UI_FILES})

# Specify build path (under /build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${GS_WORK_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GS_WORK_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GS_WORK_DIR}/bin")

	
	# target
	
	#if(NOT APPLE)
		
		ADD_EXECUTABLE(DesignerQt WIN32 ${SRCS} ${RESOURCE_SOURCE})
	#endif(NOT_APPLE)

	SET_TARGET_PROPERTIES(DesignerQt PROPERTIES DEBUG_POSTFIX _d)

	# link libraries
	target_link_libraries(DesignerQt ${DEPENDENCIES_OGRE_LIBRARY})
	target_link_libraries(DesignerQt QtSolutions_PropertyBrowser-2.6 ${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES})
	target_link_libraries(DesignerQt GsMain DesignerCore)
	
	
ENDIF(NOT QT4_FOUND)






		


