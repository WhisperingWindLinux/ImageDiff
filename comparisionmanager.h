#ifndef COMPARISIONMANAGER_H
#define COMPARISIONMANAGER_H

#include "interfaces/acomporator.h"

struct ComporatorInfo {
  ComporatorInfo(QString name, QString description, QString hotkey, ComporatorContentType contentType)
      : name(name), description(description),
        hotkey(hotkey), contentType(contentType) {}
  QString name;
  QString description;
  QString hotkey;
  ComporatorContentType contentType;
};

class ComparisionManager
{
public:
    static ComparisionManager *instance();
    AComparator *findComporator(QString name);
    QList<ComporatorInfo> allComporatorsInfo();

private:
    QList<AComparator*> comporators;

    static ComparisionManager *manager;

    ComparisionManager();
    void addComparator(AComparator *comporator);
    void removeComporator(QString name);
};

#endif // COMPARISIONMANAGER_H
