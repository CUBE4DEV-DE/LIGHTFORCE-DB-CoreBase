#include "XObjectFactory.h"

#include "XSystem.h"
#include "XUser.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  m_ChildObjects
//----------------------------------------------------------------------------- 
QMap<QString, QPointer<XObject> > XObjectFactory::m_Objects;

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  XObjectFactory
//----------------------------------------------------------------------------- 
XObjectFactory::XObjectFactory(): QObject()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  ~XObjectFactory
//----------------------------------------------------------------------------- 
XObjectFactory::~XObjectFactory()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  registerClassObjects
//----------------------------------------------------------------------------- 
void XObjectFactory::registerClassObjects()
{
	registerClassObject<XUser>();
	registerClassObject<XTranslate>();
	registerClassObject<XSystem>();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  registerClassObject
// Params:  XObject* object
//----------------------------------------------------------------------------- 
void XObjectFactory::registerClassObject(XObject* object)
{
	if (object)
	{
		QString classname=object->className();
		if (!m_Objects.contains(classname))
			m_Objects[classname] = object;
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  newObject
// Params:  const QString& classname
//----------------------------------------------------------------------------- 
XObject* XObjectFactory::newObject(const QString& classname)
{
	if (m_Objects.contains(classname)&& m_Objects[classname])
	{
		XObject* obj = m_Objects[classname]->clone();
		if (obj)
		{
			//obj->setNULL("parent");
			obj->newIndex();
			return obj;
		}
	}
	return Q_NULLPTR;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  newObject
// Params:  const QString& classname
// Params:  const QUuid& idx
//----------------------------------------------------------------------------- 
XObject* XObjectFactory::newObject(const QString& classname,const QUuid& idx)
{
	if (m_Objects.contains(classname) && m_Objects[classname])
	{
		XObject* obj=m_Objects[classname]->clone();
		if (obj)
		{
			//obj->setNULL("parent");
			obj->setIndex(idx);
			return obj;
		}
	}
	return Q_NULLPTR;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  clear
//----------------------------------------------------------------------------- 

void XObjectFactory::clear()
{
	for(XObject*obj:m_Objects.values())
	{
		if (obj)
			delete obj;
	}
	m_Objects.clear();
}
