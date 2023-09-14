#pragma once

#include "XObject.h"

namespace CoreBase
{

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  XObjectFactory
// Description: XObjectFactory is a class in the CoreBase namespace that
// provides functionality for registering and creating instances of
// classes derived from XObject.
//	The XObjectFactory class has several static methods,
//	including registerClassObject and newObject, which allow
//	developers to register new objects and create instances of them.
//	registerClassObject takes a pointer to an XObject object and adds
//	it to a map of registered objects.Similarly, newObject creates a
//	new instance of the specified class name or class name and QUuid
//	index and returns it as an XObject * .
//  In summary, the XObjectFactory class is responsible for managing
//  object creation and registration within the CoreBase library.
//----------------------------------------------------------------------------- 
class COREBASE_EXPORT XObjectFactory : public QObject
{
	Q_OBJECT
public:
	XObjectFactory();
	virtual ~XObjectFactory();

	static void registerClassObjects();
	template <class T> static T* registerClassObject();
	static void registerClassObject(XObject* object);

	static XObject* newObject(const QString&classname);
	static XObject* newObject(const QString& classname, const QUuid& idx);

	static void clear();
private:
	static QMap<QString,QPointer<XObject> > m_Objects;
};

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectFactory
// Method:  registerClassObject
// Params:  
//----------------------------------------------------------------------------- 

template <class T>
T* XObjectFactory::registerClassObject()
{
	T* obj = new T();
	registerClassObject(obj);
	return obj;
}

};
using namespace CoreBase;
