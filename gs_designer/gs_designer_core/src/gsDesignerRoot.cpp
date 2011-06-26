#include "gsDesignerRoot.h"
#include "gsDesignerEvent.h"

// editors
#include "gsBaseEditor.h"
#include "gsCameraEditor.h"
#include "gsViewportEditor.h"
#include "gsSceneNodeEditor.h"
#include "gsEntityEditor.h"
#include "gsSceneManagerEditor.h"
#include "gsMultiSelEditor.h"
#include "gsSurfaceEditor.h"
#include "gsCourseEditor.h"
#include "gsHoleEditor.h"
#include "gsLayerEditor.h"

// serializers
#include "gsBaseSerializer.h"

namespace gs_designer
{
	// template<> DesignerRoot *Singleton<DesignerRoot>::ms_Singleton = 0;	
	
	PropertyOptionsVector DesignerRoot::m_diffuseTextureNames; 
	PropertyOptionsVector DesignerRoot::m_normalTextureNames;
	unsigned int DesignerRoot::m_EditorObjectTypeIDCounter = 0;
	
	//DesignerRoot& DesignerRoot::getSingleton()
	//{
	//	assert(ms_Singleton);
	//	return *ms_Singleton;
	//}

	//DesignerRoot* DesignerRoot::getSingletonPtr()
	//{
	//	return ms_Singleton;
	//}

	//////////////////////////////////////////////////////////////////////////
	
	DesignerRoot::DesignerRoot()
		: m_isSceneLoaded(false), m_isSceneModified(false),
		m_ogreSceneMgr(NULL), m_ogreRenderWnd(NULL), m_ogreViewport(NULL)
	{
		
		m_pimpl = RootImplSharedPtr(new DesignerRootImpl());
		m_loadState = LS_UNLOADED; 
		
		// register all the factories
		registerAllEditorFactory();
		
		// create root editor
		PropertyValueMap params; 
		unsigned int id = 1;
		params["id"] = PropertyValue(PROP_UNSIGNED_INT, boost::any(id)); 
		params["name"] = PropertyValue(PROP_STRING, boost::any(String("root")));
		
		BaseEditorFactory *factory = getEditorFactory(""); 
		m_rootEditor = factory->createObject(null_shared_ptr(BaseEditor),params);
		registerEditorObject(m_rootEditor->getName(), m_rootEditor);
		
		m_idObjMap[id] = m_rootEditor;
	}
	
	DesignerRoot::~DesignerRoot()
	{
		destroyEditors(); 			
		
		// delete the factories
		NameEditorFactoryMap::iterator it = m_factoryMap.begin(); 
		for (; it != m_factoryMap.end(); it++)
		{
			delete it->second; 
		}
		m_factoryMap.clear(); 
	}
	
	//////////////////////////////////////////////////////////////////////////
	/** Initialization */
	//////////////////////////////////////////////////////////////////////////
	int DesignerRoot::prepareProjectResources()
	{
		return 0;
	}
	
	int DesignerRoot::loadScene(const String name)
	{
		return 0; 
	}
	
	int DesignerRoot::loadSceneDummy()
	{
		setLoadState(LS_LOADING); 
		BaseSerializer *serializer = new DummySerializer("Dummy Serializer");
		int ret = serializer->unserialize("");
		
		return ret;
	}
	
	bool DesignerRoot::createSceneManagerEditor(PropertyValueMap& params)
	{
		m_sceneMgrEditor = cast_editor_shared_ptr<SceneManagerEditor>(createEditorObject(BaseEditorSharedPtr(), "OctreeSceneManager", params));
		if (!m_sceneMgrEditor)
			return false;
		
		SceneManagerEditorSharedPtr sceneMgrEditor = cast_editor_shared_ptr<SceneManagerEditor>(m_sceneMgrEditor);
		m_ogreSceneMgr = sceneMgrEditor->getSceneManager();
		
		return true;
	}

