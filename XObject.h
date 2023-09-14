#pragma once

#include "XObjectCore.h"

namespace CoreBase
{
    class XObjectManager;
    //-----------------------------------------------------------------------------
    // Author:  Tobias Post
    // Company: CUBE4DEV GmbH
    // Date:    03.04.2022
    // Context: DefaultNamespace
    // Class:   XObject
    // Method:  XObject
    // Description: The XObject class is from the CoreBase-namespace and is part of
    //  the XObjectManager. This class has multiple constructors that take various
    //  types of input, allowing it to be initialized in multiple ways. It also has
    //  methods for initialization and setting table structures that are used in object
    //  management. In addition, it has methods for checking if a specified slot
    //  exists and calling the slot. This class provides a foundation for managing
    //  other objects and their data in the system.
    //----------------------------------------------------------------------------- 

    class COREBASE_EXPORT XObject:public XObjectCore
    {
        friend class XObjectManager;
        friend class XObjectFactory;

        Q_OBJECT
    public:
        XObject();
        XObject(const QMap<QString,QString>&map);
        XObject(const QMap<QString, QVariant>& map);
        XObject(const QJsonDocument& doc);
        XObject(const QJsonObject& obj);
        XObject(const QString& content);
        virtual ~XObject();

        template <class T> T* castClone() const;

        // Init
    protected:
        virtual void init();
        virtual void initMeta();

        // Construct
    protected:
        template <class T> T* objectClone() const;
        
        // Cast
    public:
        virtual XObject* clone() const { return objectClone<XObject>(); }
        template <class T> static T* cast(XObject* obj) { return dynamic_cast<T*>(obj); }
        template <class T> T* cast() { return dynamic_cast<T*>(this); }

        template <class T> static QString rawClassName() { T obj; return obj.className(); }

        // Property Setter/Getter
    public:
        template <class T> inline void setObject(const QString& key, T* obj) { if (obj == Q_NULLPTR) return; if (isUuid(key, obj->index())) return; { QWriteLocker lock(&m_PropertyLock); m_Object.insert(key, QJsonValue(obj->index().toString())); } setChanged(); }
        template <class T> inline T* getObject(const QString& key) const { if (objectManager() == Q_NULLPTR) return Q_NULLPTR; QReadLocker lock(&m_PropertyLock); return objectManager()->find<T>(QUuid(m_Object.value(key).toString())); }
        template <class T> inline bool isObject(const QString& key, T* obj) const { if (obj == Q_NULLPTR) return false; QReadLocker lock(&m_PropertyLock); return m_Object.value(key).toString() == obj->index().toString(); }

        // Property Chain
    public:
        inline XObject& Int(const QString& key, const int val) { setInt(key, val); return *this; }
        inline XObject& Double(const QString& key, const double val) { setDouble(key, val); return *this; }
        inline XObject& Bool(const QString& key, const bool val) { setBool(key, val); return *this; }
        inline XObject& String(const QString& key, const QString& val) { setString(key, val); return *this; }
        inline XObject& DateTime(const QString& key, const QDateTime& val) { setDateTime(key, val); return *this; }
        inline XObject& Date(const QString& key, const QDate& val) { setDate(key, val); return *this; }
        inline XObject& Time(const QString& key, const QTime& val) { setTime(key, val); return *this; }
        inline XObject& Color(const QString& key, const QColor& val) { setColor(key, val); return *this; }
        inline XObject& UUid(const QString& key, const QUuid& val) { setUuid(key, val); return *this; }
        
        template <class T> inline XObject& Object(const QString& key, T* obj) { setObject<T>(key, obj); return *this; }

    public:
        bool hasCall(const QString& slotname);
        bool call(const QString& slotname);
        bool call(const QString& slotname, XObject* values);

        // Parent
    public:
        void setObjectParent(XObject* objectParent) { m_ObjectParent = objectParent; }
        XObject* objectParent() const { return m_ObjectParent; }
    private:
        QPointer<XObject> m_ObjectParent = Q_NULLPTR;

    // ParentList
    public:
        void setObjectParentList(XObject* objectParentList) { m_ObjectParentList = objectParentList; }
        XObject* objectParentList() const { return m_ObjectParentList; }
    private:
        QPointer<XObject> m_ObjectParentList = Q_NULLPTR;

    // Childs
    public:
        virtual QList<QPointer<XObject> > childObjects() const { return QList<QPointer<XObject> >(); }
        XObject* newChildObject(const QString& classname, const QUuid& idx);
        virtual XObject* newChildObject(const QUuid& idx) { return Q_NULLPTR;  }
        QList<QPointer<XObject> > classObjectLists() { return m_ClassObjects.values(); }
        QList<XObject*> childAllObjects() const;
        virtual const QString& childClassName() const { return emptyString(); }
        virtual void clearAllObjects() {}
    protected:
        void registerClassObjects(XObject& objectlist);
    private:
        QMap<QString,QPointer<XObject> > m_ClassObjects;
    };

    //-----------------------------------------------------------------------------
    // Author:  Tobias Post
    // Company: CUBE4DEV GmbH
    // Date:    05.04.2022
    // Context: DefaultNamespace
    // Class:   XObject
    // Method:  objectClone
    //----------------------------------------------------------------------------- 

    template <class T>
    T* XObject::objectClone() const
    {
        T* obj = new T();
        if (obj)
        {
            //obj->object() = readObject();
            return obj;
        }
        return Q_NULLPTR;
    }

    //-----------------------------------------------------------------------------
    // Author:  Tobias Post
    // Company: CUBE4DEV GmbH
    // Date:    30.05.2022
    // Context: DefaultNamespace
    // Class:   XObject
    // Method:  castClone
    //----------------------------------------------------------------------------- 
    
    template <class T>
    T* XObject::castClone() const
    {
        T* obj = new T();
        if (obj)
        {
            obj->object() = m_Object;
            return obj;
        }
        return Q_NULLPTR;
    }

};
using namespace CoreBase;
