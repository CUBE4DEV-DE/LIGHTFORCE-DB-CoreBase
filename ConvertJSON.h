#pragma once
#include <QString>
#include <QStringList>

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  ConvertJSON
//----------------------------------------------------------------------------- 

class ConvertJSON
{
public:
	ConvertJSON()
	{
		m_Index = 0;
		m_Open = false;
		m_Close = false;
		m_Indent = 0;
	}

	static QString convertMiniToJSON(const QString& content);
	void convertObject();
private:
	QString convert(const QString& content);
	QString nextLine();
	bool atEnd();
	QString comment(const QString& line);
	void write(const QString& line);
	void Open();
	void Close();

private:
	QStringList m_Result;
	QStringList m_Lines;

	qint32 m_Index;
	bool m_Open;
	bool m_Close;

	qint32 m_Indent;
};


