#ifndef PYTHONSCRIPFILTER_H
#define PYTHONSCRIPFILTER_H

#include <domain/interfaces/business/ifilter.h>


class PythonScripFilter : public IFilter {
public:
    PythonScripFilter(const QString& pyScriptPath,
                      const QString& shortName,
                      const QString& hotkey,
                      const QString& description,
                      const QList<Property>& properties
                      );

    virtual ~PythonScripFilter() = default;

    QString getShortName() const override;
    QString getFullName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property> properties) override;
    QImage filter(const QImage &image) override;

private:
    QString mShortName;
    QString mHotkey;
    QString mDescription;
    QList<Property> mProperties;
    QString mPyScriptPath;
    std::string mDefaultSaveImageExtention;

    const QImage &prepareResult(const QImage &resultImage, const QImage &originalImage);
};

#endif // PYTHONSCRIPFILTER_H
