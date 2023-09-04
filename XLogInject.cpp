#include "XLogInject.h"
#include "XFile.h"

#include <QDir>
#include <QJsonDocument>

XLogInject XLogsInject::m_ObjectInjects;

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    16.02.2023
// Context: DefaultNamespace
// Class:   XFunctionInject
// Method:  inject
// Params:  const QString& name
// Params:  functionObject function
//----------------------------------------------------------------------------- 
void XLogsInject::inject(const QString& name, XFunctionLog function)
{
	m_ObjectInjects.inject(name, function);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    16.02.2023
// Context: DefaultNamespace
// Class:   XFunctionInject
// Method:  injects
// Params:  const QString& name
// Params:  functionObject function
//----------------------------------------------------------------------------- 

void XLogsInject::injects(const QString& name, XFunctionLog function)
{
	m_ObjectInjects.injects(name, function);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    16.02.2023
// Context: DefaultNamespace
// Class:   XFunctionInject
// Method:  hasObjectFunction
// Params:  const QString& name
//----------------------------------------------------------------------------- 
bool XLogsInject::hasObjectFunction(const QString& name)
{
	return m_ObjectInjects.hasFunction(name);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    16.02.2023
// Context: DefaultNamespace
// Class:   XFunctionInject
// Method:  hasObjectFunctions
// Params:  const QString& name
//----------------------------------------------------------------------------- 
bool XLogsInject::hasObjectFunctions(const QString& name)
{
	return m_ObjectInjects.hasFunctions(name);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    18.02.2023
// Context: DefaultNamespace
// Class:   XLogInject
// Method:  call
// Params:  const QString& name
// Params:  const QString& module
// Params:  const QString& filename
// Params:  const QString& line
// Params:  const QString& classname
// Params:  const QString& method
// Params:  const QString& event
// Params:  const QString& status
// Params:  const QJsonObject& jsonobj
//----------------------------------------------------------------------------- 
void XLogsInject::call(const QString& name, const QString& module, const QString& filename, const QString& line, const QString& classname, const QString& method, const QString& event, const QString& status, const QJsonObject& jsonobj)
{
	return m_ObjectInjects.call(module, filename, line, classname, method, event, status, jsonobj);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    18.02.2023
// Context: DefaultNamespace
// Class:   XLogInject
// Method:  calls
// Params:  const QString& name
// Params:  const QString& module
// Params:  const QString& filename
// Params:  const QString& line
// Params:  const QString& classname
// Params:  const QString& method
// Params:  const QString& event
// Params:  const QString& status
// Params:  const QJsonObject& jsonobj
//----------------------------------------------------------------------------- 
void XLogsInject::calls(const QString& name, const QString& module, const QString& filename, const QString& line, const QString& classname, const QString& method, const QString& event, const QString& status, const QJsonObject& jsonobj)
{
	return m_ObjectInjects.calls(module, filename, line, classname, method, event, status, jsonobj);
}


void XLogsInject::Log(const QString& filepath, const QString& module, const QString& filename, const QString& line, const QString& classname, const QString& method, const QString& event, const QString& status, const QJsonObject& jsonobj)
{

	QDateTime stamp = QDateTime::currentDateTime();

	QJsonObject jsonobject = jsonobj;

	jsonobject["LogStamp"] = stamp.toString(Qt::ISODate);
	jsonobject["LogTime"] = stamp.time().toString(Qt::ISODate);
	jsonobject["LogDate"] = stamp.date().toString(Qt::ISODate);
	jsonobject["LogModule"] = module;
	jsonobject["LogMethod"] = method;
	jsonobject["LogEvent"] = event;
	jsonobject["LogStatus"] = status;

	QJsonDocument doc;
	doc.setObject(jsonobject);
	QByteArray content = doc.toJson(QJsonDocument::Compact);

	content.replace('\n', ' ');
	content.replace('\r', ' ');
	content.replace('\t', ' ');

	content += "\n";


	XFile::append(filepath + QDir::separator() + "log.jsonl", content, true);
	XFile::append(filepath + QDir::separator() + module + ".jsonl", content, true);
	XFile::append(filepath + QDir::separator() + method + ".jsonl", content, true);
	XFile::append(filepath + QDir::separator() + event + ".jsonl", content, true);
	XFile::append(filepath + QDir::separator() + status + ".jsonl", content, true);
}
