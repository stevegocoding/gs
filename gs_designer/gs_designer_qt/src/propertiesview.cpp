#include "propertiesview.hxx"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"

using namespace gs_designer;

PropertiesViewWidget::PropertiesViewWidget(QWidget *parent)
: QWidget(parent)
{
	/** property manager */
	groupManager = new QtGroupPropertyManager(this);
	boolManager = new QtBoolPropertyManager(this);
	intManager = new QtIntPropertyManager(this);
	doubleManager = new QtDoublePropertyManager(this);
	stringManager = new QtStringPropertyManager(this);
	enumManager = new QtEnumPropertyManager(this);
	colourManager = new QtColorPropertyManager(this);
	
	QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this); 
	QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
	QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
	QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);
	QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);
	QtColorEditorFactory *colourEditFactory = new QtColorEditorFactory(this);
	
	propertiesWidget = new GsTreePropertyBrowser();
	propertiesWidget->setFactoryForManager(boolManager, checkBoxFactory);
	propertiesWidget->setFactoryForManager(intManager, spinBoxFactory);		
	propertiesWidget->setFactoryForManager(doubleManager, doubleSpinBoxFactory); 
	propertiesWidget->setFactoryForManager(enumManager, comboBoxFactory);
	propertiesWidget->setFactoryForManager(stringManager, lineEditFactory);
	propertiesWidget->setFactoryForManager(colourManager, colourEditFactory);
	propertiesWidget->setAlternatingRowColors(true);
	propertiesWidget->setIndentation(10);
	propertiesWidget->setResizeMode(QtTreePropertyBrowser::Interactive);

	descLabel = new QLabel(this);
	descLabel->setFixedHeight(48);
	descLabel->setWordWrap(true);
	descLabel->setMargin(4);
	descLabel->setStyleSheet("background-color: #AFAFAF;");		
	
	QVBoxLayout *boxlayout = new QVBoxLayout();
	boxlayout->setMargin(0);
	boxlayout->addWidget(propertiesWidget);
	boxlayout->addWidget(descLabel);
	boxlayout->setStretch(0, 1);
	boxlayout->setStretch(1, 0);
	boxlayout->setSpacing(0);
	setLayout(boxlayout);

	m_qtToGsPropertyMap.clear(); 
	m_gsToQtPropertyMap.clear();
	
	/** item changed */
	connect(propertiesWidget, SIGNAL(currentItemChanged(QtBrowserItem*)), this, SLOT(itemChanged(QtBrowserItem*)));

	/** test */
	//QtProperty *prop = intManager->addProperty("Test Property");
	//intManager->setValue(prop, 99);
	//propertiesWidget->addProperty(prop); 
}

