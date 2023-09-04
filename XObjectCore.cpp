#include "XObjectCore.h"

#include <qcache.h>
#include <QDir>
#include <QFile>
#include <QJsonDocument>

#include "XObjectManager.h"

#include <QMetaMethod>
#include <QJsonValue>

#include "XDebug.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  m_EmptyString
//----------------------------------------------------------------------------- 
QString XObjectCore::m_EmptyString;

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectCoreCore
// Method:  m_EmptyStringList
//----------------------------------------------------------------------------- 

QStringList XObjectCore::m_EmptyStringList;

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    03.04.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  XObjectCore
// Params:  QObject* parent
//----------------------------------------------------------------------------- 
XObjectCore::XObjectCore()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    03.04.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  ~XObjectCore
//----------------------------------------------------------------------------- 
XObjectCore::~XObjectCore()
{
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  isValue
// Params:  const QString& val
//----------------------------------------------------------------------------- 
bool XObjectCore::isValue(const QString& val) const
{
	QReadLocker lock(&m_PropertyLock);
	for (const QString& key : m_Object.keys())
	{
		if (m_Object.value(key).toString() == val)
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  isSearch
// Params:  const QString& val
//----------------------------------------------------------------------------- 
bool XObjectCore::isSearch(const QString& val) const
{
	QReadLocker lock(&m_PropertyLock);
	for (const QString& key : m_Object.keys())
	{
		if (m_Object.value(key).toString().contains(val, Qt::CaseInsensitive))
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  isValue
// Params:  const QString& key
// Params:  const QString& val
//----------------------------------------------------------------------------- 
bool XObjectCore::isValue(const QString& key, const QString& val) const
{
	QReadLocker lock(&m_PropertyLock);

	if (!m_Object.contains(key))
		return false;

	if (m_Object.value(key).toString() == val)
		return true;
	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  isSearch
// Params:  const QString& key
// Params:  const QString& val
//----------------------------------------------------------------------------- 
bool XObjectCore::isSearch(const QString& key, const QString& val) const
{
	QReadLocker lock(&m_PropertyLock);

	if (!m_Object.contains(key))
		return false;

	if (m_Object.value(key).toString().contains(val, Qt::CaseInsensitive))
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.04.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  className
// Params:  
//----------------------------------------------------------------------------- 
const QString& XObjectCore::className() const
{
	if (m_ClassName.isEmpty())
	{
		if (metaObject()) m_ClassName = metaObject()->className();
		// Remove Namespace
		if (m_ClassName.lastIndexOf("::") >= 0)
			m_ClassName = m_ClassName.mid(m_ClassName.lastIndexOf("::") + 2);
	}
	return m_ClassName;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  toMap
//----------------------------------------------------------------------------- 
QMap<QString, QString> XObjectCore::toMap() const
{
	QReadLocker lock(&m_PropertyLock);
	QMap<QString, QString> map;

	for(const QString&key:keys())
	{
		map[key] = m_Object.value(key).toString();
	}
	return map;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  fromMap
// Params:  const QMap<QString, QString>& map
//----------------------------------------------------------------------------- 
void XObjectCore::fromMap(const QMap<QString, QString>& map)
{
	QWriteLocker lock(&m_PropertyLock);

	for (const QString& key : map.keys())
	{
		m_Object[key]= map[key];
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  toMapVariant
//----------------------------------------------------------------------------- 
QMap<QString, QVariant> XObjectCore::toMapVariant() const
{
	QReadLocker lock(&m_PropertyLock);
	QMap<QString, QVariant> map;

	for (const QString& key : keys())
	{
		map[key] = m_Object.value(key).toVariant();
	}
	return map;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  fromMapVariant
// Params:  const QMap<QString, QVariant>& map
//----------------------------------------------------------------------------- 
void XObjectCore::fromMapVariant(const QMap<QString, QVariant>& map)
{
	QWriteLocker lock(&m_PropertyLock);

	for (const QString& key : map.keys())
	{
		m_Object.insert(key, QJsonValue::fromVariant(map[key]));
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  toDocument
// Params:  
//----------------------------------------------------------------------------- 

QJsonDocument XObjectCore::toDocument() const
{
	QJsonDocument doc;
	doc.setObject(toObject());
	return doc;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  fromDocument
// Params:  const QJsonDocument& document
//----------------------------------------------------------------------------- 

void XObjectCore::fromDocument(const QJsonDocument& document)
{
	if (document.isObject())
		fromObject(document.object());
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  toObject
//----------------------------------------------------------------------------- 
QJsonObject XObjectCore::toObject() const
{
	QJsonObject obj;
	for (const QString& key : keys())
	{
		obj[key] = m_Object.value(key);
		//O_PARAM(key);
		//O_PARAM(obj[key].toString());
	}
	return obj;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  fromObject
// Params:  const QJsonObject& object
//----------------------------------------------------------------------------- 
void XObjectCore::fromObject(const QJsonObject& object)
{
	QJsonObject obj;
	for (const QString& key : object.keys())
	{
		m_Object[key] = object.value(key);
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.01.2023
// Context: DefaultNamespace
// Class:   XObjectCore
// Method:  fromObject
// Params:  const CoreBase::XObject& object
//----------------------------------------------------------------------------- 
void XObjectCore::fromObject(const XObjectCore& object)
{
	for (const QString& key : object.keys())
		if (key!="index")
		{
			m_Object.insert(key, object.m_Object.value(key));
		}
}

