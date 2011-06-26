#ifndef __GS_PROPERTY_H__
#define __GS_PROPERTY_H__

#include <string>
#include <vector>
#include <map>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/signal.hpp>
#include <boost/signals2.hpp>

#include "gsDesignerPrerequisites.h"
#include "gsDesignerDef.h"

using namespace std;

namespace gs_designer
{
	/** Property value type  */
	enum PropertyType
	{
		PROP_SHORT = 0,
		PROP_UNSIGNED_SHORT = 1,
		PROP_INT = 2,
		PROP_UNSIGNED_INT = 3,
		PROP_LONG = 4, 
		PROP_UNSIGNED_LONG = 5,
		PROP_REAL = 6,
		PROP_STRING = 7,
		PROP_VECTOR2 = 8, 
		PROP_VECTOR3 = 9,
		PROP_VECTOR4 = 10, 
		PROP_COLOUR = 11,
		PROP_BOOL = 12,
		PROP_QUATERNION = 13, 
		PROP_MATRIX3 = 14,
		PROP_MATRIX4 = 15, 

		PROP_UNKNOWN = 999		
	};
	
	/** Structure defining one option of a property. 
	  */
	struct PropertyOption
	{
		String m_key;
		Any m_value;

		PropertyOption(const String& key, const Any& _value) : m_key(key), m_value(_value) {}

		static bool comp_func(PropertyOption o1, PropertyOption o2)
		{
			return strcmp(o1.m_key.c_str(), o2.m_key.c_str()) < 0;
		}
	};
	typedef std::vector<PropertyOption> PropertyOptionsVector;
	
	//////////////////////////////////////////////////////////////////////////
	/** The definition of one property, metadata a property
	  * does not contain value, but the type of value.
	  */
	//////////////////////////////////////////////////////////////////////////
	class _gsdExport PropertyDeclaration
	{
	
	public:
		PropertyDeclaration(const String& name, 
			const String& displayName, 
			const String& desc, 
			PropertyType type, 
			bool read = true, bool write = true, bool trackChanges = true)	
			:m_name(name), m_displayName(displayName), m_desc(desc), m_valueType(type), 
			m_canRead(read), m_canWrite(write), m_trackChanges(trackChanges)
		{
		}
		const String& getName() const						{ return m_name; }
		const String& getDisplayName() const				{ return m_displayName; }
		const String& getDescription() const				{ return m_desc; }
		const PropertyType getValueType() const				{ return m_valueType; }
		const bool canRead() const							{ return m_canRead; }
		const bool canWrite() const							{ return m_canWrite; }
		void setAccess(bool read, bool write)				{ m_canRead = read; m_canWrite = write; }
		bool getTrackChanges() const						{ return m_trackChanges; }
		void setTrackChanges(bool trackChanges)				{ m_trackChanges = trackChanges; }		
		void setOptions(PropertyOptionsVector *options)		{m_options = options;}
		PropertyOptionsVector* getOptions()					{ return m_options; }

		/// Return FieldNames of the property
		const String& fieldName(int index) const { return m_fieldNames[index]; }

		/// Set FieldNames of the property
		void  setFieldNames(const String& x = "X", const String& y = "Y", const String& z = "Z", const String& w = "W") 
		{ 
			m_fieldNames[0] = x; 
			m_fieldNames[1] = y; 
			m_fieldNames[2] = z; 
			m_fieldNames[3] = w; 
		}
		
