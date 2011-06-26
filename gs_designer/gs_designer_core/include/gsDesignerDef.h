#ifndef __GS_DESIGNER_DEFINITION_H__
#define __GS_DESIGNER_DEFINITION_H__

#include <boost/any.hpp>
#include <string>

#include "OgreString.h"

#include "gsDesignerExport.h"

namespace gs_designer
{
	const char GS_DESIGNER_VERSION[] = "0.0.1";
	
#if GS_DOUBLE_PRECISION == 1
	/** Software floating point type.
	@note Not valid as a pointer to GPU buffers / parameters
	*/
	typedef double Real;
#else 
	/** Software floating point type.
	@note Not valid as a pointer to GPU buffers / parameters
	*/	
	typedef float Real;

#endif
	
	/** Editors Type */
	enum EditorType
	{
		ET_BASE = 0,
		ET_SCENEMANAGER,
		ET_SCENENODE,
		ET_ENTITY,
		ET_CAMERA,
		ET_VIEWPORT,
		ET_TERRAIN,
		ET_NUM
	};
		
	enum SurfaceType
	{
		ST_BASE,
		ST_ROUGH,
		ST_FAIRWAY,
		ST_GREEN,
		ST_BUNKER,
		ST_HAZARD,
		ST_TEE,
		ST_TYPENUM
	};

	/** Mouse buttons enumeration */
	enum MouseButtons
	{
		GMB_LEFT = 1,
		GMB_RIGHT = 2,
		GMB_MIDDLE = 3
	};
	
	/** Load state enumeration */
	enum LoadState
	{
		LS_UNLOADED = 0,
		LS_LOADING = 1,
		LS_LOADED = 2
	};
	
	/** Project options structure */
	struct ProjectOptions
	{
		bool               isNewProject;              /** Flag specifying if project is newly created */
		Ogre::String       createdIn;                 /** the base directory of the Project Directory */
		Ogre::String       projectDir;                /** Root directory in which project file(s) to reside (absolute) */
		Ogre::String       projectName;               /** Name of the project */
		//Ogre::String       SceneManagerName;          /** Scene manager name */
		//Ogre::String       TerrainDirectory;          /** Terrain data directory name (relative)*/
		//Ogre::String       HydraxDirectory;           /** Hydrax (water) data directory name (relative)*/
		//Ogre::String       CaelumDirectory;           /** Caelum (sky) data directory name (relative) */
		//Ogre::String       ParticleUniverseDirectory; /** ParticleUniverse data directory name (relative) */
		//Ogre::String       PagedGeometryDirectory;    /** PagedGeometry data directory name (relative) */
		//Ogre::String       SceneManagerConfigFile;    /** Scene manager configiration file name */   
		//Ogre::StringVector ResourceDirectories;       /** A string list of resource directory(ies) */
		//float              CameraSpeed;               /** Camera Speed in turns of units per second */
		//Ogre::Vector3      CameraPositions[11];       /** Camera positions array (scene can have multiple cameras) */
		//Ogre::Quaternion   CameraOrientations[11];    /** Camera orientations array (scene can have multiple cameras) */
		//int                CameraSaveCount;           /** Index of last created camera */
		//Ogre::ColourValue  SelectionRectColour;       /** Colour of the Selection Rectangle */
		//Ogre::ColourValue  SelectionBBColour;         /** Colour of the Selection Bounding Box */
		//Ogre::ColourValue  HighlightBBColour;         /** Colour of the Highlight Bounding Box */
		//Ogre::ColourValue  SelectHighlightBBColour;   /** Colour of the Selection + Highlight Bounding Box */
		//Ogre::ColourValue  GridColour;                /** Colour of the Viewport Grid */
		//Ogre::Real         GridSpacing;               /** Spacing between Viewport Grid Cells */
		//Ogre::Real         SnapAngle;                 /** Angle to use as interval during snap to angle */
		//Ogre::String       LayerNames[31];            /** Array of Layer Names */ 
		//bool               LayerVisible[31];          /** Array of Layer Visibilities */
		//int                LayerCount;                /** Number of Layers */
		//float              WalkAroundHeight;          /** The height at which camera will be kept during walkaround mode */ 
		//Ogre::Real         VolumeSelectionDepth;      /** Depth of Volume Selection Box */
		//unsigned int       ObjectCount;           /** Number of objects in scene */
	};
}



#endif
