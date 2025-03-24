#ifndef PYTHONSCRIPFILTER_H
#define PYTHONSCRIPFILTER_H

#include <domain/interfaces/filter.h>


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
    QString getHotkey() const override;
    QString getDescription() const override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property> properties) override;
    QImage filter(const QImage &image) override;

private:
    QString shortName;
    QString hotkey;
    QString description;
    QList<Property> properties;
    QString pyScriptPath;
    std::string defaultSaveImageExtention;
};

#endif // PYTHONSCRIPFILTER_H
