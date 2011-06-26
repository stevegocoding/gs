#ifndef __GS_EDITOR_PREREQUISITES_H__
#define __GS_EDITOR_PREREQUISITES_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/any.hpp>
#include <boost/foreach.hpp>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>

#include "OgreVector2.h"
#include "OgreVector3.h"

using namespace boost; 

namespace gs_designer
{
	class DesignerRoot;								// root

	/** Property Classes */
	struct PropertyOption;
	class PropertyBase;
	struct PropertyValue;
	class PropertyDeclaration;
	class PropertySet;

	/** Editor Classes */
	class BaseEditor;
	class BaseEditorFactory;
	class MultiSelEditor;
	class MultiSelEditorFactory;
	class SceneNodeEditor; 
	class SceneNodeEditorFactory;
	class EntityEditor;
	class EntityEditorFactory;
	class CameraEditor; 
	class CameraEditorFactory;
	class ViewportEditor;
	class ViewportEditorFactory;
	class SceneManagerEditor;
	class SceneManagerEditorFactory; 
	
		class SurfaceEditor;
		class SurfaceEditorFactory;
		class BunkerEditor; 
		class BunkerEditorFactory;
		class CourseEditor;
		class CourseEditorFactory;
		class HoleEditor; 
		class HoleEditorFactory; 
		class LayerEditor;
		class LayerEditorFactory;
	
	
	/** Visualizer Classes */
	class BaseVisualHelper;
	
	/** Events Classes */
	class BaseEvent;
	class SceneModifiedChangedEvent; 
	class LoadStateChangedEvent; 
	
	/** Serializer */
	class BaseSerializer;
	
	//////////////////////////////////////////////////////////////////////////

	/** typedefs */
	typedef std::string											String;
	typedef std::vector<String>									StringVector;	
	typedef boost::any											Any;	
;

	typedef boost::shared_ptr<BaseEditor>						BaseEditorSharedPtr;
	typedef boost::shared_ptr<BaseEditorFactory>				BaseEditorFactorySharedPtr;
	typedef boost::shared_ptr<MultiSelEditor>					MultiSelEditorSharedPtr;
	typedef boost::shared_ptr<PropertyBase>						PropertyBaseSharedPtr; 
	typedef boost::shared_ptr<PropertyDeclaration>				PropertyDeclSharedPtr;
	typedef boost::shared_ptr<BaseVisualHelper>					BaseVisualizerSharedPtr;
	typedef boost::shared_ptr<EntityEditor>						EntityEditorSharedPtr;
	typedef boost::shared_ptr<ViewportEditor>					ViewportEditorSharedPtr;
	typedef boost::shared_ptr<CameraEditor>						CameraEditorSharedPtr;
	typedef boost::shared_ptr<SceneManagerEditor>				SceneManagerEditorSharedPtr;
	typedef boost::shared_ptr<SurfaceEditor>					SurfaceEditorSharedPtr;
	typedef boost::shared_ptr<CourseEditor>						CourseEditorSharedPtr;
	typedef boost::shared_ptr<HoleEditor>						HoleEditorSharedPtr;
	typedef boost::shared_ptr<LayerEditor>						LayerEditorSharedPtr;

	typedef std::list<LayerEditorSharedPtr>						LayerList;
	typedef boost::unordered_map<String, BaseEditorSharedPtr>	NameEditorObjectMap;
	typedef std::map<unsigned int, BaseEditorSharedPtr>			IdObjectMap;
	
	/** alias of BOOST_FOREACH */
#define my_foreach				BOOST_FOREACH
#define my_reverse_foreach		BOOST_REVERSE_FOREACH		
#define null_shared_ptr(T)		boost::shared_ptr<T>()
	
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

	/** Test if a boost::any object can be cast to the type T 
	*/
	template <typename T>
	bool can_cast(const any& anyValue)
	{
		return typeid(T) == anyValue.type(); 
	}

	/** Get the value from boost::any object
	*/ 
	template <typename T>
	const T& get_any_value(const any& a)
	{
		BOOST_ASSERT(can_cast<T>(a));
		return *any_cast<T>(&a);
	}
	
	template <typename T>
	boost::shared_ptr<T> cast_editor_shared_ptr(BaseEditorSharedPtr ptr)
	{
		return dynamic_pointer_cast<T>(ptr);
	}
	
}


#endif