	int DesignerRoot::afterLoadSceneDummy()
	{
		// create viewport
		PropertyValueMap params; 
		if (!m_activeVp)
		{
			params.clear(); 
			PropertyValue pval;

			pval.type = PROP_STRING; 
			pval.val = boost::any(String("viewport1")); 
			params["name"] = pval; 
			
			pval.type = PROP_COLOUR;
			pval.val = boost::any(Ogre::ColourValue(0,0,0));
			params["colour"] = pval; 

			pval.type = PROP_INT; 
			pval.val = boost::any((int)1);
			params["index"] = pval; 

			// camera position
			pval.type = PROP_VECTOR3;
			pval.val = boost::any(Ogre::Vector3(0,0,10));
			params["camera::position"] = pval; 
			
			// create viewport object
			m_activeVp = cast_editor_shared_ptr<ViewportEditor>(createEditorObject(BaseEditorSharedPtr(), "Viewport Object", params, false));
			m_activeVp->_createCustomRqInvSeq(); 
			setActiveViewport(m_activeVp);
		}
		m_rootEditor->loadAllChildren();
		
		// set the camera
		CameraEditorSharedPtr cameraEditor = m_activeVp->getCameraEditor();
		cameraEditor->setPosition(Ogre::Vector3(0.0f, 0.0f, 40.0f));
		cameraEditor->lookAt(Ogre::Vector3(0.0f, 0.0f, -1.0f));
		
		setLoadState(LS_LOADED); 
		setSceneModified(false);
		
		return 0; 
	}
	
	bool DesignerRoot::terminateScene()
	{
		if (m_loadState == LS_UNLOADED)
			return true; 
		
		setLoadState(LS_UNLOADED); 
		destroyEditors(); 
		
		return true;
	}
	
	void DesignerRoot::destroyEditors()
	{
		//Make sure to unLoad all the viewports first, since they are dependent on SceneManager
		//but they share the same parent with scene manager and scene manager may get deleted before they do
		//NameEditorObjectMap::iterator it = m_editorObjMapsByType[ET_VIEWPORT].begin();
		//for (; it != m_editorObjMapsByType[ET_VIEWPORT].end(); it++)
		//{
		//	it->second->unload(); 
		//	it++;
		//}	
		
		m_rootEditor->destroy(); 
		setSceneModified(false);
	}
	
	void DesignerRoot::registerEditorFactory(BaseEditorFactory *factory)
	{
		if (!factory)
			return;
		
		NameEditorFactoryMap::iterator it = m_factoryMap.find(factory->getTypeName());
		if (it == m_factoryMap.end())
		{
			m_factoryMap.insert(NameEditorFactoryMap::value_type(factory->getTypeName(),factory));
			factory->m_typeId = m_EditorObjectTypeIDCounter++;
		}
	}
	
	void DesignerRoot::registerAllEditorFactory()
	{
		registerEditorFactory(new BaseEditorFactory());
		registerEditorFactory(new CameraEditorFactory());
		registerEditorFactory(new ViewportEditorFactory()); 
		registerEditorFactory(new SceneNodeEditorFactory());
		registerEditorFactory(new EntityEditorFactory());
		registerEditorFactory(new SceneManagerEditorFactory());
		
		registerEditorFactory(new CourseEditorFactory());
		registerEditorFactory(new HoleEditorFactory());
		registerEditorFactory(new LayerEditorFactory());
		registerEditorFactory(new SurfaceEditorFactory());
		
	}
	
	//void DesignerRoot::registerVisualizerFactory(String editorTypeName, BaseVisualizerFactory *factory)
	//{
	//	if (!factory)
	//		return;
	//	
	//	/** find the editor name */
	//	NameEditorFactoryMap::iterator it = m_factoryMap.find(editorTypeName);
	//	// if not found, means the editor factory has not been registered yet
	//	if (it == m_factoryMap.end())
	//		return; 
	//	else
	//	{
	//		m_visualizerFactoryMap.insert(NameVisualizerFactoryMap::value_type(editorTypeName, factory));
	//	}	
	//}
	
	BaseEditorFactory *DesignerRoot::getEditorFactory(const String& typeName) const 
	{
		NameEditorFactoryMap::const_iterator it = m_factoryMap.find(typeName);				

		if (it != m_factoryMap.end()) 
			return it->second; 
		else
			return NULL; 
	}
		
