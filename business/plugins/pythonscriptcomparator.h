#ifndef PYTHONSCRIPTCOMPARATOR_H
#define PYTHONSCRIPTCOMPARATOR_H

#include <domain/interfaces/comparator.h>

class PythonScriptComparator : public IComparator {
public:
    PythonScriptComparator(const QString& pyScriptPath,
                           const QString& shortName,
                           const QString& hotkey,
                           const QString& description,
                           const QList<Property>& properties,
                           const QString& fullName,
                           bool isPartOfAutoReportingToolbox
                           );

    virtual ~PythonScriptComparator() = default;

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property> properties) override;
    QString getFullName() const override;
    bool isPartOfAutoReportingToolbox() override;
    shared_ptr<ComparisonResultVariant> compare(ComparableImage first,
                                                ComparableImage second
                                                ) override;

private:
    QString shortName;
    QString hotkey;
    QString description;
    QList<Property> properties;
    QString fullName;
    bool m_isPartOfAutoReportingToolbox;
    QString pyScriptPath;
};

#endif // PYTHONSCRIPTCOMPARATOR_H
