#ifndef PROPERTY_H
#define PROPERTY_H

#include <qstring.h>
#include <QStringList>

using namespace std;

class Property {
public:
    enum class Type { Integer, Real, Alternatives, FilePath };

    // FIXME Remove this dependency by refactoring the HelpHtmlFormatter class.
    friend class HelpHtmlFormatter;
    
    static Property createIntProperty(const QString &propertyName,
                                      const QString &propertyDescription,
                                      int defaultValue
                                      );
    
    static Property createIntProperty(const QString &propertyName,
                                      const QString &propertyDescription,
                                      int defaultValue,
                                      int minValue,
                                      int maxValue
                                      );
    
    static Property createRealProperty(const QString &propertyName,
                                       const QString &propertyDescription,
                                       int defaultValue
                                       );
    
    static Property createRealProperty(const QString &propertyName,
                                       const QString &propertyDescription,
                                       double defaultValue,
                                       double minValue,
                                       double maxValue
                                       );
    
    static Property createAlternativesProperty(const QString &propertyName,
                                               const QString &propertyDescription,
                                               const QStringList &alternatives,
                                               int deafultValueIndex
                                               );
    
    static Property createFilePathProperty(const QString &propertyName,
                                           const QString &propertyDescription,
                                           const QString &defaultValue
                                           );


    Type getPropertyType() const;
    QString getPropertyName() const;
    QString getPropertyDescription() const;
    double getValue() const;
    QStringList getAlternatives() const;
    double getMinValue() const;
    double getMaxValue() const;
    QString toString() const;
    QString getFilePath() const;
    QString getAnyValueAsString() const;

    friend class PropertyTests;
private:
    Type propertyType;
    QString propertyName;
    QString propertyDescription;
    double doubleValue;
    QStringList alternativesValue;
    QString filePathValue;
    double max, min;
    
    Property(const QString &propertyName,
             const QString &propertyDescription,
             double defaultValue,
             double minValue,
             double maxValue
             );
    
    Property(const QString &propertyName,
             const QString &propertyDescription,
             int defaultValue,
             int minValue,
             int maxValue
             );
    
    Property(const QString &propertyName,
             const QString &propertyDescription,
             const QString &defaultValue
             );
};

#endif // PROPERTY_H