	//BaseVisualizerFactory *DesignerRoot::getVisualizerFactory(const String& typeName) const
	//{
	//	NameVisualizerFactoryMap::const_iterator it = m_visualizerFactoryMap.find(typeName);			
	//
	//	if (it != m_visualizerFactoryMap.end()) 
	//		return it->second;
	//	else
	//		return NULL; 	
	//}

	BaseEditorSharedPtr DesignerRoot::createEditorObject(BaseEditorSharedPtr parent, const String& typeName, PropertyValueMap& params, bool show)
	{
		if (!parent)
		{
			// find parent editor
			PropertyValueMap::const_iterator it; 
			if ( (it = params.find("parent")) != params.end() )
			{
				String parentName = get_any_value<String>(it->second.val); 
				parent = findEditorObject(parentName);
			}
			
			// if the parent editor not found, then set the parent as the root editor
			if (!parent)
				parent = m_rootEditor;	
		}	

		BaseEditorFactory *factory = getEditorFactory(typeName);
		/** if the factory has not been found, return a null object */
		if (!factory)
			return BaseEditorSharedPtr(); 
		
		// create the editor object
		BaseEditorSharedPtr editorPtr = factory->createObject(parent, params);
		
		if (!editorPtr)
			return BaseEditorSharedPtr(); 
	
		if (editorPtr->getId() == 0)
		{
			unsigned int id = createUniqueId(editorPtr);						
			editorPtr->setId(id);
		}
		else 
			addObjectId(editorPtr->getId(), editorPtr);
		
		registerEditorObject(editorPtr->getName(), editorPtr);
		
		// link the parent and child 
		parent->addChild(editorPtr);
		editorPtr->setParent(parent->getName());
		
		if (show)
		{
			editorPtr->load(); 
		}
		
		setSceneModified(true);
		
		return editorPtr;
	}
	
	unsigned int DesignerRoot::createUniqueId(BaseEditorSharedPtr obj)
	{
		return m_pimpl->createUniqueIdImpl(obj);		
	}
	
	void DesignerRoot::addObjectId(unsigned int id, BaseEditorSharedPtr obj)
	{
		BOOST_ASSERT(obj != NULL);	
		m_idObjMap.insert(IdObjectMap::value_type(id, obj));
	}

	//////////////////////////////////////////////////////////////////////////
	/** Scene Management Functions */
	//////////////////////////////////////////////////////////////////////////
	void DesignerRoot::setSceneModified(bool modified)
	{
		if (m_isSceneModified != modified)
		{
			m_isSceneModified = modified;
		
			// emit signals
			SceneModifiedChangedEvent evt(modified);	
			signalSceneModifiedChanged(&evt);
		}
	}
	
	void DesignerRoot::changeSceneModified(bool modified)
	{
		setSceneModified(m_isSceneModified | modified);
	}
	
	void DesignerRoot::renderWindowResized()
	{

	}
	
	Ogre::Viewport*	DesignerRoot::getOgreVp()
	{
		return m_activeVp->getOgreVp(); 
	}
	
	//////////////////////////////////////////////////////////////////////////
	/** Events Handlers */
	//////////////////////////////////////////////////////////////////////////
	
	void DesignerRoot::onMouseMove(Ogre::Vector2 point, unsigned int buttons)
	{
		if (!m_activeVp)				
			return;
		Ogre::Vector4 rect;
		m_activeVp->getRect(rect);

		if((rect.x <= point.x) && (rect.y <= point.y) && ((rect.x + rect.z) >= point.x) && ((rect.y + rect.w) >= point.y))
		{
			m_activeVp->onMouseMove(point - Ogre::Vector2(rect.x, rect.y), buttons);			
		}
		else
		{
			m_activeVp->onMouseLeave(point - Ogre::Vector2(rect.x, rect.y), buttons);	
		}
	}
	
