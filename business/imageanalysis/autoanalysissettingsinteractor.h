#ifndef AUTOANALYSISSETTINGSINTERACTOR_H
#define AUTOANALYSISSETTINGSINTERACTOR_H

#include <QList>
#include <QString>
#include <QSettings>

class IComparator;

class AutoAnalysisSettingsInteractor {
public:
    QList<std::shared_ptr<IComparator> > getComparators() const;
    void saveComparatorState(const QString& shortName, bool isEnabled);
};

#endif // AUTOANALYSISSETTINGSINTERACTOR_H