		static PropertyType getTypeForValue(const short& val) { return PROP_SHORT; }
		static PropertyType getTypeForValue(const unsigned short& val) { return PROP_UNSIGNED_SHORT; }
		static PropertyType getTypeForValue(const int& val) { return PROP_INT; }
		static PropertyType getTypeForValue(const unsigned int& val) { return PROP_UNSIGNED_INT; }
		static PropertyType getTypeForValue(const long& val) { return PROP_LONG; }
		static PropertyType getTypeForValue(const unsigned long& val) { return PROP_UNSIGNED_LONG; }
		static PropertyType getTypeForValue(const Real& val) { return PROP_REAL; }
		static PropertyType getTypeForValue(const String& val) { return PROP_STRING; }
		static PropertyType getTypeForValue(const Ogre::Vector2& val) { return PROP_VECTOR2; }
		static PropertyType getTypeForValue(const Ogre::Vector3& val) { return PROP_VECTOR3; }
		static PropertyType getTypeForValue(const Ogre::Vector4& val) { return PROP_VECTOR4; }
		static PropertyType getTypeForValue(const Ogre::ColourValue& val) { return PROP_COLOUR; }
		static PropertyType getTypeForValue(const bool& val) { return PROP_BOOL; }
	
	protected:
		/** disable default constructor */
		PropertyDeclaration() {}
		
		/** name of the property */
		String m_name;
		
		/** display name  */
		String m_displayName;

		/** description */
		String m_desc;
		
		/** type of the value */
		PropertyType m_valueType;
		
		/** options */
		PropertyOptionsVector *m_options;	
		
		/** accessibility */
		bool m_canWrite;
		bool m_canRead;
		bool m_trackChanges;
		
		String m_fieldNames[4];
	};
	typedef boost::shared_ptr<PropertyDeclaration> PropertyDeclSharedPtr;
	typedef std::map<String, PropertyDeclSharedPtr> PropertyDeclMap;
	
	//////////////////////////////////////////////////////////////////////////
	/** Property Base  
	  */
	//////////////////////////////////////////////////////////////////////////
	class _gsdExport PropertyBase
	{
		typedef boost::signals2::signal< bool (PropertyBase *p, const Any _value) > Signal;
		typedef Signal::slot_type Slot; 
		typedef boost::signals2::connection Connection;

	public:	
		PropertyBase(PropertyDeclSharedPtr decl, unsigned int tag)
			: m_propertyDecl(decl), m_tag(tag)
		{}
		virtual ~PropertyBase() {}
		
		virtual Any getValue() const = 0;
		virtual void setValue(const Any& _value) = 0;
		virtual Any getOldValue() const = 0;
			
		const String& getName() const						{ return m_propertyDecl->getName(); }
		const unsigned int getTag() const					{ return m_tag; }
		PropertyDeclSharedPtr getDecl() const				{ return m_propertyDecl; }
		void setDecl(PropertyDeclSharedPtr decl)			{ m_propertyDecl = decl; }		
		PropertyType getValueType() const					{ return m_propertyDecl->getValueType(); }
		
		virtual String getOptionName() = 0;
		virtual String getOptionNameIndex(int& index) = 0;
		
		void connect(Slot slotFunc) 
		{ 
			m_connection = signal.connect(slotFunc); 
		}
		
	protected:
		/** disable default constructor */	
		PropertyBase() {}	
		PropertyDeclSharedPtr	m_propertyDecl;
		unsigned int			m_tag;
		
		/** @TODO (done) : signals emit when this property changed */
		/** signal and connection */ 
		Signal					signal;
		Connection				m_connection; 
	};

	
	//////////////////////////////////////////////////////////////////////////
	/** Property Instance */
	//////////////////////////////////////////////////////////////////////////
	
	template <typename T>
	class Property : public PropertyBase
	{
	public:
		typedef T value_type;
		
		/** setter function: the function used to set a value for this property */
		typedef boost::function< bool (PropertyBase *p, const value_type&) > SetterFunc;
		typedef boost::function< value_type (void) > GetterFunc;
		typedef boost::shared_ptr<SetterFunc> SetterFuncSharedPtr;
		typedef boost::shared_ptr<Property<value_type> > PropertySharedPtr;


		Property (PropertyDeclSharedPtr decl, value_type _value, unsigned int tag = 0, SetterFunc setterFunc = NULL)
			: PropertyBase(decl,tag)
		{ 
			m_value = _value;
			m_oldValue = _value;
			m_setterFunc = setterFunc;	
		}
		
