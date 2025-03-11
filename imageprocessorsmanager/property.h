#ifndef PROPERTY_H
#define PROPERTY_H

#include <qstring.h>
#include <QStringList>

using namespace std;

class Property {
public:
    enum class Type { Integer, Real, Alternatives };

    static Property createIntProperty(QString propertyName,
                                      QString propertyDescription,
                                      int defaultValue
                                      );

    static Property createIntProperty(QString propertyName,
                                      QString propertyDescription,
                                      int defaultValue,
                                      int minValue,
                                      int maxValue
                                      );

    static Property createRealProperty(QString propertyName,
                                      QString propertyDescription,
                                      int defaultValue
                                      );

    static Property createRealProperty(QString propertyName,
                                       QString propertyDescription,
                                       double defaultValue,
                                       double minValue,
                                       double maxValue
                                       );

    static Property createStringProperty(QString propertyName,
                                         QString propertyDescription,
                                         QStringList alternatives,
                                         int deafultValueIndex
                                         );


    Type getPropertyType() const;
    QString getPoropertyName() const;
    QString getPropertyDescription() const;
    double getValue() const;
    QStringList getAlternatives() const;
    double getMinValue() const;
    double getMaxValue() const;

    QString toString() const;

private:
    Type propertyType;
    QString propertyName;
    QString propertyDescription;
    double defaultValue;
    QStringList alternatives;
    double max, min;

    Property(QString propertyName,
             QString propertyDescription,
             double defaultValue,
             double minValue,
             double maxValue
             );

    Property(QString propertyName,
             QString propertyDescription,
             int defaultValue,
             int minValue,
             int maxValue
             );
};

#endif // PROPERTY_H
