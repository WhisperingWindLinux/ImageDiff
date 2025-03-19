#include "property.h"

#include <stdexcept>
#include <float.h>

// Constructor for real property
Property::Property(QString propertyName,
                   QString propertyDescription,
                   double defaultValue,
                   double minValue,
                   double maxValue)
    : propertyType(Type::Real),
    propertyName(propertyName),
    propertyDescription(propertyDescription),
    doubleValue(defaultValue),
    max(maxValue),
    min(minValue) {}

// Constructor for integer property
Property::Property(QString propertyName,
                   QString propertyDescription,
                   int defaultValue,
                   int minValue,
                   int maxValue)
    : propertyType(Type::Integer),
    propertyName(propertyName),
    propertyDescription(propertyDescription),
    doubleValue(static_cast<double>(defaultValue)),
    max(static_cast<double>(maxValue)),
    min(static_cast<double>(minValue)) {}

Property::Property(QString propertyName,
                   QString propertyDescription,
                   QString defaultValue)
    : propertyType(Type::FilePath),
     propertyName(propertyName),
     propertyDescription(propertyDescription),
     filePathValue(defaultValue) {}


// Static method to create an integer property without min/max constraints
Property Property::createIntProperty(QString propertyName,
                                     QString propertyDescription,
                                     int defaultValue
                                     )
{
    return Property(propertyName,
                    propertyDescription,
                    defaultValue,
                    INT_MIN,
                    INT_MAX
                    );
}


// Static method to create an integer property with min/max constraints
Property Property::createIntProperty(QString propertyName,
                                     QString propertyDescription,
                                     int defaultValue,
                                     int minValue,
                                     int maxValue
                                     )
{
    return Property(propertyName,
                    propertyDescription,
                    defaultValue,
                    minValue,
                    maxValue
                    );
}


// Static method to create a real property without min/max constraints
Property Property::createRealProperty(QString propertyName,
                                      QString propertyDescription,
                                      int defaultValue
                                      )
{
    return Property(propertyName,
                    propertyDescription,
                    static_cast<double>(defaultValue),
                    -DBL_MAX,
                    DBL_MAX
                    );
}


// Static method to create a real property with min/max constraints
Property Property::createRealProperty(QString propertyName,
                                      QString propertyDescription,
                                      double defaultValue,
                                      double minValue,
                                      double maxValue
                                      )
{
    return Property(propertyName,
                    propertyDescription,
                    defaultValue,
                    minValue,
                    maxValue
                    );
}

// Static method to create a string property with alternativesValue
Property Property::createAlternativesProperty(QString propertyName,
                                        QString propertyDescription,
                                        QStringList alternatives,
                                        int defaultValueIndex
                                        )
{
    if (alternatives.isEmpty()) {
        throw std::invalid_argument("Alternatives list cannot be empty");
    }

    if (defaultValueIndex < 0 || defaultValueIndex >= alternatives.size()) {
        throw std::out_of_range("Default value index is out of range");
    }

    Property prop(std::move(propertyName), std::move(propertyDescription), 0.0, 0.0, 0.0);
    prop.propertyType = Type::Alternatives;
    prop.alternativesValue = alternatives;
    prop.doubleValue = static_cast<double>(defaultValueIndex);

    return prop;
}

Property Property::createFilePathProperty(QString propertyName,
                                          QString propertyDescription,
                                          QString defaultValue
                                          )
{
    return Property(propertyName, propertyDescription, defaultValue);
}

// Getters
Property::Type Property::getPropertyType() const {
    return propertyType;
}

QString Property::getPropertyName() const {
    return propertyName;
}

QString Property::getPropertyDescription() const {
    return propertyDescription;
}

double Property::getValue() const {
    return doubleValue;
}

QString Property::getFilePath() const {
    return filePathValue;
}

QStringList Property::getAlternatives() const {
    if (propertyType != Type::Alternatives) {
        throw std::logic_error("This property type does not have alternatives");
    }
    return alternativesValue;
}

double Property::getMinValue() const {
    if (propertyType == Type::Alternatives || propertyType == Type::FilePath) {
        throw std::logic_error("This property type does not have min/max values");
    }
    return min;
}

QString Property::getAnyValueAsString() const {
    switch (propertyType) {
    case Type::Integer:
        return QString::number((int)doubleValue);
    case Type::Real:
        return QString::number(doubleValue);
    case Type::Alternatives:
        if (alternativesValue.size() == 0) {
            return "";
        }
        if ((int)doubleValue > alternativesValue.size() || (int)doubleValue < 0) {
            return "";
        }
        return alternativesValue[(int)doubleValue];
    case Type::FilePath:
        return filePathValue;
    }
    return "";
}

double Property::getMaxValue() const {
    if (propertyType == Type::Alternatives || propertyType == Type::FilePath) {
        throw std::logic_error("This property type does not have min/max values");
    }
    return max;
}

QString Property::toString() const {
    QString result = "Property: \n";
    result += "  Name: " + getPropertyName() + "\n";
    result += "  Description: " + getPropertyDescription() + "\n";

    switch (getPropertyType()) {
    case Type::Integer:
        result += "  Type: Integer\n";
        result += "  Value: " + QString::number(static_cast<int>(getValue())) + "\n";
        result += "  Min Value: " + QString::number(static_cast<int>(getMinValue())) + "\n";
        result += "  Max Value: " + QString::number(static_cast<int>(getMaxValue())) + "\n";
        break;

    case Type::Real:
        result += "  Type: Real\n";
        result += "  Value: " + QString::number(getValue(), 'f', 2) + "\n";
        result += "  Min Value: " + QString::number(getMinValue(), 'f', 2) + "\n";
        result += "  Max Value: " + QString::number(getMaxValue(), 'f', 2) + "\n";
        break;

    case Type::Alternatives:
        result += "  Type: Alternatives\n";
        result += "  Value Index: " + QString::number(static_cast<int>(getValue())) + "\n";
        result += "  Alternatives: [" + getAlternatives().join(", ") + "]\n";
        break;

    case Type::FilePath:
        result += "  Type: FilePath\n";
        result += "  Value: " + filePathValue + "\n";
        break;
    }

    return result;
}
