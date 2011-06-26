#ifndef __BOOST_SUPPORT_H__
#define __BOOST_SUPPORT_H__

/** any */
#include <boost/any.hpp>

/** function */
#include <boost/function.hpp>

/** bind */
#include <boost/bind.hpp>

/** smart pointers */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/** algorithms */
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

/** singleton */
#include <boost/serialization/singleton.hpp>
using boost::serialization::singleton;

/** assert */
#include <boost/assert.hpp>


/** alias of BOOST_FOREACH */
#define my_foreach				BOOST_FOREACH
#define my_reverse_foreach		BOOST_REVERSE_FOREACH			

using namespace boost; 

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

#endif