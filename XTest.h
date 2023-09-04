#pragma once

#include "CoreBaseGlobal.h"

#include <QMutex>
#include <QObject>

namespace CoreBase
{
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  XTest
//----------------------------------------------------------------------------- 

class COREBASE_EXPORT XTest:public QObject
{
	Q_OBJECT
public:
	XTest();
	virtual ~XTest();

	bool run();

	virtual void TestStart();

	virtual void TestInit();
	virtual void TestClear();

	virtual void TestEnd();

	static void globalResult();
public slots:
	//void Test01() {}
	//void Test02() {}
protected:
	void failed() { m_CallOk = false;  }
	void failed(const QString& message);
	void ok() { m_CallOk = true; }

	QString read(const QString& filename);
	void remove(const QString& filename);
	bool exist(const QString& filename);
private:
	bool m_CallOk;
	bool m_Ok;
	QMutex m_Mutex;

	int m_OkCount;
	int m_Count;

	static bool m_GlobalOk;
	static int m_GlobalOkCount;
	static int m_GlobalCount;
};

};
using namespace CoreBase;