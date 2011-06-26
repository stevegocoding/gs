#ifndef __PROPERTIES_VIEW_H__
#define __PROPERTIES_VIEW_H__

#define QT_NO_KEYWORDS

#include <QtGui/QWidget>
#include <Qtgui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMenu>
#include <QtGui/QIcon>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QSignalMapper>
#include <map>

#include "qttreepropertybrowser.h"
#include "gsProperty.h"

class QtTreePropertyBrowser;
class QtGroupPropertyManager;
class QtBoolPropertyManager;
class QtIntPropertyManager;
class QtDoublePropertyManager;
class QtStringPropertyManager;
class QtEnumPropertyManager;
class QtColorPropertyManager;
class QuaternionManager;
class QtProperty;

typedef std::map<QtProperty*, gs_designer::PropertyBaseSharedPtr> QtToGsDesignerPropertyMap;
typedef std::map<gs_designer::PropertyBaseSharedPtr, QtProperty*> GsDesignerToQtPropertyMap;

class GsTreePropertyBrowser : public QtTreePropertyBrowser
{
public:
	GsTreePropertyBrowser(QWidget *parent = 0)
		: QtTreePropertyBrowser(parent) {}
	virtual ~GsTreePropertyBrowser() {}

public:
	void contextMenu(int id) {}
	
protected:
	void contextMenuEvent(QContextMenuEvent *event) {}
};

class PropertiesViewWidget : public QWidget, public gs_designer::PropertySetListener
{
	Q_OBJECT
		
public:
	explicit PropertiesViewWidget(QWidget *parent = 0);
	virtual ~PropertiesViewWidget() {}
		
	/** show the properties of an object in the property view */
	void presentPropertiesView(gs_designer::BaseEditorSharedPtr obj);
	
public Q_SLOTS:
	/** current item changed */
	void itemChanged(QtBrowserItem *item);
	
protected:
	void createProperty(QtProperty *group, QString name, gs_designer::PropertyBaseSharedPtr prop);	
	void displayObjectProperties(gs_designer::PropertyBaseSharedPtr prop);
	
	/** implementation of PropertySetListener */
	void onPropertyAdded(gs_designer::PropertySet *set, gs_designer::PropertyBaseSharedPtr prop);
	void onPropertyChanged(gs_designer::PropertySet *set, gs_designer::PropertyBaseSharedPtr prop);
	void onPropertyRemoved(gs_designer::PropertySet *set, gs_designer::PropertyBaseSharedPtr prop);
	
protected:
	GsTreePropertyBrowser	*propertiesWidget; 
	QLabel					*descLabel;
	
	/** managers */
	QtGroupPropertyManager	*groupManager; 
	QtBoolPropertyManager	*boolManager;
	QtColorPropertyManager	*colourManager;
	QtIntPropertyManager	*intManager;
	QtDoublePropertyManager *doubleManager;
	QtStringPropertyManager *stringManager; 
	QtEnumPropertyManager	*enumManager; 

	QtToGsDesignerPropertyMap m_qtToGsPropertyMap; 
	GsDesignerToQtPropertyMap m_gsToQtPropertyMap;

	/** editor object */
	gs_designer::BaseEditorSharedPtr		m_lastObject;
				
};



#endif