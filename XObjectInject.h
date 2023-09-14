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
// Description:
// The XObjectInject class is a subclass of the XFunctionInject<XFunctionObject>
// class defined in the C++ library "CoreBase".The XObjectInject class is used to
// create an object injection system that provides a functional interface to be
// called during an application's runtime. The class has several methods that
// allow for functions to be called on an object, by either passing an object
// directly or by name. The calls method allows for multiple functions to be
// called on an object. The XObjectInject class can be used alongside the
// XFunctionInject class to provide an object-oriented approach to this inject system.
// By providing a name for functions, it becomes possible to call a specific
// function, which makes the implementation more flexible.
// The class can also be used to dynamically attach or detach functions
// to and from objects at any time during runtime.
// In short, the XObjectInject class is a useful tool for object injection
// during an application's runtime, allowing for the dynamic binding and
// detachment of functions to and from objects.
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
