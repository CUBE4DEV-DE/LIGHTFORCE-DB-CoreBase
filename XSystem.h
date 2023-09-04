#pragma once

#include "XEntity.h"
#include "XTranslate.h"
#include "XUser.h"

namespace CoreBase
{

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XSystem
// Method:  XSystem
//----------------------------------------------------------------------------- 

class COREBASE_EXPORT XSystem : public XEntity
{
	Q_OBJECT
public:
	XBASEOBJECT(XSystem, XEntity)
	virtual void init();
	// Childs
public:
	XOBJECTLIST(XUser, user)
	XOBJECTLIST(XTranslate, translate)
};

};
using namespace CoreBase;
 