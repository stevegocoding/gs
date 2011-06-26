#include "mainwindow.hxx"
#include "propertiesview.hxx"
#include "ogrewidgetdebug.hxx"

#include "gsDesigner.h"

using namespace gs_designer;

MainWindow *g_DesignerMainWnd = NULL;
MainWindow::MainWindow()
{
	setDockNestingEnabled(true);

	// add dock widgets */
	addDockWidgets(this);
	
	// add actions
	addActions();
	
	// add menus
	addMenus();
	
	// re-translate UI
	retranslateUi();
	
	// add tab widget
	m_editorViewTab = new QTabWidget(this);
	m_editorViewTab->setObjectName("editorViewTab");

	// create render widget
	createSceneRenderWidget();

	setCentralWidget(m_editorViewTab);
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::addDockWidgets(QMainWindow *parent)
{
	// explorer dock widget */
	explorerToolBox = new QToolBox(parent);
	explorerDockWidget = new QDockWidget(parent);
	explorerDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	explorerDockWidget->setObjectName(QString::fromUtf8("explorerDockWidget"));
	explorerDockWidget->setWidget(explorerToolBox);
	parent->addDockWidget(Qt::LeftDockWidgetArea, explorerDockWidget); 
		
	// properties widgets */
	propertiesViewWidget = new PropertiesViewWidget(parent);
	
	// properties dock widgets */
	propertiesToolBox = new QToolBox(parent);
	propertiesToolBox->addItem(propertiesViewWidget, tr("General") + " " + tr("Properties"));
	propertiesDockWidget = new QDockWidget(parent);
	propertiesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	propertiesDockWidget->setObjectName(QString::fromUtf8("propertiesDockWidget"));
	propertiesDockWidget->setWidget(propertiesToolBox);
	parent->addDockWidget(Qt::RightDockWidgetArea, propertiesDockWidget);
	
	// resources dock widgets */
	resourcesDockWidget = new QDockWidget(parent);
	resourcesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	resourcesDockWidget->setObjectName(QString::fromUtf8("resourcesDockWidget"));
	// resourcesDockWidget->setWidget(mResourcesToolBox);
	parent->addDockWidget(static_cast<Qt::DockWidgetArea>(2), resourcesDockWidget);

	toolsDockWidget = new QDockWidget(parent);
	toolsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	toolsDockWidget->setObjectName(QString::fromUtf8("toolsDockWidget"));
	parent->addDockWidget(static_cast<Qt::DockWidgetArea>(2), toolsDockWidget);
	
	parent->tabifyDockWidget(propertiesDockWidget, resourcesDockWidget);
	parent->tabifyDockWidget(propertiesDockWidget, toolsDockWidget);
	propertiesDockWidget->raise();
	explorerDockWidget->raise();
}



void MainWindow::addMenus()
{
	menuBar = new QMenuBar(this); 
	menuBar->setObjectName(QString::fromUtf8("menuBar"));
	menuBar->setGeometry(QRect(0, 0, 616, 22));
	
	// File Menu */	
	menuFile = new QMenu(tr("File"), menuBar); 
	menuBar->addAction(menuFile->menuAction());
	menuFile->addAction(actNew);
	menuFile->addAction(actOpen);
	menuFile->addAction(actSave);
	menuFile->addAction(actSaveAs);
	menuFile->addAction(actClose);
	menuFile->addSeparator();
	menuImport = menuFile->addMenu(tr("Import"));
	menuExport = menuFile->addMenu(tr("Export"));
	menuFile->addSeparator();
	menuFile->addAction(actExit);
	
	// Edit Menu */
	menuEdit = new QMenu(tr("Edit"), menuBar);
	menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
	menuBar->addAction(menuEdit->menuAction());
	menuEdit->addAction(actUndo);
	menuEdit->addAction(actRedo);
	menuEdit->addSeparator();
	menuEdit->addAction(actEditCopy);
	menuEdit->addAction(actEditCut);
	menuEdit->addAction(actEditPaste);
	menuEdit->addAction(actEditDelete);
	menuEdit->addAction(actEditRename);
	
	// Tools Menu  */		
	menuTools = new QMenu(tr("Tools"), menuBar);
	menuTools->setObjectName(QString::fromUtf8("menuTools"));
	menuBar->addAction(menuTools->menuAction());
	menuTools->addAction(actSelect);
	menuTools->addAction(actMove);
	menuTools->addAction(actRotate);
	menuTools->addAction(actScale);
	//menuDefineSelectionList = menuTools->addMenu(tr("Define Selection List"));
	//menuSelectSelectionList = menuTools->addMenu(tr("Activate Selection List"));
	//for(int sa = 0;sa < 10;sa++)
	//{
	//	menuDefineSelectionList->addAction(mSelectActions[sa]);
	//	menuSelectSelectionList->addAction(mSelectActions[sa + 10]);
	//}
	
	// Views Menu */
	menuView = new QMenu(tr("View"),menuBar);
	menuView->setObjectName(QString::fromUtf8("menuView"));
	menuBar->addAction(menuView->menuAction());
	menuView->addAction(actToggleExplorer);
	menuView->addAction(actToggleProperties);
	menuView->addAction(actToggleTools);
	menuView->addAction(actToggleResources);
	//menuView->addAction(actToggleLog);
	menuView->addSeparator();
	menuView->addAction(actToggleGrid);
	menuView->addAction(actToggleToolBar);
	menuView->addAction(actFullScreen);
	
	// Help Menu  */
	menuHelp = new QMenu(tr("Help"), menuBar);
	menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
	menuBar->addAction(menuHelp->menuAction());
	menuHelp->addAction(actHelp);
	menuHelp->addAction(actAbout);
	
	setMenuBar(menuBar);
}

void MainWindow::retranslateUi()
{
	QString appTitle = "Golf Course Designer ";
	appTitle += GS_DESIGNER_VERSION;    
	setWindowTitle(appTitle);
	explorerDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Explorer", 0, QApplication::UnicodeUTF8));
	// layerDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Groups", 0, QApplication::UnicodeUTF8));
	resourcesDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Resources", 0, QApplication::UnicodeUTF8));
	propertiesDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Properties", 0, QApplication::UnicodeUTF8));
	toolsDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
	menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
}

void MainWindow::createSceneRenderWidget()
{
	QWidget *renderWindowWidget = new QWidget(m_editorViewTab);
	
	QToolBar *designerViewToolBar = new QToolBar(renderWindowWidget);
	designerViewToolBar->setObjectName("designerViewToolBar");
	designerViewToolBar->setIconSize(QSize(20,20));
	designerViewToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
	designerViewToolBar->addSeparator();
	designerViewToolBar->addAction(actSelect);
	designerViewToolBar->addAction(actMove);
	designerViewToolBar->addAction(actRotate);
	designerViewToolBar->addAction(actScale);
	
	// create custom view widget */
	m_ogreWidget = new OgreWidgetDebug(renderWindowWidget);

	// setup layout */
	QVBoxLayout *renderWindowLayout = new QVBoxLayout();
	renderWindowLayout->setSpacing(0);
	renderWindowLayout->setMargin(0);
	renderWindowLayout->addWidget(designerViewToolBar, 0);
	renderWindowLayout->addWidget(m_ogreWidget, 1);
	
	renderWindowWidget->setLayout(renderWindowLayout);

	// add a tab
	m_editorViewTab->addTab(renderWindowWidget, "Layout");		
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (GetRootSingleton()->terminateScene())	
	{

	}
}

