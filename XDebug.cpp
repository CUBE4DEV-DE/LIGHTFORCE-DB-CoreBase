#include "XDebug.h"

#include <QFile>

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    31.05.2021
// Context: Object
// Class:   XDebug
//----------------------------------------------------------------------------- 
bool XDebug::m_Debug = true;
QReadWriteLock XDebug::m_Lock;
QStringList XDebug::m_Logs;
int XDebug::m_Indent=0;
QString XDebug::m_FileName;
int XDebug::m_DebugCacheCount = 128;

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   XDebug
// Method:  XDebugStack
//----------------------------------------------------------------------------- 
XDebugStack::XDebugStack()
{
	m_Line = -1;
	XDebug::pushIndent();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XDebug
// Method:  XDebugStack
// Params:  const char* message1
// Params:  const char* message2
// Params:  const char* file
// Params:  qint32 line
//----------------------------------------------------------------------------- 
XDebugStack::XDebugStack(const char* message1, const char* message2, const char* file, qint32 line)// , qint32 level, const char* infomodule) : m_Level(0)
{
	m_Message1 = message1;
	m_Message2 = message2;
	m_File = file;
	m_Line = line;
	m_StartTime = QDateTime::currentDateTime();

	if (m_File.isEmpty()||m_Line==0)
		qDebug() << QString("CALL IN  %1:%2").arg(m_Message1).arg(m_Message2);
	else
		qDebug() << QString("CALL IN  %1:%2 - %3:%4").arg(m_Message1).arg(m_Message2).arg(m_File).arg(m_Line);

	XDebug::pushIndent();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XDebug
// Method:  ~XDebugStack
//----------------------------------------------------------------------------- 

XDebugStack::~XDebugStack()
{
	if (m_Line<0)
	{
		XDebug::popIndent();
		return;
	}

	qDebug() << QString("TIME %1").arg(m_StartTime.msecsTo(QDateTime::currentDateTime()));

	XDebug::popIndent();

	if (m_File.isEmpty() || m_Line == 0)
		qDebug() << QString("CALL OUT %1:%2").arg(m_Message1).arg(m_Message2);
	else
		qDebug() << QString("CALL OUT %1:%2 - %3:%4").arg(m_Message1).arg(m_Message2).arg(m_File).arg(m_Line);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XDebug
// Method:  resetLog
//----------------------------------------------------------------------------- 
void XDebug::resetLog()
{
	QWriteLocker lock(&m_Lock);

	QString filename(m_FileName + ".txt");
	m_Logs.clear();
	
	QFile::remove(filename);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XDebug
// Method:  flushLog
//----------------------------------------------------------------------------- 

void XDebug::flushLog()
{
	write();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XDebug
// Method:  append
// Params:  QtMsgType type
// Params:  const QString& msg
//----------------------------------------------------------------------------- 

void XDebug::append(QtMsgType type, const QString& msg)
{
	QString ident;

	if (m_Indent)
		ident = QString("%1:").arg(m_Indent) + QString(m_Indent*4, QChar(' '));

	QString line;
	switch (type)
	{
		case QtDebugMsg:line = QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + QString("         %2 %1\n").arg(QString(msg)).arg(ident); break;
		case QtInfoMsg:line = QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + QString("!        %2 %1\n").arg(QString(msg)).arg(ident); break;
		case QtWarningMsg:line = QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + QString("WARNING: %2 %1\n").arg(QString(msg)).arg(ident); break;
		case QtCriticalMsg:line = QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + QString("CRITICAL:%2 %1\n").arg(QString(msg)).arg(ident); break;
		case QtFatalMsg:line = QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + QString("FATAL:   %2 %1\n").arg(QString(msg)).arg(ident); abort(); break;
	}

	{
		QWriteLocker lock(&m_Lock);
		m_Logs.append(line);
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XDebug
// Method:  write
//----------------------------------------------------------------------------- 

void XDebug::write()
{
	{
		QReadLocker lock(&m_Lock);

		if (m_Logs.isEmpty())
			return;

		//for (const QString& mod : m_Logs)
		{
			QString filename(XDebug::fileName() + ".txt");
			QFile file(filename);
			if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
			{
				QTextStream out(&file);
				for (const QString& line : m_Logs)
				{
					out << line;
				}
			}
		}
	}

	{
		QWriteLocker lock(&m_Lock);
		m_Logs.clear();
	}
	
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XDebug
// Method:  log
// Params:  QtMsgType type
// Params:  const QMessageLogContext& context
// Params:  const QString& msg
//----------------------------------------------------------------------------- 
void XDebug::log(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	if (!isDebug())
		return;

	append(type, msg);

	if (m_Logs.count()<cacheCount())
		return;
	
	{
		write();
	}
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    04.06.2018
// Context: Object
// Class:   oObjectManager
// Method:  globalMessageOutput
// Params:  QtMsgType type
// Params:  const QMessageLogContext& context
// Params:  const QString& msg
//-----------------------------------------------------------------------------
void globalMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	XDebug::log(type, context, msg);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    04.06.2018
// Context: Object
// Class:   oObjectManager
// Method:  initLog
//-----------------------------------------------------------------------------
void XDebug::initLog()
{
	m_FileName = "log" + QDateTime::currentDateTime().toString("yyyyMMdd");
	qInstallMessageHandler(globalMessageOutput);
}

