#pragma once

#include "XObjectCore.h"

namespace CoreBase
{
    //-----------------------------------------------------------------------------
    // Author:  Tobias Post
    // Company: CUBE4DEV GmbH
    // Date:    03.04.2022
    // Context: DefaultNamespace
    // Class:   XObject
    // Method:  XObject
    // Description: XObjectFiles is a class that provides methods for reading
    //  and writing files and caching their contents to improve performance.
    //  It also provides methods for creating and using shortcuts to quickly
    //  access frequently used files. The class is part of the CoreBase
    //  namespace and can be used in C++ projects.
    //----------------------------------------------------------------------------- 

    class COREBASE_EXPORT XObjectFiles
    {
	// Files
    public:
        static QString read(const QString& filename);
        static QString read(const QString& path, const QString& filename);
        static void write(const QString& filename, const QString& content);
        static const QString& readCache(const QString& filename);
        static void clearCache();
    private:
        static QCache<QString, QString> m_ContentCache;
        static QReadWriteLock m_CacheLock;

    // Shortcut
    public:
        static void shortCut(const QString& shortcut, const QString& content);
        static const QString& shortCutRead(const QString& shortcut, const QString& filename);
        static const QString& shortCutRead(const QString& shortcut, const QString& path, const QString& filename);
        static QStringList shortCutDirectory(const QString& path);
        static const QString& shortCut(const QString& shortcut);
        static bool hasShortCut(const QString& shortcut);
        static void clearShortCuts();
    private:
        static QMap<QString, QString> m_ShortCuts;
        static QReadWriteLock m_ShortCutLock;
    };


};
using namespace CoreBase;
