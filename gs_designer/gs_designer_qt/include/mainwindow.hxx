#ifndef __MAIN_WINDOW_HXX__
#define __MAIN_WINDOW_HXX__

#define QT_NO_KEYWORDS

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "gsDesignerDef.h"
#include "gsDesignerPrerequisites.h"

class PropertiesViewWidget;
class OgreWidget;
class OgreWidgetDebug;

class MainWindow : public QMainWindow
{
	Q_OBJECT
		
public:
	MainWindow();
	~MainWindow();

	// add dock widgets */
	void addDockWidgets(QMainWindow *parent); 
	void addActions();
	void addMenus();
	void setupStatusBar();
	void retranslateUi();
	void createSceneRenderWidget();

	// Widgets */
	QTabWidget			*m_editorViewTab;				// Editor View Widgets  
		// OgreWidget		*m_ogreWidget;					// Ogre rendering widget
	OgreWidgetDebug			*m_ogreWidget; 
		
	//////////////////////////////////////////////////////////////////////////
	// Actions */
	// File Menu */
	QAction *actNew;							// menu new */
	QAction *actOpen; 
	QAction *actClose;
	QAction *actSave;
	QAction *actSaveAs;
	QAction *actExit;
	
	// Edit */
	QAction	*actUndo;
	QAction *actRedo;
	QAction *actEditCopy;
	QAction *actEditCut;
	QAction *actEditDelete;
	QAction *actEditRename;
	QAction *actEditPaste;
	
	// Tools */
	QAction *actSelect;
	QAction *actMove;
	QAction *actRotate;
	QAction *actScale;	

	// Views */
	QAction *actToggleGrid;
	QAction *actToggleToolBar;
	QAction *actToggleExplorer;
	QAction *actToggleTools;
	QAction *actToggleProperties;
	QAction *actToggleResources;
	QAction *actToggleLog;
	QAction *actFullScreen;	
	
	// Helps */
	QAction *actHelp;
	QAction *actAbout;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Menus */
	QMenuBar *menuBar; 
	QMenu *menuFile;
	QMenu *menuImport;
	QMenu *menuExport;
	QMenu *menuTools;
	QMenu *menuEdit; 
	QMenu *menuView;
	QMenu *menuHelp;
	//////////////////////////////////////////////////////////////////////////
			
	//////////////////////////////////////////////////////////////////////////
	// ToolBars */
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Status Bar */
	QStatusBar *statusBar;
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// Dock Widgets */
	QDockWidget *explorerDockWidget;
	QDockWidget *resourcesDockWidget;
	QDockWidget *propertiesDockWidget;
	QDockWidget *toolsDockWidget;
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// Properties Widgets */
	PropertiesViewWidget *propertiesViewWidget;
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// Tool Box */
	QToolBox *explorerToolBox; 
	QToolBox *propertiesToolBox;
	QToolBox *resourcesToolBox;
	QToolBox *toolsToolBox;
	//////////////////////////////////////////////////////////////////////////

public Q_SLOTS:
	
	void newScene();

protected:
	void closeEvent(QCloseEvent *event); 

};

extern MainWindow *g_DesignerMainWnd;


#endif