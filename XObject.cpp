#include "XObject.h"

#include <qcache.h>
#include <QDir>
#include <QFile>
#include <QJsonDocument>

#include "XObjectManager.h"

#include <QMetaMethod>

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    03.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  XObject
// Params:  QObject* parent
//----------------------------------------------------------------------------- 
XObject::XObject()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  XObject
// Params:  const QMap<QString, QString>& map
//----------------------------------------------------------------------------- 

XObject::XObject(const QMap<QString, QString>& map)
{
	fromMap(map);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  XObject
// Params:  const QMap<QString, QVariant>& map
//----------------------------------------------------------------------------- 
XObject::XObject(const QMap<QString, QVariant>& map)
{
	fromMapVariant(map);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  XObject
// Params:  const QJsonDocument& doc
//----------------------------------------------------------------------------- 
XObject::XObject(const QJsonDocument& doc)
{
	fromDocument(doc);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  XObject
// Params:  const QJsonObject& obj
//----------------------------------------------------------------------------- 
XObject::XObject(const QJsonObject& obj)
{
	fromObject(obj);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    01.12.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  XObject
// Params:  const QString& content
//----------------------------------------------------------------------------- 
XObject::XObject(const QString& content)
{
	QJsonParseError error;
	QJsonDocument doc(QJsonDocument::fromJson(QString(content).toUtf8(), &error));
	if (error.error != 0)
	{
		qDebug() << "Error:" << error.errorString();
	}

}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    03.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  ~XObject
//----------------------------------------------------------------------------- 
XObject::~XObject()
{
	for (XObject* obj : m_ClassObjects.values())
	{
		if (obj)
			obj->clearAllObjects();
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    03.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  init
//----------------------------------------------------------------------------- 
void XObject::init()
{
	// Individual Values
	// Called only from objectManager!!!
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    04.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  initMeta
//----------------------------------------------------------------------------- 
void XObject::initMeta()
{
	// Table Structure
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  getJsonObject
//----------------------------------------------------------------------------- 

//QJsonObject XObject::getJsonObject()
//{
//	QJsonObject jsonobject(object());
//	for (XCoreObjectList* objlist : classObjectLists())
//	{
//		jsonobject[objlist->className()] = objlist->getJsonArray();
//	}
//
//	return jsonobject;
//}
//
////-----------------------------------------------------------------------------
//// Author:  Tobias Post
//// Company: CUBE4DEV GmbH
//// Date:    05.04.2022
//// Context: DefaultNamespace
//// Class:   XObject
//// Method:  setJsonObject
//// Params:  QJsonObject& object
////----------------------------------------------------------------------------- 
//
//void XObject::setJsonObject(QJsonObject& object)
//{
//	object= m_Object;
//	for (XCoreObjectList* objlist : classObjectLists())
//	{
//		object[objlist->className()] = objlist->getJsonArray();
//	}
//}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.01.2023
// Context: DefaultNamespace
// Class:   XObject
// Method:  hasCall
// Params:  const QString& slotname
// Params:  
//----------------------------------------------------------------------------- 

bool XObject::hasCall(const QString& slotname)
{
	for (int i = 0; i < metaObject()->methodCount(); i++)
	{
		QMetaMethod slotmethod = metaObject()->method(i);
		if (QString(slotmethod.methodSignature()).indexOf(slotname + "(") == 0)
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  call
// Params:  const QString& slotname
//----------------------------------------------------------------------------- 


bool XObject::call(const QString& slotname)
{
	return call(slotname, this);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  call
// Params:  const QString& slotname
// Params:  const CoreBase::XObject* values
//----------------------------------------------------------------------------- 

bool XObject::call(const QString& slotname,  XObject* values)
{
	int slotid = -1;
	for (int i = 0; i < metaObject()->methodCount(); i++)
	{
		QMetaMethod slotmethod = metaObject()->method(i);
		if (QString(slotmethod.methodSignature()).indexOf(slotname + "(") == 0)
		{
			slotid = i;
			i = metaObject()->methodCount();
		}
	}
	if (slotid < 0)
	{
		return false;
	}

	QMetaMethod method = metaObject()->method(slotid);

	static QMutex mutex;

	{
		QMutexLocker lock(&mutex);

		QMetaMethodArgument arg[10];
		static QString paramstring[10];
		static int paramint[10];
		static bool parambool[10];
		static double paramdouble[10];
		static long paramlong[10];
		static float paramfloat[10];
		static QDate paramdate[10];
		static QTime paramtime[10];
		static QUuid paramuuid[10];

		int i = 0;
		for (; i < method.parameterNames().count(); i++)
		{
			QString paramname = QString(method.parameterNames()[i]);
			QString paramtype = QString(method.parameterTypes()[i]);
			QString paramvalue;

			if (values->object().contains(paramname.toLocal8Bit()))
			{
				paramvalue = values->object().value(paramname.toLocal8Bit()).toString();
			}
			else if (object().contains(paramname.toUpper().toLocal8Bit()))
			{
				paramvalue = values->object().value(paramname.toUpper().toLocal8Bit()).toString();
			}

			if (paramtype == QString("QString"))
			{
				paramstring[i] = paramvalue;
				arg[i] = Q_ARG(QString, paramstring[i]);
				//O_PARAM(paramstring[i]);
			}
			else if (paramtype == QString("int"))
			{
				paramint[i] = paramvalue.toInt();
				arg[i] = Q_ARG(int, paramint[i]);
				//O_PARAM(paramint[i]);
			}
			else if (paramtype == QString("bool"))
			{
				parambool[i] = paramvalue.indexOf("T", Qt::CaseInsensitive) == 0 || paramvalue.indexOf("on", Qt::CaseInsensitive) == 0;
				arg[i] = Q_ARG(bool, parambool[i]);
			}
			else if (paramtype == QString("double"))
			{
				paramdouble[i] = paramvalue.toDouble();
				arg[i] = Q_ARG(double, paramdouble[i]);
			}
			else if (paramtype == QString("float"))
			{
				paramfloat[i] = paramvalue.toFloat();
				arg[i] = Q_ARG(float, paramfloat[i]);
			}
			else if (paramtype == QString("long"))
			{
				paramlong[i] = paramvalue.toLong();
				arg[i] = Q_ARG(long, paramlong[i]);
			}
			else if (paramtype == QString("QDate"))
			{
				paramdate[i] = QDate::fromString(paramvalue, Qt::ISODate);
				arg[i] = Q_ARG(QDate, paramdate[i]);
			}
			else if (paramtype == QString("QTime"))
			{
				paramtime[i] = QTime::fromString(paramvalue, Qt::ISODate);
				arg[i] = Q_ARG(QTime, paramtime[i]);
			}
			else if (paramtype == QString("QUuid"))
			{
				paramuuid[i] = QUuid(paramvalue);
				arg[i] = Q_ARG(QUuid, paramuuid[i]);
			}
			else
			{
				paramstring[i] = paramvalue;
				arg[i] = Q_ARG(QString, paramstring[i]);
			}
		}

		for (; i < 10; i++)
		{
			paramstring[i] = "";
			paramint[i] = 0;
			parambool[i] = false;
			paramdouble[i] = 0;
			paramlong[i] = 0;
			paramfloat[i] = 0;
			paramdate[i] = QDate();
			paramtime[i] = QTime();
			paramuuid[i] = QUuid();
		}

		if (QMetaObject::invokeMethod(this, slotname.toLocal8Bit(), Qt::DirectConnection, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]))
		{
			return true;
		}

	}
	return false;
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  newChildObject
// Params:  const QString& classname
// Params:  const QUuid& idx
// Params:  
//----------------------------------------------------------------------------- 
XObject* XObject::newChildObject(const QString& classname, const QUuid& idx)
{
	if (m_ClassObjects.contains(classname))
	{
		return m_ClassObjects[classname]->newChildObject(idx);
	}
	return Q_NULLPTR;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  registerClassObjects
// Params:  CoreBase::XObject& objectlist
//----------------------------------------------------------------------------- 
void XObject::registerClassObjects(XObject& objectlist)
{
	objectlist.setObjectManager(objectManager());
	objectlist.setObjectParentList(this);
	m_ClassObjects[objectlist.childClassName()] = &objectlist;
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObject
// Method:  childAllObjects
// Params:  
//----------------------------------------------------------------------------- 

QList<XObject*> XObject::childAllObjects() const
{
	QList<XObject*> objects;
	for(XObject* obj:m_ClassObjects.values())
	{
		for(XObject* childobj :obj->childObjects())
		{
			objects.append(childobj);
		}
	}
	return objects;
}
