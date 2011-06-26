/** boost uuid */

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "gsBaseEditor.h"
#include "gsDesignerRoot.h"
#include "gsEditorVisualHelper.h"

using namespace Ogre;

namespace gs_designer
{
	BaseEditor::BaseEditor(BaseEditorFactory *factory)
		: m_factory(factory), m_root(GetRootSingleton()), m_visualizer(BaseVisualizerSharedPtr()), m_boxNode(NULL), m_parentBoxNode(NULL), 
		m_boxRenderable(NULL), m_useHelper(false)
		
	{
		/** add basic properties to the property set */
		CreatePropertyPtr(m_prpParent, this, "parent", String(""), 0, BIND_SETTER(BaseEditor, _parentSetter));
		CreatePropertyPtr(m_prpName, this, "name", (String)"", 0, bind(&BaseEditor::_nameSetter, this, _1, _2));
		CreatePropertyPtr(m_prpId, this, "id", (unsigned int)0, 0, 0);
		CreatePropertyPtr(m_prpLoaded, this, "loaded", (bool)false, 0, bind(&BaseEditor::_loadedSetter,this,_1,_2));
		CreatePropertyPtr(m_prpSelected, this, "selected", (bool)false, 0, bind(&BaseEditor::_selectedSetter,this,_1,_2));
		CreatePropertyPtr(m_prpModified, this, "modified", (bool)false, 0, bind(&BaseEditor::_modifiedSetter,this,_1,_2));
		CreatePropertyPtr(m_prpLayer, this, "layer", (unsigned int)0, 0, bind(&BaseEditor::_layerSetter, this, _1, _2));

		/** typename */
		CreateProperty<String>(this, "typename", getTypeName(), 0, 0);
		CreateProperty<bool>(this, "destroyed", false, 0, 0);
	}
	
	BaseEditor::~BaseEditor()
	{
	}
	
	void BaseEditor::destroy(bool informParent /* = false */)
	{
		if (informParent && getParent())
			getParent()->removeChild(m_prpName->get()); 
		
		getFactory()->destroyObject(shared_from_this()); 
	}
	
	void BaseEditor::destroyAllChildren()
	{
		NameEditorObjectMap::iterator it, end;
		end = m_chidrenMap.end(); 

		for (it = m_chidrenMap.begin(); it != end; it++)
			it->second->destroy(); 
		
		m_chidrenMap.clear(); 
	}
	
	void BaseEditor::registerObjectName()
	{
		m_root->registerEditorObject(m_prpName->get(), shared_from_this()); 
	}
	
	void BaseEditor::unregisterObjectName()
	{
		m_root->unregisterEditorObject(m_prpName->get(), shared_from_this());
	}
	
	EditorType BaseEditor::getEditorType()
	{ 
		return m_factory->getEditorType(); 
	}
	
	void BaseEditor::createProperties(PropertyValueMap& params)
	{
		/** no any other specific parameters */
		m_properties.initValueMap(params);
	}
	
	void BaseEditor::loadAllChildren()
	{
		NameEditorObjectMap::iterator it, end;
		end = m_chidrenMap.end(); 
		
		for (it = m_chidrenMap.begin(); it != end; it ++)
		{
			it->second->load(); 
			it->second->loadAllChildren(); 
		}
	}
	
	void BaseEditor::unloadAllChildren()
	{
		NameEditorObjectMap::iterator it = m_chidrenMap.begin(); 
		for (; it != m_chidrenMap.end(); it++)
			it->second->unload();
	}
	
