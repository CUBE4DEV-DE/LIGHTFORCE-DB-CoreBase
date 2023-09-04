#include "XObjectDocument.h"

#include <QFile>
#include <QJsonDocument>

#include "../CoreStorage/XFile.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectDocument
// Method:  XObjectDocument
//----------------------------------------------------------------------------- 
XObjectDocument::XObjectDocument(XObjectManager* objectmanager, const QString& path, const QString& filename)
{
	m_Path=path;
	m_Filename=filename;
	m_ObjectManager=objectmanager;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectDocument
// Method:  ~XObjectDocument
//----------------------------------------------------------------------------- 
XObjectDocument::~XObjectDocument()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectDocument
// Method:  open
// Params:  const QString& path
// Params:  const QString& filename
// Params:  bool compressed
//----------------------------------------------------------------------------- 
bool XObjectDocument::open(bool compressed, bool encode)
{
	//QFile file(m_Path+m_Filename);
	//if (!file.open(QIODevice::ReadOnly))
	//	return false;

	//if (compressed)
	//	return read(qUncompress(file.readAll()));

	//if (encode)
	//{
	//	QTextStream fileread(&file);
	//	fileread.setAutoDetectUnicode(true);
	//	QString encoded(fileread.readAll());

	//	encoded.replace(QString(QChar(223)), QString::fromLatin1("&szlig;"));
	//	encoded.replace(QString(QChar(196)), QString::fromLatin1("&Auml;"));
	//	encoded.replace(QString(QChar(228)), QString::fromLatin1("&auml;"));
	//	encoded.replace(QString(QChar(203)), QString::fromLatin1("&Euml;"));
	//	encoded.replace(QString(QChar(235)), QString::fromLatin1("&euml;"));
	//	encoded.replace(QString(QChar(207)), QString::fromLatin1("&Iuml;"));
	//	encoded.replace(QString(QChar(239)), QString::fromLatin1("&iuml;"));
	//	encoded.replace(QString(QChar(214)), QString::fromLatin1("&Ouml;"));
	//	encoded.replace(QString(QChar(246)), QString::fromLatin1("&ouml;"));
	//	encoded.replace(QString(QChar(220)), QString::fromLatin1("&Uuml;"));
	//	encoded.replace(QString(QChar(252)), QString::fromLatin1("&uuml;"));

	//	QJsonDocument doc(QJsonDocument::fromJson(encoded.toLatin1()));
	//	if (doc.isArray())
	//	{
	//		readArray(doc.array());
	//		return true;
	//	}
	//}
	//else
	//{
	//	return read(file.readAll());
	//}
	//return false;
	if (encode)
	{
		QString encoded= XFile::readCache(m_Path + QDir::separator() + m_Filename);
		encoded.replace(QString(QChar(223)), QString::fromLatin1("&szlig;"));
		encoded.replace(QString(QChar(196)), QString::fromLatin1("&Auml;"));
		encoded.replace(QString(QChar(228)), QString::fromLatin1("&auml;"));
		encoded.replace(QString(QChar(203)), QString::fromLatin1("&Euml;"));
		encoded.replace(QString(QChar(235)), QString::fromLatin1("&euml;"));
		encoded.replace(QString(QChar(207)), QString::fromLatin1("&Iuml;"));
		encoded.replace(QString(QChar(239)), QString::fromLatin1("&iuml;"));
		encoded.replace(QString(QChar(214)), QString::fromLatin1("&Ouml;"));
		encoded.replace(QString(QChar(246)), QString::fromLatin1("&ouml;"));
		encoded.replace(QString(QChar(220)), QString::fromLatin1("&Uuml;"));
		encoded.replace(QString(QChar(252)), QString::fromLatin1("&uuml;"));

		QJsonDocument doc(QJsonDocument::fromJson(encoded.toLatin1()));
		if (doc.isArray())
		{
			readArray(doc.array());
			return true;
		}
		return false;
	}


	QByteArray content= XFile::readBinaryCache(m_Path + QDir::separator()+m_Filename);
	if (compressed)
		return read(qUncompress(content));
	else
		return read(content);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectDocument
// Method:  read
// Params:  const QByteArray& content
// Params:  
//----------------------------------------------------------------------------- 
bool XObjectDocument::read(const QByteArray& content)
{

	QJsonDocument doc(QJsonDocument::fromJson(content));
	if (doc.isArray())
	{
		readArray(doc.array());
		return true;
	}
	
	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectDocument
// Method:  readArray
// Params:  const QJsonArray& array
//----------------------------------------------------------------------------- 
void XObjectDocument::readArray(const QJsonArray& array)
{
	for (int i = 0; i < array.size(); i++)
	{
		QJsonObject jsonobj = array[i].toObject();
		QString index = jsonobj.value("idx").toString();
		QString classname = jsonobj.value("class").toString();
		QString superobjectname = jsonobj.value("super").toString();

		if (classname.isEmpty())
		{
			XObject* obj = Q_NULLPTR;

			if (index.isEmpty())
				obj = m_ObjectManager->newObject("XObject");
			else
				obj = m_ObjectManager->newObject("XObject",QUuid(index));

			if (obj)
			{
				obj->setSuperObjectName(superobjectname);
				m_ObjectManager->insertSuperObject(obj);
				if (index.isEmpty())
					index = obj->strIndex();
				jsonobj["index"] = index;
				//jsonobj["position"] = QString("%1").arg(i+1);
				jsonobj["position"] = m_ObjectManager->objects().count() + 1;
				obj->object() = jsonobj;
			}
		}
		else
		{
			XObject* obj = Q_NULLPTR;

			if (index.isEmpty())
				obj = m_ObjectManager->newObject(classname);
			else
				obj = m_ObjectManager->newObject(classname, QUuid(index));

			if (obj)
			{
				obj->setSuperObjectName(superobjectname);
				m_ObjectManager->insertSuperObject(obj);

				// Values
				QJsonObject jsonvalues = jsonobj.value("values").toObject();

				if (index.isEmpty())
					index = obj->strIndex();

				jsonvalues["index"] = index;
				obj->object() = jsonvalues;
				
				// Childs
				readObjectArray(obj, jsonobj.value("childs").toArray());
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectDocument
// Method:  readObjectArray
// Params:  CoreBase::XObject* obj
// Params:  const QJsonArray& array
//----------------------------------------------------------------------------- 
void XObjectDocument::readObjectArray(XObject* obj, const QJsonArray& array)
{
	for (int i = 0; i < array.size(); i++)
	{
		QJsonObject jsonobj = array[i].toObject();
		QString index = jsonobj.value("idx").toString();
		QString classname = jsonobj.value("class").toString();

		if (!index.isEmpty() && !classname.isEmpty())
		{
			XObject* childobj = obj->newChildObject(classname, QUuid(index));
			if (childobj)
			{
				// Values
				QJsonObject jsonvalues = jsonobj.value("values").toObject();
				jsonvalues["index"] = index;
				childobj->object() = jsonvalues;

				// Childs
				readObjectArray(childobj, jsonobj.value("childs").toArray());
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectDocument
// Method:  save
// Params:  const QString& path
// Params:  const QString& filename
// Params:  bool compressed
//----------------------------------------------------------------------------- 
bool XObjectDocument::save(bool compressed)
{
	//QFile file(m_Path + m_Filename);
	//if (!file.open(QIODevice::WriteOnly))
	//	return false;

	//if (compressed)
	//	return file.write(qCompress(write()));

	//return file.write(write());

	if (compressed)
		return XFile::writeBinary(m_Path + QDir::separator() + m_Filename, qCompress(write()), true);
	else
		return XFile::writeBinary(m_Path + QDir::separator() + m_Filename, write(),true);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectDocument
// Method:  write
// Params:  
//----------------------------------------------------------------------------- 
const QByteArray& XObjectDocument::write()
{
	QJsonArray rootobj;


	bool issuper = false;
	for (XObject* obj : m_ObjectManager->objects())
	{
		if (obj && !obj->superObjectName().isEmpty())
			issuper = true;
	}

	for(XObject*obj:m_ObjectManager->objects())
	{
		if (obj&&(!obj->superObjectName().isEmpty()||!issuper))
		{
			QJsonObject jsonobj;

			jsonobj.insert("idx", QJsonValue(obj->strIndex()));
			jsonobj.insert("class", QJsonValue(obj->className()));
			jsonobj.insert("super", QJsonValue(obj->superObjectName()));

			QJsonObject values(obj->object());
			values.remove("index");
			if (!values.isEmpty())
				jsonobj.insert("values", values);

			QJsonArray array = writeArray(obj);
			if (!array.isEmpty())
				jsonobj.insert("childs", array);

			rootobj.append(jsonobj);
		}
	}

	QJsonDocument jsondocument(rootobj);
	m_Content=jsondocument.toJson();
	return m_Content;
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.04.2022
// Context: DefaultNamespace
// Class:   XObjectDocument
// Method:  writeArray
// Params:  CoreBase::XObject* obj
//----------------------------------------------------------------------------- 

QJsonArray  XObjectDocument::writeArray(XObject*obj)
{
	QJsonArray childs;

	for (XObject* childobj:obj->childAllObjects())
	{
		if (childobj)
		{
			QJsonObject jsonobj;

			jsonobj.insert("idx", QJsonValue(childobj->strIndex()));
			jsonobj.insert("class", QJsonValue(childobj->className()));

			QJsonObject values(childobj->object());
			values.remove("index");
			if (!values.isEmpty())
				jsonobj.insert("values", values);

			QJsonArray array = writeArray(childobj);
			if (!array.isEmpty())
				jsonobj.insert("childs", array);
			
			childs.append(jsonobj);
		}
	}

	return childs;
}
