#ifndef __GS_EDITOR_ROOT_H__
#define __GS_EDITOR_ROOT_H__

// std and boost headers
#include <map>
#include <boost/signals2.hpp>

// OGRE headers
#include "OgrePrerequisites.h"
#include "OgreVector2.h"

// project headers
#include "gsDesignerPrerequisites.h"
#include "gsDesignerDef.h"
#include "gsProperty.h"
#include "gsSingleton.h"

using namespace boost::signals2; 

namespace gs_designer
{
	class BaseEditorFactory; 
	class _gsdExport DesignerRootImpl
	{
	public:	
		virtual unsigned int createUniqueIdImpl(BaseEditorSharedPtr obj) {
			static unsigned int i = 0;
			return i++;
		}
	};
	typedef boost::shared_ptr<DesignerRootImpl> RootImplSharedPtr;
	
	//////////////////////////////////////////////////////////////////////////
	
	/** Root Class
	  */
	class _gsdExport DesignerRoot // : public Singleton<DesignerRoot>
	{
		friend class DesignerRootImpl;
		
		//////////////////////////////////////////////////////////////////////////
		/** typedefs */
		//////////////////////////////////////////////////////////////////////////
		typedef std::map<String, BaseEditorFactory*>				NameEditorFactoryMap;	
		typedef boost::signals2::signal<void(BaseEvent*)>			signal_t;
		typedef signal_t::slot_type									slot_t;

	public:
		DesignerRoot();
		~DesignerRoot();
		
		static DesignerRoot& getSingleton();
		static DesignerRoot* getSingletonPtr();
		
		/** Initialization */
		int prepareProjectResources();
		int loadScene(const String name);
		int loadSceneDummy();
		int afterLoadSceneDummy();
		bool terminateScene(); 
		
		/** Destruction */
		void destroyEditors();
		
		BaseEditorSharedPtr					createEditorObject(BaseEditorSharedPtr parent, const String& typeName, PropertyValueMap& params, bool show = true);
		unsigned int						createUniqueId(BaseEditorSharedPtr obj);
		void								addObjectId(unsigned int id, BaseEditorSharedPtr obj);
		bool								createSceneManagerEditor(PropertyValueMap& params);

		void								setSceneModified(bool modified);
		void								changeSceneModified(bool modified);
		
		MultiSelEditorSharedPtr				getMultiSelection();
		inline Ogre::SceneManager*			getSceneManager()		{ return m_ogreSceneMgr; }
		inline Ogre::RenderWindow*			getRenderWindow()		{ return m_ogreRenderWnd; }
		inline SceneManagerEditorSharedPtr	getSceneMgrEditor()		{ return m_sceneMgrEditor; }
		
		inline void						setRenderWindow(Ogre::RenderWindow *renderWnd) { m_ogreRenderWnd = renderWnd; } 	
		inline void						setActiveViewport(ViewportEditorSharedPtr viewport) { m_activeVp = viewport; }
		inline ViewportEditorSharedPtr	getActiveViewport()			{ return m_activeVp; }
		inline Ogre::Viewport*			getOgreVp(); 					

		inline bool						isSceneModified() const { return m_isSceneModified; }				
		inline bool						isSceneLoaded() const { return (m_loadState == LS_LOADED); }
		inline ProjectOptions*			getProjectOptions() { return &m_projectOptions; } 
		
		/** render window events */
		void renderWindowResized();	
		
		/** resources management */
		static PropertyOptionsVector*	GetTerrainDiffuseTextureNames(); 
		static PropertyOptionsVector*	GetTerrainNormalTextureNames();
		
		/** events handlers */
		void onMouseMove(Ogre::Vector2 point, unsigned int buttons);
		void onMouseLeftDown(Ogre::Vector2 point, unsigned int buttons);
		void onMouseLeftUp(Ogre::Vector2 point, unsigned int buttons);
		void onMouseRightDown(Ogre::Vector2 point, unsigned int buttons);
		void onMouseRightUp(Ogre::Vector2 point, unsigned int buttons);
		void onMouseMidDown(Ogre::Vector2 point, unsigned int buttons);
		void onMouseMidUp(Ogre::Vector2 point, unsigned int buttons); 
		void onMouseWheel(Ogre::Vector2 point, float delta, unsigned int buttons);
		
		/** editor objects management */
		void					registerEditorFactory(BaseEditorFactory *factory);
		void					registerAllEditorFactory();
		void					registerEditorObject(String& name, BaseEditorSharedPtr editorPtr);
		void					unregisterEditorObject(String& name, BaseEditorSharedPtr editorPtr);
		BaseEditorSharedPtr		findEditorObject(const String& name);
		BaseEditorFactory*		getEditorFactory(const String& typeName) const;	

		void setLoadState(LoadState state);

		/** SIGNALS */
		signal_t signalLoadStateChanged; 
		signal_t signalSceneModifiedChanged;

	private:
		static PropertyOptionsVector m_diffuseTextureNames;		
		static PropertyOptionsVector m_normalTextureNames;
		static unsigned int m_EditorObjectTypeIDCounter;
		
		//////////////////////////////////////////////////////////////////////////
		/** members */
		//////////////////////////////////////////////////////////////////////////
		RootImplSharedPtr		m_pimpl;
		NameEditorFactoryMap	m_factoryMap;
		NameEditorObjectMap		m_editorObjMap;
		NameEditorObjectMap		m_editorObjMapsByType[ET_NUM];
		NameEditorObjectMap		m_editorObjMapsByTypeID[128];
		IdObjectMap				m_idObjMap;
	
		/** states */
		bool			m_isSceneModified;
		bool			m_isSceneLoaded;
		LoadState		m_loadState;
		
		SceneManagerEditorSharedPtr		m_sceneMgrEditor;	
		ViewportEditorSharedPtr			m_activeVp;
		MultiSelEditorSharedPtr			m_multiSelection;  	
		
		/** Ogre stuff */
		Ogre::RenderWindow				*m_ogreRenderWnd;
		Ogre::Viewport					*m_ogreViewport; 
		Ogre::SceneManager				*m_ogreSceneMgr;
		
		ProjectOptions					m_projectOptions;
		
		BaseEditorSharedPtr				m_rootEditor;

	};
	
	extern _gsdExport DesignerRoot *GetRootSingleton();

}

#endif