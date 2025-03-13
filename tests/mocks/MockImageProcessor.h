#ifndef MOCKIMAGEPROCESSOR_H
#define MOCKIMAGEPROCESSOR_H

#include <qstring.h>

#include <interfaces/imageprocessor.h>


// Mock class for AImageProcessor to simulate processors
class MockImageProcessor : public AImageProcessor {
public:
    MockImageProcessor() {}
    MockImageProcessor(QString name, QString hotkey, QString description, ImageProcessorType type)
        : m_name(name), m_hotkey(hotkey), m_description(description), m_type(type) {}
    virtual ~MockImageProcessor() {};
    QString name() override { return m_name; }
    QString hotkey() override { return m_hotkey; }
    QString description() override { return m_description; }
    ImageProcessorType getType() const override { return m_type; }

    QList<Property> getDefaultProperties() const override {
        return defaultProperties;
    }

    void setProperties(QList<Property> properties) override {
        updatedProperties = properties;
        wasSetPropertiesCalled = true;
    }

    void setDefaultProperties(QList<Property> properties) {
        defaultProperties = properties;
    }

    void reset() override {
        wasSetPropertiesCalled = false;
        updatedProperties.clear();
        defaultProperties.clear();
    }

    bool wasSetPropertiesCalled = false;
    QList<Property> updatedProperties;
    QList<Property> defaultProperties;

    QString m_name;
    QString m_hotkey;
    QString m_description;
    ImageProcessorType m_type;
};


#endif // MOCKIMAGEPROCESSOR_H
