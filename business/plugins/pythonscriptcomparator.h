#ifndef PYTHONSCRIPTCOMPARATOR_H
#define PYTHONSCRIPTCOMPARATOR_H

#include <domain/interfaces/business/icomparator.h>

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
    ComparisonResultVariantPtr compare(const ComparableImage &first,
                                       const ComparableImage &second) override;

private:
    const int mCharsInReportMax = 10000;
    QString mShortName;
    QString mHotkey;
    QString mDescription;
    QList<Property> mProperties;
    QString mFullName;
    bool misPartOfAutoReportingToolbox;
    QString mPyScriptPath;
    std::string mDefaultSaveImageExtention;

    optional<QString> validateText(QString &text);
};

#endif // PYTHONSCRIPTCOMPARATOR_H
