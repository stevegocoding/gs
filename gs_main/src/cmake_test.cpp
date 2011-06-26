#include "cmake_test.h"
#include "OgreRoot.h"

namespace gs
{
	Test::Test(int x /* = 0 */)
		: m_value(x)
		
	{
		m_pluginCfg = Ogre::StringUtil::BLANK;
		m_root = new Ogre::Root(m_pluginCfg);
	}
	
	Test::~Test()
	{
		delete m_root;
	}
}