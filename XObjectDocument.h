#pragma once

#include <QObject>

#include "XObjectManager.h"

namespace CoreBase
{

	//-----------------------------------------------------------------------------
	// Author:  Tobias Post
	// Company: CUBE4DEV GmbH
	// Date:    06.04.2022
	// Context: DefaultNamespace
	// Class:   XObjectDocument
	// Method:  XObjectDocument
	// Description:  The purpose of the XObjectDocument class is to provide
	//  functionality to open, save and read JSON data from a file. It has
	//  methods open() and save() that are used to read data from a file and
	//  save data respectively. Additionally, it has two helper methods that
	//  are used to parse JSON data: readArray() and readObjectArray().
	//  Furthermore, the XObjectDocument class serves as a data storage
	//  container with the help of its attribute variables m_Path, m_Filename
	//  and m_Content. These attributes store the location and actual content
	//  of the file being processed. The class also has a protected read
	//  and write methods that can be used to perform read and write
	//  operations on the file.
	//----------------------------------------------------------------------------- 
	
	class XObjectDocument
	{
	public:
		XObjectDocument(XObjectManager* objectmanager,const QString& path, const QString& filename);
		virtual ~XObjectDocument();

		bool open(bool compressed=false,bool encode=false);
		bool save(bool compressed=false);
		void readArray(const QJsonArray& array);
		void readObjectArray(XObject* obj, const QJsonArray& array);

	protected:
		bool read(const QByteArray& content);
		const QByteArray& write();
		QJsonArray writeArray(XObject* obj);
	private:
		QString m_Path;
		QString m_Filename;
		XObjectManager* m_ObjectManager;

		QByteArray m_Content;
	};


};
using namespace CoreBase;
