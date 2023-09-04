#include "XObjectManager.h"

#include "XDebug.h"
#include "XObjectDocument.h"
#include "XObjectFactory.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  XObjectManager
//----------------------------------------------------------------------------- 
XObjectManager::XObjectManager()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  ~XObjectManager
//----------------------------------------------------------------------------- 
XObjectManager::~XObjectManager()
{
	for (XObject* obj : objects())
	{
		if (obj)
			deleteObject(obj);
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    02.12.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  lockRead
//----------------------------------------------------------------------------- 

void XObjectManager::lockRead()
{
	m_BaseLock.lockForRead();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    02.12.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  lockWrite
//----------------------------------------------------------------------------- 

void XObjectManager::lockWrite()
{
	m_BaseLock.lockForWrite();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    02.12.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  unlock
//----------------------------------------------------------------------------- 
void XObjectManager::unlock()
{
	m_BaseLock.unlock();
}



//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  insertSuperObject
// Params:  CoreBase::XObject* obj
//----------------------------------------------------------------------------- 
void XObjectManager::insertSuperObject(XObject* obj)
{
	QWriteLocker lock(&m_ObjectLock);

	if (!obj->superObjectName().isEmpty())
		m_IndexSuperObjects.insert(obj->superObjectName(), obj);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  newObject
// Params:  const QString& classname
//----------------------------------------------------------------------------- 
XObject* XObjectManager::newObject(const QString& classname)
{
	QWriteLocker lock(&m_ObjectLock);

	XObject* obj=XObjectFactory::newObject(classname);
	if (obj)
	{
		m_IndexObjects[obj->strIndex()] = obj;
		obj->setObjectManager(this);
		obj->init();
	}
	return obj;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  newObject
// Params:  const QString& classname
// Params:  const QUuid& idx
//----------------------------------------------------------------------------- 
XObject* XObjectManager::newObject(const QString& classname, const QUuid& idx)
{
	QWriteLocker lock(&m_ObjectLock);

	XObject* obj = XObjectFactory::newObject(classname,idx);
	if (obj)
	{
		m_IndexObjects[obj->strIndex()] = obj;
		obj->setObjectManager(this);
		obj->init();
	}
	return obj;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  deleteObject
// Params:  XObject* obj
//----------------------------------------------------------------------------- 
bool XObjectManager::deleteObject(XObject* obj)
{
	QWriteLocker lock(&m_ObjectLock);

	if (obj)
	{
		if (m_IndexObjects.contains(obj->strIndex()))
			m_IndexObjects.remove(obj->strIndex());

		if (!obj->superObjectName().isEmpty())
		{
			m_IndexSuperObjects.remove(obj->superObjectName(),obj);
		}

		delete obj;

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  deleteObject
// Params:  const QUuid& uuid
//----------------------------------------------------------------------------- 
bool XObjectManager::deleteObject(const QUuid& uuid)
{
	QWriteLocker lock(&m_ObjectLock);

	if (m_IndexObjects.contains(uuid.toString()))
	{
		XObject* obj = m_IndexObjects[uuid.toString()];
		m_IndexObjects.remove(uuid.toString());

		if (obj)
		{
			if (!obj->superObjectName().isEmpty())
			{
				m_IndexSuperObjects.remove(obj->superObjectName(), obj);
			}

			delete obj;
		}
		
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  open
// Params:  const QString& path
// Params:  const QString& filename
// Params:  bool compressed
//----------------------------------------------------------------------------- 

bool XObjectManager::open(const QString& path, const QString& filename, bool compressed,bool encode)
{
	XObjectDocument document(this,path,filename);
	return document.open(compressed, encode);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  save
// Params:  const QString& path
// Params:  const QString& filename
// Params:  bool compressed
//----------------------------------------------------------------------------- 

bool XObjectManager::save(const QString& path, const QString& filename, bool compressed)
{
	XObjectDocument document(this, path, filename);
	return document.save(compressed);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.01.2023
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  objectManager
// Params:  const QString& name
// Params:  const QString& path
//----------------------------------------------------------------------------- 
XObjectManager* XObjectManager::objectManager(const QString& name)
{
	if (m_ObjectManagerMap.contains(name))
		return m_ObjectManagerMap[name];

	m_ObjectManagerMap[name] = new XObjectManager();
	return m_ObjectManagerMap[name];
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.01.2023
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  removeObjectManager
// Params:  const QString& name
//----------------------------------------------------------------------------- 

void XObjectManager::removeObjectManager(const QString& name)
{
	if (m_ObjectManagerMap.contains(name))
	{
		delete m_ObjectManagerMap[name];
		m_ObjectManagerMap.remove(name);
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.01.2023
// Context: DefaultNamespace
// Class:   XObjectManager
// Method:  m_ObjectManagerMap
//----------------------------------------------------------------------------- 

QMap<QString, XObjectManager* > XObjectManager::m_ObjectManagerMap;
