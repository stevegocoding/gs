#ifndef __GS_CMAKE_TEST_H__
#define __GS_CMAKE_TEST_H__

#include <iostream>
#include "OgrePrerequisites.h"

using namespace std;

class Ogre::Root; 
namespace gs
{
	 class __declspec(dllexport) Test
	{
	public:
		Test(int x = 0);
		~Test();
		
	private:
		int m_value;
		Ogre::Root *m_root;
		Ogre::String m_pluginCfg;
	};
}



#endif