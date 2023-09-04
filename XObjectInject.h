#pragma once

#include "XObjectCore.h"
#include <QMap>
#include "XObject.h"

#include "../../Core/CoreStorage/XFunctionInject.h"

namespace CoreBase
{
typedef std::function<void(XObject* object)> XFunctionObject;


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    16.02.2023
// Context: DefaultNamespace
// Class:   XFunctionInject
// Method:  XObjectInject
//----------------------------------------------------------------------------- 
class COREBASE_EXPORT XObjectInject :public  XFunctionInject<XFunctionObject>
{
public:
	void call(XObject* object)
	{
		if (function())
			function()(object);
	}
	void call(const QString& name, XObject* object)
	{
		if (function(name))
			function(name)(object);
	}
	void calls(XObject* object)
	{
		call(object);
		for (XFunctionObject f : functions())
			f(object);
	}
	void calls(const QString& name, XObject* object)
	{
		call(name, object);

		for (XFunctionObject f : functions(name))
			f(object);
	}
};

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    16.02.2023
// Context: DefaultNamespace
// Class:   XObjectInject
// Method:  XObjectsInject
//----------------------------------------------------------------------------- 

class COREBASE_EXPORT XObjectsInject:public  XFunctionInject<XFunctionObject>
{
public:
	static void inject(const QString& name, XFunctionObject function);
	static void injects(const QString& name, XFunctionObject function);
	static bool hasObjectFunction(const QString& name);
	static bool hasObjectFunctions(const QString& name);
	static void call(const QString& name, XObject* object);
	static void calls(const QString& name, XObject* object);
private:
	static XObjectInject m_ObjectInjects;
};


};
using namespace CoreBase;
