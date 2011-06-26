/** DesignerCore headers */
#include "gsDesignerRoot.h"

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtCore/QSettings>
#include "mainwindow.hxx"

/** Ogre headers */
#include "OgreRoot.h"
#include "OgreConfigFile.h"

#include "gsSurfaceEditor.h"
#include "gsCourseEditor.h"

/** Global Objects */ 
Ogre::Root *g_OgreRoot;
gs_designer::DesignerRoot *g_DesignerRoot;
Ogre::String ResourcePath = ""; 

// @TODO : more sophisicated way of parsing configuration file 
void SetupOGRE(Ogre::String plugins, Ogre::String config, Ogre::String log)
{
	// create Ogre::Root
	g_OgreRoot = OGRE_NEW Ogre::Root(plugins);

	// setup Ogre::LogManager
	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_NORMAL); 

	Ogre::ConfigFile cf;
	std::string cfPath = ResourcePath.append("resources_d.cfg");
	cf.load(cfPath);
	
	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String secName, typeName, archName; 
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			// Add resources locations
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	// create the render system
#if ((OGRE_VERSION_MAJOR * 10) + OGRE_VERSION_MINOR) >= 17
	const Ogre::RenderSystemList& rl = g_OgreRoot->getAvailableRenderers();
	
	Ogre::RenderSystemList::const_iterator pRend = g_OgreRoot->getAvailableRenderers().begin();
	while (pRend != g_OgreRoot->getAvailableRenderers().end())
#else
	Ogre::RenderSystemList::const_iterator pRend = g_OgreRoot->getAvailableRenderers()->begin();
	while (pRend != g_OgreRoot->getAvailableRenderers()->end())
#endif	
	{
		Ogre::String rName = (*pRend)->getName();
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (rName == "Direct3D9 Rendering Subsystem")
#else
		if (rName == "OpenGL Rendering Subsystem")
#endif
			break;
		pRend++;
	}
	Ogre::RenderSystem *renderSys = *pRend;
	
	/*
	QSettings settings;
	settings.beginGroup("preferences"); 
		int antialias = settings.value("antiAliasing", 0).toInt();
		bool vsync = settings.value("useVSync", false).toBool();
	
	if (vsync)
		renderSys->setConfigOption("VSync", "Yes");
	else 
		renderSys->setConfigOption("VSync", "No");
	*/
	
	// dummy configuration of render system
	renderSys->setConfigOption("VSync",			"No");
	renderSys->setConfigOption("FSAA",			"No");
	renderSys->setConfigOption("Full Screen",	"No"); 
	
	// set the rendering system and initialise OGRE
	g_OgreRoot->setRenderSystem(renderSys);
	
	// initialize without creating window 
	g_OgreRoot->initialise(false);
	
	// create DesignerRoot
	// g_DesignerRoot = new gs_designer::DesignerRoot();
	g_DesignerRoot = gs_designer::GetRootSingleton();
	

}

void RegisterGameObjFactories()
{
	g_DesignerRoot->registerEditorFactory(new gs_designer::CourseEditorFactory()); 
	g_DesignerRoot->registerEditorFactory(new gs_designer::SurfaceEditorFactory()); 
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QDir::setCurrent(a.applicationDirPath());
	
	// See if we are loading something from the command line.
	/*
	QString fileArg("");
	QString suffix("");
	QString argument = a.arguments().last();
	QFileInfo info(a.arguments().last());
	if(info.exists() && info.isFile())
	{
		if(info.suffix() == "ogscene")
		{
			// Yes we've got an ogscene file to load.
			fileArg = info.absoluteFilePath();
			suffix = info.suffix();
		}
		else if(info.baseName() != "qtOgitor" && info.baseName() != "qtOgitor_d"  )
		{
			// We are trying to load something we can't load
			// Exit the application.
			QMessageBox msg(QMessageBox::Critical, "Ogitor Error", "We can only load Ogitor Scenes.\nExiting..");
			msg.exec();
			return 0;
		}
	}	
	*/
	
	a.setOrganizationName("gsDesigner");
	a.setApplicationName("gsDesignerQt");
	
	// setup OGRE
#ifdef _DEBUG
	SetupOGRE(ResourcePath + Ogre::String("plugins_d.cfg"), Ogre::String("ogre.cfg"), Ogre::String("gs_designer.log"));
#else
	SetupOGRE(ResourcePath + Ogre::String("plugins.cfg"), Ogre::String("ogre.cfg"), Ogre::String("ogitor.log"));
#endif
	
	RegisterGameObjFactories();
	
	// create main window
	g_DesignerMainWnd = new MainWindow();
	g_DesignerMainWnd->show();
	// g_DesignerMainWnd->setApplicationObject(&a);	
	
	QString sceneToLoad = "";
	
	// gs_designer::DesignerRoot::getSingleton().loadScene(sceneToLoad.toStdString());
	gs_designer::GetRootSingleton()->loadScene(sceneToLoad.toStdString());
	g_DesignerMainWnd->repaint(); 
	
	int exitCode = a.exec(); 

	//////////////////////////////////////////////////////////////////////////
	
	// Cleanup */
	delete g_DesignerMainWnd; 
	// delete g_DesignerRoot;
	OGRE_DELETE(g_OgreRoot);

	return exitCode;
}
