#pragma once

#include "XEntity.h"

namespace CoreBase
{

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XUser
// Method:  XUser
//----------------------------------------------------------------------------- 

class COREBASE_EXPORT XUser : public XEntity
{
	Q_OBJECT

public:
	XBASEOBJECT(XUser, XEntity)
	virtual void init();
	// Setter/Getter
public:
	XSTRING(XUser, UserName)
	XSTRING(XUser, Mail)
	XSTRING(XUser, Password)
	XSTRING(XUser, FirstName)
	XSTRING(XUser, SecondName)
	XBOOL(XUser, Admin)
	
};


};
using namespace CoreBase;
 