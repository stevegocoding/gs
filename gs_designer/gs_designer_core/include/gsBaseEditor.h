#ifndef __GS_BASE_EDITOR_H__
#define __GS_BASE_EDITOR_H__

#include <boost/enable_shared_from_this.hpp>

#include "gsDesignerPrerequisites.h"
#include "gsDesignerDef.h"
#include "gsProperty.h"

#include "OgreViewport.h"
#include "OgreSceneNode.h"
#include "OgreSceneManager.h"
#include "OgreAxisAlignedBox.h"
#include "OgreManualObject.h"

#define BIND_SETTER(classname, func) (boost::bind(&classname::func, this, _1, _2))

namespace gs_designer
{
	class DesignerRoot;
	class BaseEditorFactory;
	class _gsdExport BaseEditor : public enable_shared_from_this<BaseEditor>
	{
		friend class BaseEditorFactory;
	public:
		BaseEditor(BaseEditorFactory *factory);
		~BaseEditor();
		
		virtual bool load(bool aync = true) {m_prpLoaded->set(true); return true;}
		virtual bool unload() { m_prpLoaded->set(false); return true;}
		void destroy(bool informParent = false); 
		void destroyAllChildren(); 
		void loadAllChildren();
		void unloadAllChildren();
		void createBoundingBox();
		void destroyBoundingBox();
		
		/** create properties 
		  */
		virtual void createProperties(PropertyValueMap& params);
		
		/** setters and getters interfaces of the properties 
		  */
		String&			getTypeName();
		unsigned int	getTypeId();	
		bool			getModified() { return m_prpModified->get(); }
		bool			getSelected() { return m_prpSelected->get(); }
		bool			getLoaded() { return m_prpLoaded->get(); }
		String			getName() { return m_prpName->get(); }
		unsigned int	getId() { return m_prpId->get(); }
		BaseEditorSharedPtr getParent();
			
		void			setModified(bool _value) { m_prpModified->set(_value); }
		void			setSelected(bool _value) { m_prpSelected->set(_value); }
		void			setLoadeda(bool _value) { m_prpLoaded->set(_value); }
		void			setName(const String& _value) { m_prpName->set(_value); }
		void			setParent(const String& _value) { m_prpParent->set(_value); }
		void			setId(const unsigned int _value) { m_prpId->init(_value); }	

		void			initParent(BaseEditorSharedPtr parent);
		
		/** getters  */
		EditorType			getEditorType();
		DesignerRoot*		getRoot()										{ return m_root; }  
		BaseEditorFactory*	getFactory()									{ return m_factory; }
		PropertySet*		getPropertSet()									{ return &m_properties; }
			
		// virtual void*						getHandle();						
		virtual Ogre::SceneNode*			getSceneNode();
		virtual Ogre::Vector3				getDerivedPosition()			{ return Ogre::Vector3(0,0,0); }
		virtual Ogre::Quaternion			getDerivedOrientation()			{ return Ogre::Quaternion::IDENTITY; }
		virtual Ogre::Vector3				getDerivedScale()				{ return Ogre::Vector3(1.0f, 1.0f, 1.0f); }
		virtual Ogre::AxisAlignedBox		getAABB()						{ return Ogre::AxisAlignedBox::BOX_NULL; }
		virtual Ogre::AxisAlignedBox		getWorldAABB();
		virtual bool						getEditorContextMenu(StringVector& menuItems) { menuItems.clear(); return false; }	
		virtual bool						isNodeType()					{ return false; }
		virtual bool						isSerializable()				{ return true; }
		virtual void						onContextMenu(int menuResult)	{}
		
		virtual void onMouseMove(Ogre::Vector2& pos, int button)			{}
		virtual void onMouseLeave(Ogre::Vector2& pos, int button)			{}

		/** inline functions */
		void					setVisualizer(BaseVisualizerSharedPtr visualizer) { m_visualizer = visualizer; }
		void					addChild(BaseEditorSharedPtr childPtr);
		void					removeChild(const String& name);
		BaseEditorSharedPtr		findChild(const String& name, bool recursiveSerach = false);
		
		inline bool	isLoaded()		const { return m_prpLoaded->get(); }
		inline bool isBBCreated()	const { return (m_boxRenderable != NULL); }
		inline bool isModified()	const { return m_prpModified->get(); }
	
	protected:
		/** basic property setter functions that are called when calling properties "set" methods 
		  */
		bool			_parentSetter(PropertyBase *_property, const String& _value);
		bool			_nameSetter(PropertyBase *_property, const String& _value);
		bool			_modifiedSetter(PropertyBase *_property, const bool& modified);
		bool			_selectedSetter(PropertyBase *_property, const bool& selected);
		bool			_loadedSetter(PropertyBase *_property, const bool& loaded);
		bool			_layerSetter(PropertyBase *_property, const unsigned int& layer);
		virtual bool	_setNameImpl(const String& _value);
		virtual bool	_setParentImpl(BaseEditorSharedPtr oldParent, BaseEditorSharedPtr newParent);
		virtual bool	_setSelectedImpl(bool isSelected);

		void			registerObjectName();
		void			unregisterObjectName();
		
		/** setup aabb manual object, vertices and material */
		virtual void	_createAABBImpl();

	protected:

