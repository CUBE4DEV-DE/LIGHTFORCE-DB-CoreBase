#include "XSystem.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XSystem
// Method:  init
//----------------------------------------------------------------------------- 
void XSystem::init()
{
	XEntity::init();

	registerClassObjects(user());
	registerClassObjects(translate());
}

