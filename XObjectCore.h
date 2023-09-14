#pragma once

#include "CoreBaseGlobal.h"

#include <QObject>
#include <QList>
#include <QMap>
#include <QHash>
#include <QPointer>
#include <QJSonObject>
#include <QJsonArray>
#include <QMutex>
#include <QReadLocker>
#include <QColor>
#include <QMetaObject>
#include <QReadWriteLock>

#include <QtQml/qjsvalue.h>


using namespace std;

#define XOBJECT(XCLASS)	\
    public:\
    ##XCLASS():XObject() {}  \
    virtual ~##XCLASS() {}  \
    ##XCLASS(const QMap<QString,QString>&map):XObject(map) {}  \
    ##XCLASS(const QMap<QString,QVariant>&map):XObject(map) {}  \
    ##XCLASS(const QJsonDocument&doc):XObject(doc) {}  \
    ##XCLASS(const QJsonObject&obj):XObject(obj) {}  \
    ##XCLASS(const QString&content):XObject(content) {}  \
	virtual XObject* clone() const { return objectClone<##XCLASS>(); }   

#define XBASEOBJECT(XCLASS,XCLASSBASE)	\
    public:\
    ##XCLASS():##XCLASSBASE() {}  \
    virtual ~##XCLASS() {}  \
    ##XCLASS(const QMap<QString,QString>&map):##XCLASSBASE(map) {}  \
    ##XCLASS(const QMap<QString,QVariant>&map):##XCLASSBASE(map) {}  \
    ##XCLASS(const QJsonDocument&doc):##XCLASSBASE(doc) {}  \
    ##XCLASS(const QJsonObject&obj):##XCLASSBASE(obj) {}  \
    ##XCLASS(const QString&content):##XCLASSBASE(content) {}  \
	virtual XObject* clone() const { return objectClone<##XCLASS>(); }   

#define XSTRING(XCLASS,XX)	\
	QString get##XX()	        		{ return getString(#XX); }          \
	void set##XX(const QString& val)	{ setString(#XX,val); }             \
	bool is##XX(const QString& val)		{ return isString(#XX,val); }       \
	##XCLASS & ##XX(const QString& val)	{ setString(#XX,val); return *this; }

#define XBOOL(XCLASS,XX)	\
	bool get##XX()	        		    { return getBool(#XX); }            \
	void set##XX(const bool val) 		{ setBool(#XX,val); }               \
    bool is##XX(const bool val)	    	{ return isBool(#XX,val); }         \
	##XCLASS & ##XX(const bool val)	    { setBool(#XX,val); return *this; }

#define XINT(XCLASS,XX)	\
	int get##XX()	        		    { return getInt(#XX); }            \
	void set##XX(const int val) 		{ setInt(#XX,val); }               \
    bool is##XX(const int val)	    	{ return isInt(#XX,val); }         \
	##XCLASS & ##XX(const int val)	    { setInt(#XX,val); return *this; }

#define XINTEGER(XCLASS,XX)	\
	qint64 get##XX()	        		{ return getInteger(#XX); }            \
	void set##XX(const qint64 val) 		{ setInteger(#XX,val); }               \
    bool is##XX(const qint64 val)	    { return isInteger(#XX,val); }         \
	##XCLASS & ##XX(const qint64 val)	{ setInteger(#XX,val); return *this; }

#define XDOUBLE(XCLASS,XX)	\
	double get##XX()	        		{ return getDouble(#XX); }            \
	void set##XX(const double val) 		{ setDouble(#XX,val); }               \
    bool is##XX(const double val)	    { return isDouble(#XX,val); }         \
	##XCLASS & ##XX(const double val)	{ setDouble(#XX,val); return *this; }

#define XCOLOR(XCLASS,XX)	\
	QColor get##XX()	        		{ return getColor(#XX); }            \
	void set##XX(const QColor&val) 	    { setColor(#XX,val); }               \
    bool is##XX(const QColor&val)	    { return isColor(#XX,val); }         \
	##XCLASS & ##XX(const QColor& val)  { setColor(#XX,val); return *this; }

#define XDATETIME(XCLASS,XX)	\
	QDateTime get##XX()	        		{ return getDateTime(#XX); }            \
	void set##XX(const QDateTime&val) 	{ setDateTime(#XX,val); }               \
    bool is##XX(const QDateTime&val)	{ return isDateTime(#XX,val); }         \
	##XCLASS & ##XX(const QDateTime& val) { setDateTime(#XX,val); return *this; }

#define XDATE(XCLASS,XX)	\
	QDate get##XX()	        		    { return getDate(#XX); }            \
	void set##XX(const QDate&val) 	    { setDate(#XX,val); }               \
    bool is##XX(const QDate&val)	    { return isDate(#XX,val); }         \
	##XCLASS & ##XX(const QDate& val)   { setDate(#XX,val); return *this; }

#define XTIME(XCLASS,XX)	\
	QTime get##XX()	        		    { return getTime(#XX); }            \
	void set##XX(const QTime&val) 	    { setTime(#XX,val); }               \
    bool is##XX(const QTime&val)	    { return isTime(#XX,val); }         \
	##XCLASS & ##XX(const QTime& val)   { setTime(#XX,val); return *this; }

#define XURL(XCLASS,XX)	\
	QUrl get##XX()	        		    { return getUrl(#XX); }            \
	void set##XX(const QUrl&val) 	    { setUrl(#XX,val); }               \
    bool is##XX(const QUrl&val)	        { return isUrl(#XX,val); }         \
	##XCLASS & ##XX(const QUrl& val)    { setUrl(#XX,val); return *this; }

#define XBYTEARRAYUTF8(XCLASS,XX)	\
	QByteArray get##XX()	        	{ return getByteArrayUtf8(#XX); }   \
	void set##XX(const QByteArray&val) 	{ setByteArrayUtf8(#XX,val); }      \
    bool is##XX(const QByteArray&val)	{ return isByteArrayUtf8(#XX,val); } \
	##XCLASS & ##XX(const QByteArray& val) { setByteArrayUtf8(#XX,val); return *this; }

#define XUUID(XCLASS,XX)	\
	QUuid get##XX()	        		    { return getUuid(#XX); }            \
	void set##XX(const QUuid&val) 	    { setUuid(#XX,val); }               \
    bool is##XX(const QUuid&val)	    { return isUuid(#XX,val); }         \
	##XCLASS & ##XX(const QUuid& val)   { setUuid(#XX,val); return *this; }

#define XPOINTER(XCLASS,XSUBCLASS,XX)	\
	void set##XX(const ##XSUBCLASS*val) { setObject<##XSUBCLASS>(#XX,val); }               \
    bool is##XX(const ##XSUBCLASS*val)	{ return isObject<##XSUBCLASS>(#XX,val); }         \
	##XSUBCLASS* get##XX()	        	{ return getObject<##XSUBCLASS>(#XX); }            \
	##XCLASS & ##XX(const QObject*val)  { setObject<##XSUBCLASS>(#XX,val); return *this; }

#define SXSCRIPTOBJECT()	\
    public:\
	    void setJSValue(QJSValue value) { m_JSValue = value; }\
        QJSValue& JSValue() { return m_JSValue; }\
    private:\
        QJSValue m_JSValue;

#define NEUTRAL(A)	/**/

#define SXSTRING(XCLASS,XX)	\
	QString get##XX()	        		{ return getString(#XX); }          \
	void set##XX(const QString& val)	{ setString(#XX,val); }             \
	bool is##XX(const QString& val)		{ return isString(#XX,val); }       \
	QJSValue NEUTRAL(1) ##XX(const QString& val) { setString(#XX, val); return JSValue(); }

#define SXBOOL(XCLASS,XX)	\
	bool get##XX()	        		    { return getBool(#XX); }            \
	void set##XX(const bool val) 		{ setBool(#XX,val); }               \
    bool is##XX(const bool val)	    	{ return isBool(#XX,val); }         \
	QJSValue NEUTRAL(1) ##XX(const bool& val) { setBool(#XX, val); return JSValue(); }

#define SXINT(XCLASS,XX)	\
	int get##XX()	        		    { return getInt(#XX); }            \
	void set##XX(const int val) 		{ setInt(#XX,val); }               \
    bool is##XX(const int val)	    	{ return isInt(#XX,val); }         \
	QJSValue NEUTRAL(1) ##XX(const int val) { setInt(#XX, val); return JSValue(); }

#define SXDOUBLE(XCLASS,XX)	\
	double get##XX()	        		{ return getDouble(#XX); }            \
	void set##XX(const double val) 		{ setDouble(#XX,val); }               \
    bool is##XX(const double val)	    { return isDouble(#XX,val); }         \
	QJSValue NEUTRAL(1)  ##XX(const double val) { setDouble(#XX, val); return JSValue(); }

#define SXCOLOR(XCLASS,XX)	\
	QColor get##XX()	        		{ return getColor(#XX); }            \
	void set##XX(const QColor&val) 	    { setColor(#XX,val); }               \
    bool is##XX(const QColor&val)	    { return isColor(#XX,val); }         \
	QJSValue NEUTRAL(1) ##XX(const QString& val) { setString(#XX, val); return JSValue(); }


namespace CoreBase
{
    class XObjectManager;
    class XObject;
    //-----------------------------------------------------------------------------
    // Author:  Tobias Post
    // Company: CUBE4DEV GmbH
    // Date:    03.04.2022
    // Context: DefaultNamespace
    // Class:   XObject
    // Method:  XObject
    // Description:
    //  XObjectCore is a C++ class that provides core functionality for the
    //  object model in a QObject / JSON hybrid programming paradigm.It
    //  includes various types of functions that handle maps, JSON documents,
    //  and objects.
    //  Some of its specific functions include : parsing a JSON object into
    //  a QMap<QString, QVariant>, cloning XObjects, setting and getting
    //  integer, double, and other types of values, and much more.Overall,
    //  it aims to streamline the creation of hybrid programming paradigms
    //  by codifying a consistent and predictable model.
    //----------------------------------------------------------------------------- 

    class COREBASE_EXPORT XObjectCore :public QObject
    {
        friend class XObjectManager;
        friend class XObjectFactory;
        friend class XObject;

        Q_OBJECT
    public:
        XObjectCore();
        virtual ~XObjectCore();

    public Q_SLOTS:
        inline void setNULL(const QString& key) {  { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(QJsonValue::Null)); } setChanged(); }

        inline bool hasValue(const QString& key) { { QWriteLocker lock(&m_PropertyLock); return m_Object.keys().contains(key); } }
    	inline void writeValue(const QString& key,const QVariant&value) { { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, value.toJsonValue()); }  }
        
    	inline void setInt(const QString& key, const int val) { if (isInt(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val)); } setChanged(); }
        inline int getInt(const QString& key) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toInt(); }
        inline bool isInt(const QString& key, const int& val) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toInt() == val; }

        inline void setBool(const QString& key, const bool val) { if (isBool(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val)); } setChanged(); }
        inline int getBool(const QString& key) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toBool(); }
        inline int isBool(const QString& key, const bool val) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toBool() == val; }

        inline void setInteger(const QString& key, const qint64 val) { if (isInteger(key, val)) return; QReadLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val)); }
        inline qint64 getInteger(const QString& key) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toInteger(); }
        inline bool isInteger(const QString& key, const qint64& val) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toInteger() == val; }

        inline void setDouble(const QString& key, const double val) { if (isDouble(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val)); } setChanged(); }
        inline double getDouble(const QString& key) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toDouble(); }
        inline bool isDouble(const QString& key, const double& val) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toDouble() == val; }

        inline void setString(const QString& key, const QString& val) { if (isString(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val)); } setChanged(); }
        inline QString getString(const QString& key) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toString(); }
        inline bool isString(const QString& key, const QString& val) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toString() == val; }

        inline void setUuid(const QString& key, const QUuid& val) { if (isUuid(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val.toString())); } setChanged(); }
        inline QUuid getUuid(const QString& key) const { QReadLocker lock(&m_PropertyLock); return QUuid(m_Object.value(key).toString()); }
        inline bool isUuid(const QString& key, const QUuid& val) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toString() == val.toString(); }

        inline void setDate(const QString& key, const QDate& val) { if (isDate(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val.toString(Qt::ISODate))); } setChanged(); }
        inline QDate getDate(const QString& key) const { QReadLocker lock(&m_PropertyLock); return QDate::fromString(m_Object.value(key).toString(), Qt::ISODate); }
        inline bool isDate(const QString& key, const QDate& val) const { QReadLocker lock(&m_PropertyLock); return QDate::fromString(m_Object.value(key).toString(), Qt::ISODate) == val; }

        inline void setDateTime(const QString& key, const QDateTime& val) { if (isDateTime(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val.toString(Qt::ISODate))); } setChanged(); }
        inline QDateTime getDateTime(const QString& key) const { QReadLocker lock(&m_PropertyLock); return QDateTime::fromString(m_Object.value(key).toString(), Qt::ISODate); }
        inline bool isDateTime(const QString& key, const QDateTime& val) const { QReadLocker lock(&m_PropertyLock); return QDateTime::fromString(m_Object.value(key).toString(), Qt::ISODate) == val; }

        inline void setTime(const QString& key, const QTime& val) { if (isTime(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val.toString(Qt::ISODate))); } setChanged(); }
        inline QTime getTime(const QString& key) const { QReadLocker lock(&m_PropertyLock); return QTime::fromString(m_Object.value(key).toString(), Qt::ISODate); }
        inline bool isTime(const QString& key, const QTime& val) const { QReadLocker lock(&m_PropertyLock); return QTime::fromString(m_Object.value(key).toString(), Qt::ISODate) == val; }

        inline void setColor(const QString& key, const QColor& val) { if (isColor(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val.name())); } setChanged(); }
        inline QColor getColor(const QString& key) const { QReadLocker lock(&m_PropertyLock); return QColor(m_Object.value(key).toString()); }
        inline bool isColor(const QString& key, const QColor& val) const { QReadLocker lock(&m_PropertyLock); return QColor(m_Object.value(key).toString()) == val; }

        inline void setUrl(const QString& key, const QUrl& val) { if (isUrl(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val.toString())); } setChanged(); }
        inline QUrl getUrl(const QString& key) const { QReadLocker lock(&m_PropertyLock); return QUrl(m_Object.value(key).toString()); }
        inline bool isUrl(const QString& key, const QUrl& val) const { QReadLocker lock(&m_PropertyLock); return QUrl(m_Object.value(key).toString()) == val; }

        inline void setByteArrayUtf8(const QString& key, const QByteArray& val) { if (isByteArrayUtf8(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(QString::fromUtf8(val))); } setChanged(); }
        inline QByteArray getByteArrayUtf8(const QString& key) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toString().toUtf8(); }
        inline bool isByteArrayUtf8(const QString& key, const QByteArray& val) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toString().toLatin1() == QString::fromUtf8(val); }

        inline void setObject(const QString& key, const QUuid& val) { if (isUuid(key, val)) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(val.toString())); } setChanged(); }
        inline QUuid getObject(const QString& key) const { QReadLocker lock(&m_PropertyLock); return QUuid(m_Object.value(key).toString()); }
        inline bool isObject(const QString& key, const QUuid& val) const { QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toString() == val.toString(); }
    public:
        template <class T> inline void setObject(const QString& key, T* obj) { if (obj == Q_NULLPTR) return; if (isUuid(key, obj->index())) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(obj->index().toString())); } setChanged(); }
        template <class T> inline T* getObject(const QString& key) const { if (objectManager() == Q_NULLPTR) return Q_NULLPTR; QReadLocker lock(&m_PropertyLock); return objectManager()->find<T>(QUuid(m_Object.value(key).toString())); }
        template <class T> inline bool isObject(const QString& key, T* obj) const { if (obj == Q_NULLPTR) return false; QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toString() == obj->index().toString(); }

        bool isValue(const QString& val) const;
        bool isSearch(const QString& val) const;

        bool isValue(const QString& key, const QString& val) const;
        bool isSearch(const QString& key, const QString& val) const;

    // Meta
    public:
        QString strIndex() const { return getString("index"); }
        QUuid index() const { return getUuid("index"); }
        void newIndex() { setUuid("index", QUuid::createUuid()); }
        void setIndex(const QUuid& idx) { setUuid("index", idx); }

        QString strIndexParent() const { return getString("parent"); }
        QUuid indexParent() const { return getUuid("parent"); }
        void setIndexParent(const QUuid& idx) { setUuid("parent", idx); }

        const QString& className() const;
    private:
        mutable QString m_ClassName;

	// SuperMeta
    public:
        void setSuperObjectName(const QString& name) { m_SuperObjectName = name; }
        const QString& superObjectName() const { return m_SuperObjectName; }
    private:
        mutable QString m_SuperObjectName;

    // Store
    public:
        inline bool isChanged() const { return m_Changed; }
        inline bool isStored() const { return m_Stored; }
        inline void setDateTime(const QString& key, const QDateTime& val, bool change) { if (isDateTime(key, val)) return; { QWriteLocker lock(&m_PropertyLock); if (change) setChanged(); m_Object.insert(key, QJsonValue(val.toString(Qt::ISODate))); } }
        inline void setInt(const QString& key, const int val, bool change) { if (isInt(key, val)) return; { QWriteLocker lock(&m_PropertyLock); if (change) setChanged(); m_Object.insert(key, QJsonValue(val)); } }
        inline void setChanged(bool change = true) { m_Changed = change; if (change) changed(); }
        inline void setStored(bool stored = true) { m_Stored = stored; }
        virtual void changed() {}
    private:
        bool m_Changed = false;
        bool m_Stored = false;

	// Property
    public:
        inline QJsonObject& object() { return m_Object; }
        inline const QJsonObject& readObject() const { return m_Object; }
        inline QStringList keys() const { return m_Object.keys(); }
        bool contains(const QString& key) const { return m_Object.contains(key); }
        QString value(const QString& key) const { return getString(key); }
        QString operator[](const QString& key) const { return value(key); }
        void insert(const QString& key, const QString& value) { setString(key,value); }

        QMap<QString, QString> toMap() const;
        void fromMap(const QMap<QString, QString>&map);
        QMap<QString, QVariant> toMapVariant() const;
        void fromMapVariant(const QMap<QString, QVariant>& map);

        QJsonDocument toDocument() const;
        void fromDocument(const QJsonDocument& document);
        QJsonObject toObject() const;
        void fromObject(const QJsonObject& object);
        void fromObject(const XObjectCore& object);
    private:
        mutable QReadWriteLock m_PropertyLock;
        QJsonObject m_Object;

    // Manager
    public:
        void setObjectManager(XObjectManager* objectManager) { m_ObjectManager = objectManager; }
        XObjectManager* objectManager() const { return m_ObjectManager; }
    private:
        XObjectManager* m_ObjectManager = Q_NULLPTR;

	// DefaultValues
    public:
        inline static const QString& emptyString() { return m_EmptyString; }
        inline static const QStringList& emptyStringList() { return m_EmptyStringList; }
    private:
        static QString m_EmptyString;
        static QStringList m_EmptyStringList;
    };

};
using namespace CoreBase;
