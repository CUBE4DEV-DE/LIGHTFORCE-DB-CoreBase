#include "XEntity.h"

#include "XDebug.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XEntity
// Method:  init
//----------------------------------------------------------------------------- 

void XEntity::init()
{
	XObject::init();

	setChanges(0);
	setCreated(QDateTime::currentDateTime());
	setChanged(QDateTime::currentDateTime());
	setActive(true);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XEntity
// Method:  changed
//----------------------------------------------------------------------------- 
void XEntity::changed()
{
	if (m_BlockChange)
		return;

	XObject::changed();

	m_BlockChange = true;
	setChanged(QDateTime::currentDateTime());
	setChanges(getChanges() + 1);
	m_BlockChange = false;
}
