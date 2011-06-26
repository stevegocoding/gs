#include "gsProperty.h"

#include "Ogre.h"

namespace gs_designer
{
	PropertySet::PropertySet()
	{

	}
	
	void PropertySet::addProperty(PropertyBaseSharedPtr prop)
	{
		BOOST_ASSERT(prop != NULL);	
		m_propertyEntriesMap.insert(PropertyEntriesMap::value_type(prop->getName(), prop));
		m_propertyEntriesVec.push_back(prop);
		
		// notify property added
		my_foreach(PropertySetListener *listener, m_propertySetListeners)
			listener->onPropertyAdded(this, prop);	
	}
	
	PropertyBaseSharedPtr PropertySet::getProperty(const String& name) const
	{
		ConstPropIterator it = m_propertyEntriesMap.find(name); 
		BOOST_ASSERT(it != m_propertyEntriesMap.end());
		return it->second; 
	}
	
	bool PropertySet::hasProperty(const String& name) const
	{
		ConstPropIterator it = m_propertyEntriesMap.find(name); 
		return (it == m_propertyEntriesMap.end());
	}
	
	void PropertySet::initValueMap(const PropertyValueMap& _values)
	{
#define INIT_PROPERTY_VALUE(_type)\
		{\
			const boost::shared_ptr<Property<_type> > ptr = dynamic_pointer_cast<Property<_type> >(propEntryIt->second);\
			ptr->init(get_any_value<_type>(i->second.val));\
		}\
		
		for (PropertyValueMap::const_iterator i = _values.begin(); i != _values.end(); ++i) 
		{
			PropertyEntriesMap::const_iterator propEntryIt = m_propertyEntriesMap.find(i->first);
			if (propEntryIt != m_propertyEntriesMap.end())		
			{
				/** check if the type matched */
				BOOST_ASSERT(propEntryIt->second->getValueType() == i->second.type);
				switch(propEntryIt->second->getValueType())
				{
				case PROP_SHORT: 
					INIT_PROPERTY_VALUE(short);
					break;
				case PROP_UNSIGNED_SHORT:
					INIT_PROPERTY_VALUE(unsigned short); 
					break;
				case PROP_INT:
					INIT_PROPERTY_VALUE(int);
					break;
				case PROP_UNSIGNED_INT:
					INIT_PROPERTY_VALUE(unsigned int);
					break;
				case PROP_LONG:
					INIT_PROPERTY_VALUE(long); 
					break;
				case PROP_UNSIGNED_LONG:
					INIT_PROPERTY_VALUE(unsigned long);
					break;
				case PROP_REAL:
					INIT_PROPERTY_VALUE(Real);
					break;
				case PROP_STRING:
					INIT_PROPERTY_VALUE(String);
					break;
				case PROP_BOOL:
					INIT_PROPERTY_VALUE(bool);
					break;
				case PROP_VECTOR2:
					INIT_PROPERTY_VALUE(Ogre::Vector2);
					break;
				case PROP_VECTOR3:
					INIT_PROPERTY_VALUE(Ogre::Vector3);
					break;
				case PROP_VECTOR4:
					INIT_PROPERTY_VALUE(Ogre::Vector4);
					break;
				case PROP_COLOUR:
					INIT_PROPERTY_VALUE(Ogre::ColourValue)	
					break;
				case PROP_QUATERNION:
					INIT_PROPERTY_VALUE(Ogre::Quaternion);
					break;
				case PROP_MATRIX3:
					INIT_PROPERTY_VALUE(Ogre::Matrix3);	
					break;
				case PROP_MATRIX4:
					INIT_PROPERTY_VALUE(Ogre::Matrix4);	
					break;
				}
			}
		}
	}
	
	void PropertySet::setValueMap(const PropertyValueMap& valueMap)
	{
			
	}
	
	//PropIterator PropertySet::getPropertyIterator()
	//{
	//	return PropIterator(m_propertyEntriesMap.begin(), m_propertyEntriesMap.end());
	//}
	//
	//ConstPropIterator PropertySet::getConstPropertyIterator() const
	//{
	//	return ConstPropIterator(m_propertyEntriesMap.begin(), m_propertyEntriesMap.end());
	//}
	 
	void PropertySet::addListener(PropertySetListener *listener)
	{
		BOOST_ASSERT(listener != NULL);	
		m_propertySetListeners.push_back(listener);
	}

}