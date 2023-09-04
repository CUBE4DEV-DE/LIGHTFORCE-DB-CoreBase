#pragma once

#include "XObject.h"

namespace CoreBase
{

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.04.2022
// Context: DefaultNamespace
// Class:   XTranslate
// Method:  XTranslate
//----------------------------------------------------------------------------- 
class COREBASE_EXPORT XTranslate : public XObject
{
	Q_OBJECT
public:
	XOBJECT(XTranslate)
	// Setter/Getter
public:

	XSTRING(XTranslate, Name)
	XSTRING(XTranslate, TextDE)
	XSTRING(XTranslate, TextEN)
	XSTRING(XTranslate, TextIT)
	XSTRING(XTranslate, TextFR)
	XSTRING(XTranslate, TextES)
	XSTRING(XTranslate, TextNL)

	QString getText(const QString& language) const { return getString("Text"+language); }
	void setText(const QString& language,const QString& val) { setString("Text" + language, val); }
	bool isText(const QString& language, const QString& val) { return isString("Text" + language, val); }
	XTranslate& Text(const QString& language, const QString& val) { setString("Text" + language, val); return *this; }

};

 

};
using namespace CoreBase;
