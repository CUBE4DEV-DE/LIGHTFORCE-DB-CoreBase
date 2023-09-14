#pragma once

#include <QObject>

#include "XObject.h"

namespace CoreBase
{

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  XObjectManager
// Descriptions: The XObjectManager class is responsible for managing the
//	construction and destruction of XObject instances by generating new
//	objects of a specific class name, finding objects with a particular
//	identifier, casting to a template, and deleting objects based on either
//	UUID or object. It also provides a set of functions for locking and
//	unlocking the object for concurrent program access. Furthermore,
//	XObjectManager has additional functionality for caching and managing
//	super objects to which these XObjects are parented.
//----------------------------------------------------------------------------- 
class COREBASE_EXPORT XObjectManager //: public QObject
{
	friend class XObjectDocument;
	//Q_OBJECT
public:
	XObjectManager();
	virtual ~XObjectManager();

// Super Object
public:
	template<class T> T* newSuperObject(const QString& superobjectname);
	template<class T> T* superObject(const QString& superobjectname);
	template<class T> QList<T*> superObjects(const QString& superobjectname);
	template<class T> QList<T*> allObjects();
	void lockRead();
	void lockWrite();
	void unlock();

	static XObjectManager* objectManager(const QString& name);
	static void removeObjectManager(const QString& name);
private:
	static QMap<QString, XObjectManager* > m_ObjectManagerMap;
protected:
	void insertSuperObject(XObject* obj);
private:
	QMultiMap<QString, QPointer<XObject> > m_IndexSuperObjects;

// Memory Construction Destruction
public:
	template<class T> T* newObject();
	XObject* newObject(const QString& classname);
	XObject* newObject(const QString& classname, const QUuid& idx);
	bool deleteObject(XObject* obj);
	bool deleteObject(const QUuid& uuid);

	const QList<QPointer<XObject> > objects() { return m_IndexObjects.values(); }
private:
	QMap<QString, QPointer<XObject> > m_IndexObjects;
	mutable QReadWriteLock m_ObjectLock;
	mutable QReadWriteLock m_BaseLock;

// Cast
public:
	template <class T> static T* cast(XObject* obj);

// Find
public:
	inline XObject* findObject(const QUuid& uuid) const;
	template <class T> T* find(const QUuid& uuid);
	template <class T> T* find(const QString& id);

// Store
public:
	bool open(const QString& path, const QString& filename, bool compressed = false, bool encode=false);
	bool save(const QString& path, const QString& filename, bool compressed = false);
};

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  newObject
// Params:  const QString& superobjectname
//----------------------------------------------------------------------------- 
template <class T>
T* XObjectManager::newSuperObject(const QString& superobjectname)
{
	T t;
	QString c(t.className());
	XObject* object = newObject(t.className());
	if (object)
	{
		T* obj = dynamic_cast<T*>(object);
		if (obj)
		{
			obj->setSuperObjectName(superobjectname);
			insertSuperObject(obj);
		}
		return obj;
	}

	return Q_NULLPTR;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  superObject
// Params:  const QString& superobjectname
//----------------------------------------------------------------------------- 
template <class T>
T* XObjectManager::superObject(const QString& superobjectname)
{
	QReadLocker lock(&m_ObjectLock);

	if (m_IndexSuperObjects.contains(superobjectname))
	{
		for(XObject* obj:m_IndexSuperObjects.values(superobjectname))
		{
			return dynamic_cast<T*>(obj);
		}
	}
	return Q_NULLPTR;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  superObjects
// Params:  const QString& superobjectname
//----------------------------------------------------------------------------- 
template <class T>
QList<T*> XObjectManager::superObjects(const QString& superobjectname)
{
	QReadLocker lock(&m_ObjectLock);

	QList<T*> list;
	if (m_IndexSuperObjects.contains(superobjectname))
	{
		for(XObject*obj:m_IndexSuperObjects.values(superobjectname))
		{
			if (dynamic_cast<T*>(obj))
				list.append(dynamic_cast<T*>(obj));
		}
	}
	return list;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.07.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  allObjects
//----------------------------------------------------------------------------- 

template <class T> QList<T*> XObjectManager::allObjects()
{
	QReadLocker lock(&m_ObjectLock);

	QList<T*> list;
	{
		for (XObject* obj : objects())
		{
			if (dynamic_cast<T*>(obj))
				list.append(dynamic_cast<T*>(obj));
		}
	}
	return list;
}
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    10.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  newObject
// Params:  
//----------------------------------------------------------------------------- 

template <class T>
T* XObjectManager::newObject()
{
	T t;
	return cast<T>(newObject(t.className()));
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  cast
// Params:  XObject* obj
//----------------------------------------------------------------------------- 
template <class T>
T* XObjectManager::cast(XObject* obj)
{
	return static_cast<T*>(obj);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  findObject
// Params:  const QUuid& uuid
//----------------------------------------------------------------------------- 
XObject* XObjectManager::findObject(const QUuid& uuid) const
{
	QReadLocker lock(&m_ObjectLock);

	if (m_IndexObjects.contains(uuid.toString()))
		return m_IndexObjects[uuid.toString()];
	return Q_NULLPTR;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  find
// Params:  const QUuid& uuid
//----------------------------------------------------------------------------- 
template <class T>
T* XObjectManager::find(const QUuid& uuid)
{
	QReadLocker lock(&m_ObjectLock);

	if (m_IndexObjects.contains(uuid.toString()))
	{
		XObject* obj = m_IndexObjects[uuid.toString()];
		return dynamic_cast<T*>(obj);
	}
	return Q_NULLPTR;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    04.01.2023
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  find
// Params:  const QString& id
//----------------------------------------------------------------------------- 
template <class T>
T* XObjectManager::find(const QString& id)
{
	if (id.startsWith('{'))
		return find<T>(QUuid(id ));
	else
		return find<T>(QUuid("{" + id + "}"));
}

};
using namespace CoreBase;