		~Property() {}
		
		void setSetterFunc(SetterFunc setterFunc)
		{
			m_setterFunc = setterFunc;
		}
								
		virtual void set(value_type _value)
		{
			if (_value == m_value)
				return; 
			
			m_oldValue = m_value;
			m_value = _value;
			
			if (m_setterFunc)
			{
				/** call the setter function */
				if (!m_setterFunc(this, _value))
				{
					m_value = m_oldValue; 
					return;
				}
			}
			
			/** emit the signal */
			Any anyValue(m_value);
			signal(this, anyValue);
		}
		
		virtual value_type get() const
		{
			return m_value;
		}
		
		virtual value_type getOld() const 
		{
			return m_oldValue; 
		}

		virtual void init(value_type _value)
		{
			m_value = _value;
		}
		 
		virtual void initAndSignal(value_type _value)
		{
			m_oldValue = m_value;
			m_value = _value; 
			
			// emit the signal
			Any anyValue(_value); 
			signal(this, anyValue);
		}
		
		virtual void setByOptionValue(const String& optionName)
		{
			//PropertyOptionsVectorSharedPtr options = m_propertyDecl->getOptions(); 
			//BOOST_ASSERT(options != NULL); 
			//			
			//foreach(PropertyOption option, *options)
			//{
			//	if (option.m_key == optionName)	
			//	{
			//		setValue(option.m_value);
			//		return;
			//	}
			//}
			//BOOST_ASSERT(false &&  "Could not find the option in the options vector");	
		}
		
		virtual String getOptionName()
		{
			PropertyOptionsVector *options = m_propertyDecl->getOptions();
						
			my_foreach(PropertyOption o, *options)	
			{
				if (get_any_value<T>(o.m_value) == m_value)
					return o.m_key;
			}

			return String("");
		}
		
		virtual String getOptionNameIndex(int& index)
		{
			PropertyOptionsVector *options = m_propertyDecl->getOptions();
			
			for (unsigned int i = 0; i < options->size(); i++)
			{
				if (get_any_value<T>((*options)[i].m_value) == m_value)	
				{
					index = i; 
					return (*options)[i].m_key;
				}
			}
			return String("");
		}

		void setValue(const Any& _value)
		{
			//T val = get_any_value<T>(_value); 
			T val = boost::any_cast<T>(_value);
			
			if (val == m_value)
				return;
			
			m_oldValue = m_value; 
			m_value = val; 
			if (m_setterFunc)
			{
				if (!m_setterFunc(this,val))
				{
					m_value = m_oldValue; 
					return;
				}
			}
			
			/** emit the signal */
			Any anyValue(m_value);
			signal(this, anyValue);
		}
	
		Any getValue() const
		{
			return Any(get());	
		}
		
		Any getOldValue() const
		{
			return Any(m_oldValue);	
		}

	protected:
		value_type		m_value;
		value_type		m_oldValue;
		
		/** setter function */
		SetterFunc		m_setterFunc;

		
		Property() {}

	};
	
	/** Represents a value of a property
	  */
	struct PropertyValue
	{
		PropertyType type; 
		Any val;
		
		PropertyValue()
			:val(PROP_UNKNOWN) {}
		
		PropertyValue(PropertyType _type, Any _val)
			: type(_type), val(_val) {}
		
		static PropertyValue CreateFromString(PropertyType type, const String& _val)
		{
			PropertyValue propertyValue;
			propertyValue.type = type;
			
			/** parse the string to PropertyValue */
			/*
				switch(type)
				{
					
				}

			*/
			return propertyValue; 
		}
	};
	
	//////////////////////////////////////////////////////////////////////////
	/** Property Set */
	//////////////////////////////////////////////////////////////////////////

	/** const empty property value */
	const PropertyValue EMPTY_PROPERTY_VALUE(PROP_STRING, Any(String("")));
	
