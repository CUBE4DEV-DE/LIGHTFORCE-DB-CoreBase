#include "XUser.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XUser
// Method:  init
//----------------------------------------------------------------------------- 
void XUser::init()
{
	XEntity::init();

	setUserName("");
	setMail("");
	setFirstName("");
	setSecondName("");

	setAdmin(false);
}

