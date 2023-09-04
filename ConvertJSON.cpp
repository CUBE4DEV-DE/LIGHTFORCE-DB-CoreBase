#include "ConvertJSON.h"

#include <QJsonDocument>
#include <QStringList>


/*
-- -
# <- yaml supports comments, json does not
    # did you know you can embed json in yaml ?
    # try uncommenting the next nextLine
# { foo: 'bar' }
json:
-rigid
- better for data interchange
yaml :
-slim and flexible
- better for configuration
object :
key: value
array :
    -null_value :
    -boolean : true
    - integer : 1
    - alias : &example aliases are like variables
    - alias : *example
    paragraph : >
    Blank lines denote

    paragraph breaks
    content : | -
    Or we
    can auto
    convert nextLine breaks
    to save space
    alias : &foo
    bar : baz
    alias_reuse : *foo


*/

/*
 {
  "Info": {
    "Name": "",
    "Description": "",
    "Author": "",
    "Created": "",
    "Changed": ""
  },

  "Forms": [
    {
      "Title": "Workflow A",
      "Name": "EDITUSER",
      "Columns": [
        {
          "Title": "Lane",
          "Name": "LANEROOT",
          "Columns": [
            {
              "Title": "Group A",
              "Name": "GROUP1",
              "Columns": [
                {
                  "Title": "Team A Lane1",
                  "Name": "LANE1",
                  "Columns": [
                    {
                      "Title": "Basics",
                      "Name": "BASICS",
                      "Values": [
                        {
                          "Title": "Title",
                          "TitleDE": "Titel",
                          "Name": "Title"
                        },
                        {
                          "Title": "Active",
                          "TitleDE": "Aktiv",
                          "Name": "Active",
                          "Type": "bool"
                        },
                        {
                          "Title": "Description",
                          "TitleDE": "Beschreibung",
                          "Name": "Description",
                          "Type": "richtext"
                        }
                      ]
                    }
                  ]
                },
                {
                  "Title": "Team B Lane2",
                  "Name": "LANE2",
                  "Columns": [
                    {
                      "Title": "Start",
                      "Name": "START2B"
                    },
                    {
                      "Title": "ToDo",
                      "Name": "TODO2B"
                    }

                  ]
                }

              ]

            },
            {
              "Title": "Group B",
              "Name": "GROUP2",
              "Columns": [
                {
                  "Title": "Team A Lane1",
                  "Name": "LANETEAMB",
                  "Columns": [

                    {
                      "Title": "Extra",
                      "Name": "EXTRA",
                      "Values": [
                        {
                          "Title": "Card",
                          "TitleDE": "Karte",
                          "Name": "Card",
                          "Type": "list",
                          "List": [
                            {
                              "Title": "Task",
                              "TitleDE": "Aufgabe",
                              "Name": "Task"
                            },
                            {
                              "Title": "Bug",
                              "TitleDE": "Fehler",
                              "Name": "Bug"
                            },
                            {
                              "Title": "Call",
                              "TitleDE": "Anruf",
                              "Name": "Call"
                            }
                          ]
                        },
                        {
                          "Title": "Created",
                          "TitleDE": "Erzeugt",
                          "Name": "Created",
                          "Type": "datetime"
                        }
                      ]
                    }
                  ]
                },
                {
                  "Title": "Team B Lane2",
                  "Name": "LANETEAMB",
                  "Columns": [

                    {
                      "Title": "Active",
                      "Name": "ACTIVE4A",
                      "Columns": [
                        {
                          "Title": "Basics",
                          "Name": "BASICS2",
                          "Values": [
                            {
                              "Title": "Title",
                              "TitleDE": "Titel",
                              "Name": "Title2"
                            },
                            {
                              "Title": "Active",
                              "TitleDE": "Aktiv",
                              "Name": "Active2",
                              "Type": "bool"
                            },
                            {
                              "Title": "Description",
                              "TitleDE": "Beschreibung",
                              "Name": "Description2",
                              "Type": "richtext"
                            }
                          ]
                        }
                      ]

                    },
                    {
                      "Title": "Finish",
                      "Name": "FINISH4B"
                    }
                  ]
                }

              ]

            },
            {
              "Title": "Group XX",
              "Name": "GROUP3",
              "Columns": [
                {
                  "Title": "Finish",
                  "Name": "FINISH5A",
                  "Values": [
                    {
                      "Title": "Title",
                      "TitleDE": "Titel",
                      "Name": "Title3"
                    },
                    {
                      "Title": "Active",
                      "TitleDE": "Aktiv",
                      "Name": "Active3",
                      "Type": "bool"
                    },
                    {
                      "Title": "Description",
                      "TitleDE": "Beschreibung",
                      "Name": "Description3",
                      "Type": "richtext"
                    }
                  ]
                },
                {
                  "Title": "Finish B",
                  "Name": "FINISH5B"
                },
                {
                  "Title": "X Finish",
                  "Name": "FINISH",
                  "Values": [
                    {
                      "Title": "Title",
                      "TitleDE": "Titel",
                      "Name": "Title4"
                    },
                    {
                      "Title": "Active",
                      "TitleDE": "Aktiv",
                      "Name": "Active4",
                      "Type": "bool"
                    },
                    {
                      "Title": "Active",
                      "TitleDE": "Aktiv",
                      "Name": "Active5",
                      "Type": "bool"
                    },
                    { 
                      "Title": "Active",
                      "TitleDE": "Aktiv",
                      "Name": "Active6",
                      "Type": "bool"
                    },
                    {
                      "Title": "Description",
                      "TitleDE": "Beschreibung",
                      "Name": "Description4",
                      "Type": "richtext"
                    }
                  ]
                },
                {
                  "Title": "X Archive",
                  "Name": "ARCHIVE"
                }
              ]
            }

          ]
        }

      ]
    }

  ]
}
 */


 /*

Info: 
	Name: 
	Description: 
	Author: 
	Created: 
	Changed: 

Forms:
[
	Title: Workflow A
	Name: EDITUSER
	Columns:
	[
		Title: Lane
		Name: LANEROOT
		Columns:
		[
			Title: Group A
			Name: GROUP1
			Columns:
			[
				Title: Team B Lane2
				Name: LANETEAMB
				Columns:
				[
					Title: Active
					Name: ACTIVE4A
					Columns:
					[
						Title: Basics
						Name: BASICS2
						Values:
						[
							Title: Title
							TitleDE: Titel
							Name: Title2

							Title: Active
							TitleDE: Aktiv
							Name: Active2
							Type: bool

							Title: Description
							TitleDE: Beschreibung
							Name: Description2
							Type: richtext
						]
					]
				]
			]
		]
	]
]

*/

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  convertMiniToJSON
// Params:  const QString& content
// Params:  
//----------------------------------------------------------------------------- 
QString ConvertJSON::convertMiniToJSON(const QString& content)
{
    ConvertJSON cnv;
    return QJsonDocument::fromJson(cnv.convert(content).toUtf8()).toJson();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  convert
// Params:  const QString& content
// Params:  
//----------------------------------------------------------------------------- 

void ConvertJSON::convertObject()
{
    Open();

    QStringList blockresult;
	while (!atEnd())
	{
        QString line = nextLine();
        bool comma = false;
		if (line.contains(':'))
		{
            QStringList blocks = line.split(',');
            for (const QString& block : blocks)
            {
                if (block.contains(':'))
                {
                    QStringList attribute = block.split(':');
                    if (attribute.count() >= 2)
                    {
                        const QString name = attribute[0];
                        const QString value = attribute[1];
                        if (comma) write(",");
                        comma = true;
                        write("\"" + name + "\":\"" + value + "\"");
                    }
                }
            }
		}
        else if (line.contains('['))
        {
	        
        }
        else if (line.contains('['))
        {
	        
        }
	}


    Close();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  convert
// Params:  const QString& content
// Params:  
//----------------------------------------------------------------------------- 

QString ConvertJSON::convert(const QString & content)
{
    m_Lines = QString(content).split('\n');
    Open();

	convertObject();

    Close();
	return m_Result.join('\n');
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  nextLine
// Params:  
//----------------------------------------------------------------------------- 
QString ConvertJSON::nextLine()
{
    if (m_Index < m_Lines.count())
        return QString();

    QString nextline = comment(m_Lines[m_Index++]);
    while (nextline.trimmed().isEmpty())
    {
        nextline=comment(m_Lines[m_Index++]);
    }
    return nextline;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  atEnd
//----------------------------------------------------------------------------- 
bool ConvertJSON::atEnd()
{
	return m_Index >= m_Lines.count();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  comment
// Params:  const QString& nextLine
//----------------------------------------------------------------------------- 
QString ConvertJSON::comment(const QString& line)
{
    if (line.startsWith("#")||line.startsWith("//"))
        return QString();
    return line;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  write
// Params:  const QString& line
//----------------------------------------------------------------------------- 

void ConvertJSON::write(const QString& line)
{
    if (line.trimmed().isEmpty())
        return;

    m_Open = false;
    m_Close = false;
    m_Result << line;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  Open
//----------------------------------------------------------------------------- 
void ConvertJSON::Open()
{
    if(m_Close)
        m_Result << ",";
    m_Open = true;
    m_Close = false;
    m_Result << "{";
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    09.01.2023
// Context: DefaultNamespace
// Class:   ConvertJSON
// Method:  Close
//----------------------------------------------------------------------------- 

void ConvertJSON::Close()
{
    m_Open = false;
    m_Close = true;
    m_Result << "}";
}