void PropertiesViewWidget::createProperty(QtProperty *group, QString name, PropertyBaseSharedPtr prop)
{
	PropertyDeclSharedPtr def = prop->getDecl();				
	QtProperty *newProp = 0; 
	
	PropertyOptionsVector *options = def->getOptions();	
	if (options->size() != 0)
	{
		newProp = enumManager->addProperty(name);
		int choiceIndex = -1;
		String curOptionKey = prop->getOptionNameIndex(choiceIndex);
		QStringList choices;
		
		my_foreach(PropertyOption option, *options)
		{
			choices.push_back(QString(option.m_key.c_str()));
		}
		enumManager->setEnumNames(newProp, choices);
		enumManager->setValue(newProp, choiceIndex);
	}
	else
	{
		switch(def->getValueType())
		{
		case PROP_BOOL:
			newProp = boolManager->addProperty(name);
			boolManager->setValue(newProp, dynamic_pointer_cast<Property<bool>>(prop)->get());
			break;
		case PROP_SHORT:
			newProp = intManager->addProperty(name);
			intManager->setValue(newProp, dynamic_pointer_cast<Property<short>>(prop)->get());
			break;
		case PROP_UNSIGNED_SHORT:
			newProp = intManager->addProperty(name);
			intManager->setValue(newProp, dynamic_pointer_cast<Property<unsigned short>>(prop)->get());
			break;
		case PROP_INT:
			newProp = intManager->addProperty(name);
			intManager->setValue(newProp, dynamic_pointer_cast<Property<int>>(prop)->get());
			break;
		case PROP_UNSIGNED_INT:
			newProp = intManager->addProperty(name);
			intManager->setValue(newProp, dynamic_pointer_cast<Property<unsigned int>>(prop)->get());
			break;
		case PROP_LONG:
			newProp = intManager->addProperty(name);
			intManager->setValue(newProp, dynamic_pointer_cast<Property<long>>(prop)->get());
			break;
		case PROP_UNSIGNED_LONG:
			newProp = intManager->addProperty(name);
			intManager->setValue(newProp, dynamic_pointer_cast<Property<unsigned long>>(prop)->get());
			break;
		case PROP_REAL:
			newProp = doubleManager->addProperty(name);
			doubleManager->setValue(newProp, dynamic_pointer_cast<Property<Real>>(prop)->get());
			break;
		case PROP_STRING:
			newProp = stringManager->addProperty(name);
			stringManager->setValue(newProp, dynamic_pointer_cast<Property<String>>(prop)->get().c_str());
			break;
		
	/*	case PROP_VECTOR2:
			newProp = vector2Manager->addProperty(QVariant::PointF, name);
			fieldX = propDef->fieldName(0).c_str();
			fieldY = propDef->fieldName(1).c_str();
			vector2Manager->setPropertyNames(newProp, fieldX, fieldY);
			val2 = static_cast<OgitorsProperty<Ogre::Vector2>*>(prop)->get();
			vpf = QPointF(val2.x, val2.y);
			vector2Manager->setValue(newProp, vpf);
			break;
		case PROP_VECTOR3:
			newProp = vector3Manager->addProperty(QVariant::RectF, name);
			fieldX = propDef->fieldName(0).c_str();
			fieldY = propDef->fieldName(1).c_str();
			fieldZ = propDef->fieldName(2).c_str();
			vector3Manager->setPropertyNames(newProp, fieldX, fieldY, fieldZ);
			val3 = static_cast<OgitorsProperty<Ogre::Vector3>*>(prop)->get();
			vrf = QRectF(val3.x, val3.y, val3.z, 0);
			vector3Manager->setValue(newProp, vrf);
			break;
		case PROP_VECTOR4:
			newProp = vector4Manager->addProperty(QVariant::RectF, name);
			fieldX = propDef->fieldName(0).c_str();
			fieldY = propDef->fieldName(1).c_str();
			fieldZ = propDef->fieldName(2).c_str();
			fieldW = propDef->fieldName(3).c_str();
			vector4Manager->setPropertyNames(newProp, fieldX, fieldY, fieldZ, fieldW);
			val4 = static_cast<OgitorsProperty<Ogre::Vector4>*>(prop)->get();
			vrf = QRectF(val4.x, val4.y, val4.z, val4.w);
			vector4Manager->setValue(newProp, vrf);
			break;
		case PROP_COLOUR:
			newProp = colourManager->addProperty(name);
			valc = static_cast<OgitorsProperty<Ogre::ColourValue>*>(prop)->get();
			col = QColor(valc.r * 255.0f, valc.g * 255.0f, valc.b * 255.0f, valc.a * 255.0f);
			colourManager->setValue(newProp, col);
			break;

		case PROP_QUATERNION:
			newProp = quaternionManager->addProperty(QVariant::RectF, name);
			valQ = static_cast<OgitorsProperty<Ogre::Quaternion>*>(prop)->get();
			vrf = QRectF(valQ.x, valQ.y, valQ.z, valQ.w);
			quaternionManager->setValue(newProp, vrf);
			break;*/
		}
		
		if (newProp)
		{
			m_qtToGsPropertyMap.insert(QtToGsDesignerPropertyMap::value_type(newProp, prop));	
			m_gsToQtPropertyMap.insert(GsDesignerToQtPropertyMap::value_type(prop, newProp));
			
			if (group)
				group->addSubProperty(newProp);
			else 
			{
				propertiesWidget->addProperty(newProp);
			}
		}
	}
}

void PropertiesViewWidget::presentPropertiesView(BaseEditorSharedPtr obj)
{
	propertiesWidget->clear();
	boolManager->clear();
	intManager->clear();
	doubleManager->clear();
	stringManager->clear();
	enumManager->clear();
	colourManager->clear();
	//quaternionManager->clear();
	//vector2Manager->clear();
	//vector3Manager->clear();
	//vector4Manager->clear();
	
	m_qtToGsPropertyMap.clear();
	m_gsToQtPropertyMap.clear(); 
}

void PropertiesViewWidget::displayObjectProperties(PropertyBaseSharedPtr prop)
{
		
}

void PropertiesViewWidget::itemChanged(QtBrowserItem *item)
{

}

void PropertiesViewWidget::onPropertyChanged(PropertySet *set, PropertyBaseSharedPtr prop)
{

}

void PropertiesViewWidget::onPropertyAdded(PropertySet *set, PropertyBaseSharedPtr prop)
{

}

void PropertiesViewWidget::onPropertyRemoved(PropertySet *set, PropertyBaseSharedPtr prop)
{
	int a = 0;	
}