	void BaseEditor::createBoundingBox()
	{
		if (!m_prpLoaded->get())		
			return; 
		
		/** create scene node for box */
		if (m_useHelper)
			//
			int a = 0; 
		else 
			m_parentBoxNode = getSceneNode()->createChildSceneNode("parentboxnode" + m_prpName->get(), Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
		
		/** calculate the aabb */
		m_aabb = getAABB();
		
		/** create scene node for box */
		m_boxNode = m_parentBoxNode->createChildSceneNode("boxnode" + m_prpName->get(), Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
		m_boxRenderable = m_root->getSceneManager()->createManualObject("boxobj" + m_prpName->get()); 
		
		/** setup the aabb renderable */
		_createAABBImpl();
		
		/** attach the aabb renderable to the scene node */
		m_boxNode->attachObject(m_boxRenderable);
		m_boxNode->setVisible(false);
	}
	
	void BaseEditor::destroyBoundingBox()
	{
		if (!m_boxRenderable)
			return;
		
		m_boxRenderable->detachFromParent();
		OGRE_DELETE m_boxRenderable;
		m_boxRenderable = NULL;
		
	}
	
	String& BaseEditor::getTypeName() 
	{
		return m_factory->getTypeName();
	}
	
	unsigned int BaseEditor::getTypeId()
	{
		return m_factory->getTypeId();
	}
	
	bool BaseEditor::_setParentImpl(BaseEditorSharedPtr oldParent, BaseEditorSharedPtr newParent)
	{
		return true; 	
	}
	
	bool BaseEditor::_parentSetter(PropertyBase *_property, const String& _value)
	{
		String oldParentName = m_prpParent->getOld(); 
		BaseEditorSharedPtr oldParentEditor = m_root->findEditorObject(oldParentName);	
		BaseEditorSharedPtr newParentEditor = m_root->findEditorObject(_value);

		if (!newParentEditor)
			return false;
		
		if (oldParentEditor)
		{
			oldParentEditor->removeChild(m_prpName->get()); 
		}	

		_setParentImpl(oldParentEditor, newParentEditor);
		newParentEditor->addChild(shared_from_this());
		
		return true; 
	}

	bool BaseEditor::_setNameImpl(const String& _value)
	{
		// m_propName->set(_value);
		return true;
	}
	
	bool BaseEditor::_nameSetter(PropertyBase *_property, const String& _value)
	{
		String name = _value;
		boost::trim(name);			
		if (name == m_prpName->getOld() || name.empty())	
			return false;

		bool ret = _setNameImpl(name);
		if (ret)
		{
			m_root->unregisterEditorObject(m_prpName->getOld(), shared_from_this());
			m_root->registerEditorObject(m_prpName->get(), shared_from_this());
			
			// handle parent editor
			BaseEditorSharedPtr parent;
			if ((parent = m_root->findEditorObject(m_prpParent->get())))
			{
				parent->removeChild(m_prpName->getOld()); 
				parent->addChild(shared_from_this());
			}
		}
		return ret;
	}
	
	bool BaseEditor::_modifiedSetter(PropertyBase *_property, const bool& modified)
	{
		m_root->changeSceneModified(modified);
		return true;
	}
	
	bool BaseEditor::_selectedSetter(PropertyBase *_property, const bool& selected)
	{
		return true;
	}
	
	bool BaseEditor::_loadedSetter(PropertyBase *_property, const bool& selected)
	{
		return true;
	}
	
	bool BaseEditor::_layerSetter(PropertyBase *_property, const unsigned int& layer)
	{
		return true;
	}
	
	void BaseEditor::_createAABBImpl()
	{
		Ogre::Vector3 minimum = m_aabb.getMinimum();
		Ogre::Vector3 maximum = m_aabb.getMaximum(); 

		/** setup the manual object */
		m_boxRenderable->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

			m_boxRenderable->position(minimum.x, minimum.y, 1.0f);
			m_boxRenderable->position(minimum.x, maximum.y, 1.0f); 
			m_boxRenderable->position(maximum.x, maximum.y, 1.0f);
			m_boxRenderable->position(maximum.x, minimum.y, 1.0f); 

		m_boxRenderable->end();
	}
	
	bool BaseEditor::_setSelectedImpl(bool isSelected)
	{
		return true;
	}
	
	void BaseEditor::addChild(BaseEditorSharedPtr childPtr)
	{
		m_chidrenMap.insert(NameEditorObjectMap::value_type(childPtr->getName(), childPtr));
	}
	
	void BaseEditor::removeChild(const String& name)
	{
		NameEditorObjectMap::iterator it = m_chidrenMap.find(name); 			
		if (it != m_chidrenMap.end())
			m_chidrenMap.erase(it); 
	}
	
	BaseEditorSharedPtr BaseEditor::findChild(const String& name, bool recursiveSerach)
	{
		NameEditorObjectMap::iterator it = m_chidrenMap.find(name);
		if (it != m_chidrenMap.end())
			return it->second;
		
		if (recursiveSerach)
		{
			NameEditorObjectMap::iterator it = m_chidrenMap.begin();
			for ( ; it != m_chidrenMap.end(); it++)
			{
				BaseEditorSharedPtr ptr = it->second->findChild(name, recursiveSerach);		
				if (ptr == BaseEditorSharedPtr())
					return ptr;
			}
		}
		
		return BaseEditorSharedPtr();
	}
	
	Ogre::SceneNode* BaseEditor::getSceneNode()
	{
		BaseEditorSharedPtr parent = m_root->findEditorObject(m_prpParent->get());
		if (parent)
			return parent->getSceneNode(); 
		else 
			return NULL; 
	}
	
	Ogre::AxisAlignedBox BaseEditor::getWorldAABB()
	{
		AxisAlignedBox box = getAABB();
		
		Ogre::Vector3 scale(1,1,1);
		Ogre::Vector3 pos(0,0,0);	
		BaseEditorSharedPtr parent = m_root->findEditorObject(m_prpParent->get()); 
		if (parent)	
			scale = parent->getDerivedScale(); 

		pos = parent->getDerivedPosition(); 
		box.scale(scale); 
		box = Ogre::AxisAlignedBox(box.getMinimum() + pos, box.getMaximum() + pos);

		return box;
	}

	BaseEditorSharedPtr BaseEditor::getParent() 
	{ 
		BaseEditorSharedPtr parent = m_root->findEditorObject(m_prpParent->get()); 
		return parent; 
	}
	
	void BaseEditor::initParent(BaseEditorSharedPtr parent)
	{
		if (!parent)
			m_prpParent->init(""); 
		else 
			m_prpParent->init(parent->getName()); 
	}

	//////////////////////////////////////////////////////////////////////////
	/**  */
	//////////////////////////////////////////////////////////////////////////
	BaseEditorFactory::BaseEditorFactory()
		: m_typeId(0), m_typeName(""), m_editorTypeEnum(ET_BASE), m_instanceCount(0), m_addToObjectList(false), m_iconName("")
	{
		addPropertyDecl("parent", "Parent", "The name of the parent", PROP_STRING, true, true, true);
		addPropertyDecl("id", "ID", "The ID of the object", PROP_UNSIGNED_INT, false, false, false);
		addPropertyDecl("name","Name","The name of the object",PROP_STRING);
		addPropertyDecl("typename","Type Name", "The type name of the object", PROP_STRING, true, false);
		addPropertyDecl("loaded", "", "Is the object loaded?", PROP_BOOL, false, false, false); 
		addPropertyDecl("layer", "", "The layer", PROP_INT, true, true, true); 	
		addPropertyDecl("selected", "", "Is the object selected?", PROP_BOOL, false, false, false);
		addPropertyDecl("modified", "", "Is the object modified?", PROP_BOOL, false, false, false);
		addPropertyDecl("destroyed", "", "Is the object destroyed?", PROP_BOOL, false, false, false);
	}
	
	BaseEditorFactory::~BaseEditorFactory()
	{
			

	}
	
	PropertyDeclSharedPtr BaseEditorFactory::addPropertyDecl(const String& name, const String& displayName, const String& desc, PropertyType type, bool read, bool write, bool trackChanges)
	{
		PropertyDeclMap::iterator it = m_propertyDecls.find(name);
		if (it == m_propertyDecls.end())
		{
			PropertyDeclSharedPtr declPtr(new PropertyDeclaration(name, displayName,desc,type,read,write,trackChanges));
			m_propertyDecls.insert(PropertyDeclMap::value_type(name,declPtr));
			return declPtr;
		}
		return it->second;
	}

	gs_designer::BaseEditorSharedPtr BaseEditorFactory::createObject( BaseEditorSharedPtr parent, PropertyValueMap& params )
	{
		m_instanceCount++; 

		/** create object */	
		BaseEditorSharedPtr editorPtr = make_shared<BaseEditor>(this);
		editorPtr->createProperties(params);	
		editorPtr->initParent(parent);
			
		return editorPtr;
	}
	
	void BaseEditorFactory::destroyObject(BaseEditorSharedPtr editorObj)
	{
		editorObj->destroyAllChildren(); 
		editorObj->unload(); 
		
		// unregister the editor 
		if (editorObj->m_prpName->get() != "")
			GetRootSingleton()->unregisterEditorObject(editorObj->m_prpName->get(), editorObj); 
		
		m_instanceCount--; 
	}

	String BaseEditorFactory::createUniqueName(const String& baseName)
	{
		String objName = baseName + "#" + lexical_cast<String>(m_instanceCount);
		//name_generator ngen(lexical_cast<uuid>(FACTORY_UUID));
		//uuid id = ngen(objName);
		//BOOST_ASSERT(!id.is_nil());
		
		return objName;
	}
}