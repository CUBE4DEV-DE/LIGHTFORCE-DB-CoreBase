#pragma once


#include "XObject.h"
#include "XObjectList.h"

namespace CoreBase
{
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  XFilter
// Description: The XFilter class is a templates class that is used to
//	filter a list of QObject's or objects that inherit from QObject. The
//	class provides a set of methods to sort and search this list of objects
//	based on specific properties. When creating an instance of the XFilter
//	class, a QList or XObjectList has to be provided in its constructor.
//	This instance is then later used by the methods of the class to apply
//	the different filters.
//	The available methods include sort, which sorts the objects based on
//	a specific property, search which returns a list of objects that match
//	the provided search string, and filter which combines searching with a
//	filter.
//----------------------------------------------------------------------------- 
template <class T>
class XFilter
{

public:
	XFilter(QList<QPointer<T> >&childobjects);
	XFilter(QList<T*>& childobjects);
	XFilter(XObjectList<T>& objectlist);
	XFilter(XObjectManager& objectmanager);

	virtual ~XFilter();

	const QList<QPointer<T> >& all();
	QList<QPointer<T> > sort(const QString&sortproperty);
	QList<QPointer<T> > search(const QString&search);
	QList<QPointer<T> > search(const QString&search, const QString&sortproperty);

	QList<QPointer<T> > filter(const QString& filterproperty,const QString& search);
	QList<QPointer<T> > filter(const QString& filterproperty,const QString& search, const QString& sortproperty);

private:
	QList<QPointer<T> >* m_ChildObjects;
	QList<QPointer<T> > m_ManagerChildObjects;
};

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  XFilter
// Params:  QList<QPointer<T>>& childobjects
//----------------------------------------------------------------------------- 

template <class T>
XFilter<T>::XFilter(QList<QPointer<T>>& childobjects)
{
	m_ChildObjects = &childobjects;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    02.12.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  XFilter
// Params:  QList<T*>& childobjects
//----------------------------------------------------------------------------- 

template <class T>
XFilter<T>::XFilter(QList<T*>& childobjects)
{
	m_ChildObjects = &childobjects;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  XFilter
// Params:  CoreBase::XObjectList<T>& objectlist
//----------------------------------------------------------------------------- 

template <class T>
XFilter<T>::XFilter(XObjectList<T>& objectlist)
{
	m_ChildObjects = &objectlist.objects();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    05.07.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  XFilter
// Params:  CoreBase::XObjectManager& objectmanager
//----------------------------------------------------------------------------- 

template <class T> XFilter<T>::XFilter(XObjectManager& objectmanager)
{
	for (T* f : objectmanager.allObjects<T>())
	{
		m_ManagerChildObjects.append(f);
	}
	m_ChildObjects = &m_ManagerChildObjects;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  ~XFilter
//----------------------------------------------------------------------------- 
template <class T> XFilter<T>::~XFilter()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  all
//----------------------------------------------------------------------------- 

template <class T>
const QList<QPointer<T>>& XFilter<T>::all()
{
	return m_ChildObjects;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  sort
// Params:  const QString& sortproperty
// Params:  
//----------------------------------------------------------------------------- 

template <class T>
QList<QPointer<T>> XFilter<T>::sort(const QString& sortproperty)
{
	QMultiMap<QString, QPointer<T>> map;

	for(T* obj:*m_ChildObjects)
	{
		if (obj)
			map.insert(obj->getString(sortproperty), obj);
	}

	QList<QPointer<T>> list;

	for (const QString& key:map.uniqueKeys())
	{
		for(T* obj:map.values(key))
		{
			list.prepend(obj);
		}
	}
	return list;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  search
// Params:  const QString& search
//----------------------------------------------------------------------------- 

template <class T>
QList<QPointer<T>> XFilter<T>::search(const QString& search)
{
	QList<QPointer<T>> list;
	for(T*obj:*m_ChildObjects)
	{
		if (obj && obj->isSearch(search))
		{
			list.append(obj);
		}
	}
	return list;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  search
// Params:  const QString& search
// Params:  const QString& sort
//----------------------------------------------------------------------------- 

template <class T>
QList<QPointer<T>> XFilter<T>::search(const QString& search, const QString& sortproperty)
{
	QMultiMap<QString,QPointer<T>> map;

	for(T* obj:*m_ChildObjects)
	{
		if (obj&&obj->isSearch(search))
			map.insert(obj->getString(sortproperty),obj);
	}

	QList<QPointer<T>> list;

	for(const QString&key:map.keys())
	{
		for(T*obj:map.values(key))
		{
			list.append(obj);
		}
	}
	return list;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  filter
// Params:  const QString& filterproperty
// Params:  const QString& search
// Params:  
//----------------------------------------------------------------------------- 

template <class T>
QList<QPointer<T>> XFilter<T>::filter(const QString& filterproperty, const QString& search)
{
	QList<QPointer<T>> list;
	for (T* obj : *m_ChildObjects)
	{
		if (obj && obj->isSearch(filterproperty, search))
		{
			list.append(obj);
		}
	}
	return list;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    08.04.2022
// Context: DefaultNamespace
// Class:   XFilter
// Method:  filter
// Params:  const QString& filterproperty
// Params:  const QString& search
// Params:  const QString& sortproperty
// Params:  
//----------------------------------------------------------------------------- 

template <class T>
QList<QPointer<T>> XFilter<T>::filter(const QString& filterproperty, const QString& search,const QString& sortproperty)
{
	QMultiMap<QString, QPointer<T>> map;

	for(T* obj:*m_ChildObjects)
	{
		if (obj && obj->isSearch(filterproperty,search))
			map.insert(obj->getString(sortproperty), obj);
	}

	QList<QPointer<T>> list;

	for (const QString& key:map.keys())
	{
		for (T* obj:map.values(key))
		{
			list.append(obj);
		}
	}
	return list;
}

};
using namespace CoreBase;
