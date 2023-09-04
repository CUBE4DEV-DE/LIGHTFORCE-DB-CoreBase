#pragma once
#include "XObject.h"
#include "XObjectList.h"

namespace CoreBase
{

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XEntity
// Method:  XEntity
//----------------------------------------------------------------------------- 

class COREBASE_EXPORT XEntity :public XObject
{
	Q_OBJECT
public:
	XOBJECT(XEntity)
	virtual void init();

public:
	XDATETIME(XEntity, Created)
	XDATETIME(XEntity, Changed)
	XBOOL(XEntity, Active)
	XINT(XEntity, Changes)
protected:
	virtual void changed();
	bool m_BlockChange = true;
};


};
using namespace CoreBase;
