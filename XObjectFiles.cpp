#include "XObjectFiles.h"
#include <qcache.h>
#include <QDir>
#include <QFile>
#include <QMetaMethod>


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  m_ContentCache
//----------------------------------------------------------------------------- 

QCache<QString, QString> XObjectFiles::m_ContentCache(5000);
QReadWriteLock XObjectFiles::m_CacheLock;

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  read
// Params:  const QString& filename
// Params:  
//----------------------------------------------------------------------------- 
QString XObjectFiles::read(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
		return XObjectCore::emptyString();

	QTextStream text(&file);
	text.setAutoDetectUnicode(true);
	return text.readAll();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  read
// Params:  const QString& path
// Params:  const QString& filename
//----------------------------------------------------------------------------- 
QString XObjectFiles::read(const QString& path, const QString& filename)
{
	return read(path + QDir::separator() + filename);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  write
// Params:  const QString& filename
// Params:  const QString& content
//----------------------------------------------------------------------------- 
void XObjectFiles::write(const QString& filename, const QString& content)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
		return;

	QTextStream text(&file);
	text << content;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  readCache
// Params:  const QString& filename
// Params:  
//----------------------------------------------------------------------------- 

const QString& XObjectFiles::readCache(const QString& filename)
{
	{
		QReadLocker lock(&m_CacheLock);
		if (m_ContentCache.contains(filename))
		{
			return *m_ContentCache[filename];
		}
	}
	{
		QWriteLocker lock(&m_CacheLock);
		QString content = read(filename);
		m_ContentCache.insert(filename, new QString(content));
	}
	{
		QReadLocker lock(&m_CacheLock);
		if (m_ContentCache.contains(filename))
		{
			return *m_ContentCache[filename];
		}
	}

	return XObjectCore::emptyString();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  clearCache
//----------------------------------------------------------------------------- 

void XObjectFiles::clearCache()
{
	QWriteLocker lock(&m_CacheLock);
	m_ContentCache.clear();
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  m_ShortCuts
//----------------------------------------------------------------------------- 

QMap<QString, QString> XObjectFiles::m_ShortCuts;
QReadWriteLock XObjectFiles::m_ShortCutLock;


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  shortCut
// Params:  const QString& shortcut
// Params:  const QString& content
//----------------------------------------------------------------------------- 

void XObjectFiles::shortCut(const QString& shortcut, const QString& content)
{
	QWriteLocker lock(&m_ShortCutLock);
	m_ShortCuts[shortcut] = QString(content);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  shortCutRead
// Params:  const QString& shortcut
// Params:  const QString& filename
//----------------------------------------------------------------------------- 

const QString& XObjectFiles::shortCutRead(const QString& shortcut, const QString& filename)
{
	{
		QReadLocker lock(&m_ShortCutLock);
		if (m_ShortCuts.contains(shortcut))
		{
			return m_ShortCuts[shortcut];
		}
	}
	{
		QWriteLocker lock(&m_ShortCutLock);
		m_ShortCuts[shortcut] = read(filename);
	}
	{
		QReadLocker lock(&m_ShortCutLock);
		if (m_ShortCuts.contains(shortcut))
		{
			return m_ShortCuts[shortcut];
		}
	}
	return XObjectCore::emptyString();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  shortCutRead
// Params:  const QString& shortcut
// Params:  const QString& path
// Params:  const QString& filename
//----------------------------------------------------------------------------- 

const QString& XObjectFiles::shortCutRead(const QString& shortcut, const QString& path, const QString& filename)
{
	return shortCutRead(shortcut, path + QDir::separator() + filename);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  shortCutDirectory
// Params:  const QString& path
//----------------------------------------------------------------------------- 
QStringList XObjectFiles::shortCutDirectory(const QString& path)
{
	QStringList shortcutlist;

	QDir dir(path);
	for (QFileInfo fileinfo : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::Files))
	{
		fileinfo.refresh();
		shortcutlist.append(fileinfo.baseName());
		shortCutRead(fileinfo.baseName(), path, fileinfo.fileName());
	}

	return shortcutlist;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  shortCut
// Params:  const QString& shortcut
// Params:  
//----------------------------------------------------------------------------- 

const QString& XObjectFiles::shortCut(const QString& shortcut)
{
	{
		QReadLocker lock(&m_ShortCutLock);
		if (m_ShortCuts.contains(shortcut))
		{
			return m_ShortCuts[shortcut];
		}
	}

	return XObjectCore::emptyString();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.06.2022
// Context: DefaultNamespace
// Class:   XObjectFiles
// Method:  hasShortCut
// Params:  const QString& shortcut
// Params:  
//----------------------------------------------------------------------------- 

bool XObjectFiles::hasShortCut(const QString& shortcut)
{
	{
		QReadLocker lock(&m_ShortCutLock);
		if (m_ShortCuts.contains(shortcut))
		{
			return true;
		}
	}
	return false;
}

