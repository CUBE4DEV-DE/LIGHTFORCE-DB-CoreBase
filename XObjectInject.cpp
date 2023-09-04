#include "XObjectInject.h"

XObjectInject XObjectsInject::m_ObjectInjects;

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
void XObjectsInject::inject(const QString& name, XFunctionObject function)
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

void XObjectsInject::injects(const QString& name, XFunctionObject function)
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
bool XObjectsInject::hasObjectFunction(const QString& name)
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
bool XObjectsInject::hasObjectFunctions(const QString& name)
{
	return m_ObjectInjects.hasFunctions(name);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    16.02.2023
// Context: DefaultNamespace
// Class:   XFunctionInject
// Method:  call
// Params:  const QString& name
// Params:  CoreBase::XObject* object
//----------------------------------------------------------------------------- 
void XObjectsInject::call(const QString& name, XObject* object)
{
	return m_ObjectInjects.call(name, object);
}
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    16.02.2023
// Context: DefaultNamespace
// Class:   XFunctionInject
// Method:  call
// Params:  const QString& name
// Params:  CoreBase::XObject* object
//----------------------------------------------------------------------------- 
void XObjectsInject::calls(const QString& name, XObject* object)
{
	return m_ObjectInjects.calls(name, object);

}
