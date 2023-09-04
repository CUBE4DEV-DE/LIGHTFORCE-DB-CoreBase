#pragma once

#include "XObject.h"
#include "XObjectManager.h"
#include "XLinq.h"

#include <QtConcurrent>

#define XOBJECTLIST(XCLASS,NAME)	public:\
	XObjectList<##XCLASS>& ##NAME() { return m_##NAME; }\
	QList<QPointer<##XCLASS> > objects##NAME() { return m_##NAME.objects(); }\
	auto from##XCLASS() { return from(##NAME().objects()); }\
	private:\
	XObjectList<##XCLASS> m_##NAME;

//#define XOBJECTLISTWHERE(NAME,FILTERNAME,FILTER)	public:\
//	auto ##FILTERNAME() { return from(##NAME().objects()).where(FILTER); } }

namespace CoreBase
{
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    03.04.2022
// Context: DefaultNamespace
// Class:   XCoreObjectList
// Method:  XObjectList
//----------------------------------------------------------------------------- 
template <class T> class XObjectList:public XObject
{
public:
	XObjectList() {}
	virtual ~XObjectList() {}

	T* newObject();
	T* newObject(const QUuid& idx);
	bool deleteObject(T* obj); 
	bool deleteObject(const QUuid& idx);

	virtual void clearAllObjects()
	{
		if (objectManager())
		{
			QWriteLocker lock(&m_ListLock);

			QList<QPointer<XObject> > list;
			for (T* obj : m_ChildObjects)
			{
				if (obj)
				{
					m_ChildObjects.removeAll(obj);
					m_IndexChildObjects.remove(obj->index());
					objectManager()->deleteObject(obj);
				}
			}
		}
	}


	T* find(const QUuid& idx);

	// Cast
public:
	inline T* castChild(XObject* obj) const
	{
		return static_cast<T*>(obj);
	}

// Meta
public:
	virtual const QString& childClassName() const;
private:
	mutable QString m_ChildClassName;

// Child Objects
public:
	QList<QPointer<T> >& objects() { return  m_ChildObjects; }

// Child Objects Store
public:
	virtual QList<QPointer<XObject> > childObjects() const;
	virtual XObject* newChildObject(const QUuid& idx);
private:
	QList<QPointer<T> > m_ChildObjects;
	QMap<QUuid, QPointer<T> > m_IndexChildObjects;
	mutable QReadWriteLock m_ListLock;
};

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectList
// Method:  newObject
//----------------------------------------------------------------------------- 

template <class T>
T* XObjectList<T>::newObject()
{
	if (objectManager())
	{
		QWriteLocker lock(&m_ListLock);
		T*obj= cast<T>(objectManager()->newObject(childClassName()));
		if (obj)
		{
			obj->setObjectParent(objectParentList());
			m_ChildObjects.append(obj);
			m_IndexChildObjects[obj->index()] = obj;

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
// Class:   XObjectList
// Method:  newObject
// Params:  const QUuid& idx
//----------------------------------------------------------------------------- 

template <class T>
T* XObjectList<T>::newObject(const QUuid& idx)
{
	if (objectManager())
	{
		QWriteLocker lock(&m_ListLock);

		T* obj = cast<T>(objectManager()->newObject(childClassName(),idx));
		if (obj)
		{
			obj->setObjectParent(objectParentList());
			m_ChildObjects.append(obj);
			m_IndexChildObjects[obj->index()] = obj;

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
// Class:   XObjectList
// Method:  deleteObject
// Params:  T* obj
//----------------------------------------------------------------------------- 

template <class T>
bool XObjectList<T>::deleteObject(T* obj)
{
	if (objectManager()&&obj)
	{
		QWriteLocker lock(&m_ListLock);
		m_ChildObjects.removeAll(obj);
		m_IndexChildObjects.remove(obj->index());
		return objectManager()->deleteObject(obj);
	}
	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectList
// Method:  deleteObject
// Params:  const QUuid& idx
//----------------------------------------------------------------------------- 

template <class T>
bool XObjectList<T>::deleteObject(const QUuid& idx)
{
	if (objectManager())
	{
		QWriteLocker lock(&m_ListLock);
		if (m_IndexChildObjects.contains(idx))
		{
			T* obj = m_IndexChildObjects[idx];
			m_ChildObjects.removeAll(obj);
			m_IndexChildObjects.remove(idx);
			return objectManager()->deleteObject(obj);
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectList
// Method:  find
// Params:  const QUuid& idx
//----------------------------------------------------------------------------- 
template <class T>
T* XObjectList<T>::find(const QUuid& idx)
{
	QReadLocker lock(&m_ListLock);

	if (m_IndexChildObjects.contains(idx))
		return m_IndexChildObjects[idx];
	return Q_NULLPTR;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectList
// Method:  childClassName
//----------------------------------------------------------------------------- 

template <class T>
const QString& XObjectList<T>::childClassName() const
{
	if (m_ChildClassName.isEmpty())
	{
		T t;
		m_ChildClassName = t.className();
	}
	return m_ChildClassName;
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectList
// Method:  childObjects
//----------------------------------------------------------------------------- 
template <class T>
QList<QPointer<XObject>> XObjectList<T>::childObjects() const
{
	QReadLocker lock(&m_ListLock);

	QList<QPointer<XObject> > list;
	for(T*obj:m_ChildObjects)
	{
		list.append((XObject*)obj);
	}
	return list;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectList
// Method:  newChildObject
// Params:  const QString& classname
// Params:  const QUuid& idx
//----------------------------------------------------------------------------- 

template <class T>
XObject* XObjectList<T>::newChildObject(const QUuid& idx)
{
	return newObject(idx);
}

};
using namespace CoreBase;