	void DesignerRoot::onMouseLeftDown(Ogre::Vector2 point, unsigned int buttons)
	{
		// @TODO : Get the viewport with maximum z order
		
		// transform the widget space coordinates to viewport space
		Ogre::Vector4 rect;
		m_activeVp->getRect(rect); 
		m_activeVp->onMouseLeftDown(point - Ogre::Vector2(rect.x, rect.y), buttons);
	}
	
	void DesignerRoot::onMouseLeftUp(Ogre::Vector2 point, unsigned int buttons)
	{
			
				
	}
	
	void DesignerRoot::onMouseRightDown(Ogre::Vector2 point, unsigned int buttons)
	{
			
		
	}
	
	void DesignerRoot::onMouseRightUp(Ogre::Vector2 point, unsigned int buttons)
	{

	}

	void DesignerRoot::onMouseMidDown(Ogre::Vector2 point, unsigned int buttons)
	{

	}

	void DesignerRoot::onMouseMidUp(Ogre::Vector2 point, unsigned int buttons)
	{

	}
	
	void DesignerRoot::onMouseWheel(Ogre::Vector2 point, float delta, unsigned int buttons)
	{
	}
	
	/** resources management */
	PropertyOptionsVector *DesignerRoot::GetTerrainDiffuseTextureNames() 
	{ 
		return &m_diffuseTextureNames; 
	}
	PropertyOptionsVector *DesignerRoot::GetTerrainNormalTextureNames() 
	{ 
		return &m_normalTextureNames; 
	}
	
	//////////////////////////////////////////////////////////////////////////
	/** Editors Objects Management */
	//////////////////////////////////////////////////////////////////////////
	
	void DesignerRoot::registerEditorObject(String& name, BaseEditorSharedPtr editorPtr)	
	{
		m_editorObjMap.insert(NameEditorObjectMap::value_type(name, editorPtr));
		m_editorObjMapsByType[editorPtr->getEditorType()].insert(NameEditorObjectMap::value_type(name,editorPtr));
		m_editorObjMapsByTypeID[editorPtr->getTypeId()].insert(NameEditorObjectMap::value_type(name,editorPtr));
	}
	
	void DesignerRoot::unregisterEditorObject(String& name, BaseEditorSharedPtr editorPtr)
	{
		NameEditorObjectMap::iterator it = m_editorObjMap.find(name);
		if (it != m_editorObjMap.end())
			m_editorObjMap.erase(it); 
		
		it = m_editorObjMapsByType[editorPtr->getEditorType()].find(name);
		if (it != m_editorObjMapsByType[editorPtr->getEditorType()].end())
			m_editorObjMapsByType[editorPtr->getEditorType()].erase(it);
		
		it = m_editorObjMapsByTypeID[editorPtr->getTypeId()].find(name); 
		if (it != m_editorObjMapsByTypeID[editorPtr->getTypeId()].end())
			m_editorObjMapsByTypeID[editorPtr->getTypeId()].erase(it); 
		
	}
	
	BaseEditorSharedPtr DesignerRoot::findEditorObject(const String& name)
	{
		NameEditorObjectMap::iterator it = m_editorObjMap.find(name);
		if (it != m_editorObjMap.end())
			 return it->second; 
		else
			return BaseEditorSharedPtr();
	}
	
	void DesignerRoot::setLoadState(LoadState state)
	{
		if (m_loadState != state)
		{
			m_loadState = state;
			
			// emit the signal
			LoadStateChangedEvent evt(state);
			signalLoadStateChanged(&evt);
		}
	}
	
	MultiSelEditorSharedPtr DesignerRoot::getMultiSelection()
	{
		// if null, create one
		if (!m_multiSelection && m_loadState == LS_UNLOADED)	
		{
			PropertyValueMap params; 
			params.clear();
			unsigned int id = 20000000;
			params["id"] = PropertyValue(PROP_INT, Any(id));
			m_multiSelection = cast_editor_shared_ptr<MultiSelEditor>(createEditorObject(MultiSelEditorSharedPtr(), "Multiple Selection", params));
		}
		
		return m_multiSelection;
	}


	//////////////////////////////////////////////////////////////////////////
	
	DesignerRoot *GetRootSingleton() 
	{
		static DesignerRoot root; 
		return &root;
	}


}