		DesignerRoot			*m_root;						/** root  */
		BaseEditorFactory		*m_factory;						/** factory that creates this editor */
		PropertySet				m_properties;					/** properties */	
		NameEditorObjectMap		m_chidrenMap;					/** children editors */
		BaseVisualizerSharedPtr m_visualizer;					/** visualizer */
		Ogre::Viewport			*m_viewport;
		Ogre::SceneNode			*m_boxNode;						/** the scene node for attaching the bounding box */
		Ogre::SceneNode			*m_parentBoxNode;				
		Ogre::ManualObject		*m_boxRenderable;				/** bounding box manual object for rendering */
		Ogre::AxisAlignedBox	m_aabb;							/** the previous aabb data*/
		bool					m_useHelper;
		
		/** Base properties of an object for easy access, property is actually stored in Property Set */
		Property<String>::PropertySharedPtr				m_prpParent;		/** parent editor, name of the editor */
		Property<String>::PropertySharedPtr				m_prpName;			/** name property */
		Property<unsigned int>::PropertySharedPtr		m_prpId;
		Property<bool>::PropertySharedPtr				m_prpSelected;
		Property<bool>::PropertySharedPtr				m_prpModified;
		Property<bool>::PropertySharedPtr				m_prpLoaded;
		Property<unsigned int>::PropertySharedPtr		m_prpLayer;			/** the layer this object belongs to */
	};
	typedef boost::shared_ptr<BaseEditor> BaseEditorSharedPtr; 
	
	template<typename T>
	typename Property<T>::PropertySharedPtr CreateProperty(BaseEditor *editor, const String& name, const T& _value, unsigned int tag, typename Property<T>::SetterFunc setter)
	{
		PropertyDeclMap::iterator it = editor->getFactory()->m_propertyDecls.find(name);			
		Property<T>::PropertySharedPtr ptr;
		if (it != editor->getFactory()->m_propertyDecls.end())
		{
			ptr = make_shared<Property<T> >(it->second, _value, tag, setter); 
			BOOST_ASSERT(ptr != NULL);
			editor->getPropertSet()->addProperty(ptr);
		}
		return ptr;
	}

	template<typename T>
	void CreatePropertyPtr(typename Property<T>::PropertySharedPtr& p, BaseEditor *editor, const String& name, const T& _value, unsigned int tag, typename Property<T>::SetterFunc setter)
	{
		PropertyDeclMap::iterator it = editor->getFactory()->m_propertyDecls.find(name);			
		if (it != editor->getFactory()->m_propertyDecls.end())
		{
			p = make_shared<Property<T> >(it->second, _value, tag, setter); 
			BOOST_ASSERT(p != NULL);
			editor->getPropertSet()->addProperty(p);
		}
	}
	
	template<typename T>
	void AddPropertyPtr(typename Property<T>::PropertySharedPtr ptr, BaseEditor *editor, const String& name, T& _value, unsigned int tag, typename Property<T>::SetterFunc setter)
	{
		PropertyDeclMap::iterator it = editor->getFactory()->m_propertyDecls.find(name);			
		if (it != editor->getFactory()->m_propertyDecls.end())
		{
			typename Property<T>::PropertySharedPtr ptr;
			ptr = make_shared<Property<T> >(it->second, _value, tag, setter); 
			editor->getPropertSet()->addProperty(ptr);
		}	
	}
	
	/** The factory for BaseEditor class 
	*/
	class _gsdExport BaseEditorFactory 
	{
	public:
		BaseEditorFactory();
		virtual ~BaseEditorFactory();

		/** add a property declaration */
		PropertyDeclSharedPtr addPropertyDecl(const String& name, 
			const String& displayName, 
			const String& desc,
			PropertyType type,
			bool read = true,
			bool write = true,
			bool trackChanges = true);
		
		/** factory method - createObject */
		virtual BaseEditorSharedPtr createObject(BaseEditorSharedPtr parent, PropertyValueMap& params);
		virtual void				destroyObject(BaseEditorSharedPtr editorObj); 

		/** get the unique type ID and name string of this type of factory */
		unsigned int	getTypeId() { return m_typeId; }
		String&			getTypeName() { return m_typeName; }	
		EditorType		getEditorType() { return m_editorTypeEnum; }
		
		/** init the name generator */
		void			initNameGenerator();
		
	protected: 
		/** create an unique id string */
		String createUniqueName(const String& baseName);
		
	public:
		unsigned int		m_typeId;				/** unique type id that identifies the object type */
		String				m_typeName;				/** unique name of this factory */
		EditorType			m_editorTypeEnum;		/** the enum of the type of the editor */ 	
		PropertyDeclMap		m_propertyDecls;		/** static property definition data */
		unsigned int		m_instanceCount;		/** number of instances created */
		bool				m_addToObjectList;
		String				m_iconName;
		
	};
}

#endif



//////////////////////////////////////////////////////////////////////////
// Editor Built-in Properties
//////////////////////////////////////////////////////////////////////////
/*

BaseEditor: 
---------------------------------------------------------------------------------------
name			type			initial value				description	
---------------------------------------------------------------------------------------
parent			string			""							the name of the parent 
name			string			""							the nane of the editor object
id				uint			0							the id of the editor object
selected		bool			false						is the editor selected?
modified		bool			false						has the editor been modified?
loaded			bool			false						is the editor loaded 
layer			uint			0							the layer index









*/