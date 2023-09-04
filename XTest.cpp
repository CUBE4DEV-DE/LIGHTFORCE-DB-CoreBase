#include "XTest.h"

#include <QFile>
#include <QMutex>
#include <QMetaMethod>

#include "XDebug.h"


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XTest
//----------------------------------------------------------------------------- 

bool XTest::m_GlobalOk=true;
int XTest::m_GlobalOkCount=0;
int XTest::m_GlobalCount=0;

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  XTest
//----------------------------------------------------------------------------- 
XTest::XTest()
{
	m_Ok = true;
	m_CallOk = true;

	m_OkCount=0;
	m_Count=0;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  ~XTest
//----------------------------------------------------------------------------- 
XTest::~XTest()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  run
//----------------------------------------------------------------------------- 
bool XTest::run()
{
	QString testname(metaObject()->className());

	qDebug() << "";
	qDebug() << "";

	qDebug() << ("------------------------------------------------------------------------------------------------------------");
	qDebug() << testname;
	qDebug() << ("------------------------------------------------------------------------------------------------------------");
	{
		XDebugStack stack(testname.toLocal8Bit(), "Test");

		m_Count = 0;
		m_OkCount = 0;

		{
			QMutexLocker lock(&m_Mutex);
			XDebugStack stack(testname.toLocal8Bit(), "TestStart");
			TestStart();
		}

		for (int i = 0; i < metaObject()->methodCount(); i++)
		{
			QMetaMethod slotmethod = metaObject()->method(i);

			QString methodname(slotmethod.methodSignature());

			if (methodname.indexOf("Test") == 0)
			{
				if (methodname.indexOf("(") > 0)
					methodname = methodname.left(methodname.indexOf("("));

				{
					QMutexLocker lock(&m_Mutex);
					TestInit();
				}

				qDebug() << "";
				qDebug() << QString("Test %1").arg(m_Count+1);

				{
					m_CallOk = true;

					QMutexLocker lock(&m_Mutex);
					XDebugStack stack(testname.toLocal8Bit(), methodname.toLocal8Bit());
					if (QMetaObject::invokeMethod(this, methodname.toLocal8Bit(), Qt::DirectConnection))
					{
						m_Count++;

						if (!m_CallOk)
							m_Ok = false;

						if (m_CallOk)
							m_OkCount++;

						if (m_CallOk)
							qDebug() << ("*** OK ***");
						else
							qDebug() << ("!!! FAILED !!!");



						m_GlobalCount++;

						if (!m_CallOk)
							m_GlobalOk = false;

						if (m_CallOk)
							m_GlobalOkCount++;

					}

				}

				{
					QMutexLocker lock(&m_Mutex);
					TestClear();
				}
				qDebug() << "";
			}
		}

		{
			QMutexLocker lock(&m_Mutex);
			XDebugStack stack(testname.toLocal8Bit(), "TestEnd");
			TestEnd();

		}
	}
	qDebug() << ("------------------------------------------------------------------------------------------------------------");
	qDebug() << testname;
	qDebug() << ("------------------------------------------------------------------------------------------------------------");

	int ok= m_OkCount;
	O_PARAM(ok);

	int failed= m_Count - m_OkCount;
	O_PARAM(failed);

	int count= m_Count;
	O_PARAM(count);

	if (m_Ok)
		qDebug() << ("*** OK *****************************************************************************************************");
	else
		qDebug() << ("!!! FAILED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

	qDebug() << ("------------------------------------------------------------------------------------------------------------");
	qDebug() << "";
	qDebug() << "";

	return m_Ok;
}
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  TestStart
//----------------------------------------------------------------------------- 
void XTest::TestStart()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  TestInit
//----------------------------------------------------------------------------- 

void XTest::TestInit()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  TestClear
//----------------------------------------------------------------------------- 

void XTest::TestClear()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    07.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  TestEnd
//----------------------------------------------------------------------------- 
void XTest::TestEnd()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  globalResult
//----------------------------------------------------------------------------- 

void XTest::globalResult()
{
	qDebug() << ("------------------------------------------------------------------------------------------------------------");
	qDebug() << "GLOBAL";
	qDebug() << ("------------------------------------------------------------------------------------------------------------");

	int ok = m_GlobalOkCount;
	O_PARAM(ok);

	int failed = m_GlobalCount - m_GlobalOkCount;
	O_PARAM(failed);

	int count = m_GlobalCount;
	O_PARAM(count);

	if (m_GlobalOk)
		qDebug() << ("*** OK *****************************************************************************************************");
	else
		qDebug() << ("!!! FAILED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

	qDebug() << ("------------------------------------------------------------------------------------------------------------");
	qDebug() << "";
	qDebug() << "";

}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  failed
// Params:  const QString& message
//----------------------------------------------------------------------------- 
void XTest::failed(const QString& message)
{
	m_CallOk = false;
	qDebug() << message;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  read
// Params:  const QString& filename
//----------------------------------------------------------------------------- 

QString XTest::read(const QString& filename)
{
	QString doc;

	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		failed("failed - read file " + filename);
		return "";
	}

	return file.readAll();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  remove
// Params:  const QString& filename
//----------------------------------------------------------------------------- 
void XTest::remove(const QString& filename)
{
	QFile::remove(filename);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XTest
// Method:  exist
// Params:  const QString& filename
// Params:  
//----------------------------------------------------------------------------- 

bool XTest::exist(const QString& filename)
{
	if (QFile::exists(filename))
		return true;

	return false;
}

