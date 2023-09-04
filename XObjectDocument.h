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
