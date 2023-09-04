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
