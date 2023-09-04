#pragma once

#include "CoreBaseGlobal.h"

#include <QDebug>
#include <QReadWriteLock>
#include <QDateTime>
#include <QStack>

namespace CoreBase
{
#define XDEBUG
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    31.05.2021
// Context: DefaultNamespace
// Class:   XDebug
// Method:  XDebug
//----------------------------------------------------------------------------- 

#ifdef XDEBUG

#define O_ASSERT(cond,msg) if (!(cond)) { qWarning()<<QString("%1  [%4] (%2:%3)").arg(msg).arg(__FILE__).arg(__LINE__).arg(#cond); } 
#define O_DEBUG(msg) (qDebug()<<QString("%1   (%2:%3)").arg(msg).arg(__FILE__).arg(__LINE__))
#define O_DEBUGSILENT(msg) (qDebug()<<QString("%1").arg(msg))
#define O_CRITICAL(msg) (qCritical()<<QString("%1   (%2:%3)").arg(msg).arg(__FILE__).arg(__LINE__))
#define O_WARNING(msg) (qWarning()<<QString("%1   (%2:%3)").arg(msg).arg(__FILE__).arg(__LINE__))
#define O_FATAL(msg) (qFatal()<<QString("%1   (%2:%3)").arg(msg).arg(__FILE__).arg(__LINE__))
#define O_INFO(msg) (qInfo()<<QString("%1   (%2:%3)").arg(msg).arg(__FILE__).arg(__LINE__))
#define O_STACK(info1,info2) XDebugStack debugstack_##info1_##info2(#info1,#info2,__FILE__,__LINE__);
#define O_STACKSILENT() XDebugStack debugstack;
#define O_PARAM(variable) (qDebug()<<QString("%1=%2").arg(#variable).arg(variable))
#define O_PARAMHEX(variable) (qDebug()<<QString("%1=%2:%3").arg(#variable).arg(variable).arg( QString( ((QString("%1").arg(variable)).toLocal8Bit()).toHex())))
#endif

#ifndef XDEBUG
#define O_ASSERT(cond,msg) ;
#define O_DEBUG(msg) ;
#define O_CRITICAL(msg) ;
#define O_WARNING(msg) ;
#define O_FATAL(msg) ;
#define O_INFO(msg) ;
#define O_STACK(info1,info2) ;
#define O_PARAM(variable) ;
#define O_PARAMHEX(variable) ;
#endif

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    31.05.2021
// Context: DefaultNamespace
// Class:   XDebug
// Method:  XDebug
//----------------------------------------------------------------------------- 

class COREBASE_EXPORT XDebug
{
	friend class XDebugStack;
public:
	//XDebug() {}
	//virtual ~XDebug() {}

	static void initLog();
	static void resetLog();
	static void flushLog();

	static void setDebug(bool debug = true) { m_Debug = debug; }
	static bool isDebug() { return m_Debug; }

	static void setDebugCache(int count) { m_DebugCacheCount = count; }

	static void log(QtMsgType type, const QMessageLogContext& context, const QString& msg);

protected:
	static const QString& fileName() { return m_FileName; }
	static int cacheCount() { return m_DebugCacheCount; }

	
	static void append(QtMsgType type, const QString& msg);
	static void write();
	
	static void pushIndent() { m_Indent++;  }
	static void popIndent() { m_Indent--; }
private:
	static bool m_Debug;
	static QString m_FileName;
	static int m_DebugCacheCount;
	static QReadWriteLock m_Lock;
	static QStringList m_Logs;
	static int m_Indent;
};

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    17.1.2011
// Context: Object
// Class:   XDebug
// Method:  XDebugStack
//----------------------------------------------------------------------------- 

class COREBASE_EXPORT XDebugStack
{
public:
	XDebugStack();
	XDebugStack(const char* message1, const char* message2, const char* file="", qint32 line = 0);
	virtual ~XDebugStack();
private:
	QString m_File;
	qint32 m_Line;
	QString m_Message1;
	QString m_Message2;
	QDateTime m_StartTime;
};

};
using namespace CoreBase;

