#ifndef IRECENTFILESMANAGER_H
#define IRECENTFILESMANAGER_H

#include <qstring.h>

typedef QPair<QString, QString> QStringPair;

class IRecentFilesManager {
public:
    virtual ~IRecentFilesManager() = default;

    virtual void addPair(const QString &file1, const QString &file2) = 0;
    virtual void clear() = 0;
    virtual QList<QStringPair> getAllPairs() const = 0;
};

#endif // IRECENTFILESMANAGER_H