	/** <name,value> map */
	typedef std::map<String, PropertyValue> PropertyValueMap;
	/** PropertyBaseSharedPtr vector */
	typedef std::vector<PropertyBaseSharedPtr> PropertyEntriesVector;
	/** <name,PropertyBaseSharedPtr> map */
	typedef std::map<String, PropertyBaseSharedPtr> PropertyEntriesMap;
	
	/** property iterators */
	typedef PropertyEntriesMap::iterator PropIterator;
	typedef PropertyEntriesMap::const_iterator  ConstPropIterator;
	
	enum PropertySetOwnerTypes
	{
		PROPSETOWNER_ROOT = 0,
		PROPSETOWNER_EDITOR = 1,
		PROPSETOWNER_CUSTOM = 2
	};

	enum PropertySetType
	{
		PROPSET_OBJECT = 0,
		PROPSET_CUSTOM = 1,
		PROPSET_PHYSICS = 2
	};
	
	//struct PropertySetOwnerData
	//{
	//	PropertySetOwnerTypes m_ownerType;
	//					
	//};
	class PropertySetListener
	{
	public:
		PropertySetListener() {}
		virtual ~PropertySetListener() {}
		
		virtual void onPropertyRemoved(PropertySet *set, PropertyBaseSharedPtr prop) = 0;
		virtual void onPropertyAdded(PropertySet *set, PropertyBaseSharedPtr prop) = 0;
		virtual void onPropertyChanged(PropertySet *set, PropertyBaseSharedPtr prop) = 0;
	};
	
	typedef std::vector<PropertySetListener*> PropertySetListenerVec;
	
	/** Set of Properties */	
	class _gsdExport PropertySet
	{
	public:
		PropertySet(); 
		~PropertySet()
		{
			m_propertyEntriesVec.clear();
			m_propertyEntriesMap.clear();
			m_propertySetListeners.clear();
		}
		 
		/** test if this set is empty */
		bool isEmpty() const { return m_propertyEntriesVec.size() == 0; }
		
		/** add a property to the set */
		void addProperty(PropertyBaseSharedPtr prop);
		
		/** get the property by name */
		PropertyBaseSharedPtr getProperty(const String& name) const;
		
		/** return true if the set contains the property */
		bool hasProperty(const String& name) const;
		
		/** init the property value map */
		void initValueMap(const PropertyValueMap& _values);

		// /** get property iterator */
		//PropIterator getPropertyIterator();
		//ConstPropIterator getConstPropertyIterator() const;
		
		/** register a property listener
		  */
		void addListener(PropertySetListener *listener);

		template<typename T>
		void setPropertyValue(const String& name, const T *_value)
		{
			setPropertyValueImpl(name, _value, PropertyDeclaration::getTypeForValue(_value));	
		}
		
		template<typename T>
		void getPropertyValue(const String& name, T& _value) const
		{
			getPropertyValueImpl(name, _value, PropertyDeclaration::getTypeForValue(_value));	
		}
		
		void setValueMap(const PropertyValueMap& valueMap);
		
	protected:
		template <typename T>
		void setPropertyValueImpl(const String& name, const T *_value, PropertyType typeCheck)
		{
			PropertyBase *prop = getProperty(name);	
			BOOST_ASSERT(prop->getValueType() == typeCheck);
			static_cast<Property<T>*>(prop)->set(_value);
		}
		
		template <typename T>
		void getPropertyValueImpl(const String& name, T& outValue, PropertyType typeCheck) const
		{
			PropertyBase *prop = getProperty(name);
			BOOST_ASSERT(prop->getValueType() == typeCheck);
			outValue = static_cast<Property<T>*>(prop)->get();
		}
		
	protected:
		PropertyEntriesMap			m_propertyEntriesMap;
		PropertyEntriesVector		m_propertyEntriesVec;
	
		// property listeners	
		PropertySetListenerVec		m_propertySetListeners;
	};

}



#endif