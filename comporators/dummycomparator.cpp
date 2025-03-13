#include "dummycomparator.h"
#include <QtCore/qdebug.h>


QString DummyComporator::name() {
    return "Dummy comparator";
}

QString DummyComporator::hotkey() {
    return "x";
}

QString DummyComporator::description() {
    return QString("It's very simple and useless dummy comparator. It is used for testing purposes only. ") +
           "It's very simple and useless dummy comparator. It is used for testing purposes only. " +
           "It's very simple and useless dummy comparator. It is used for testing purposes only.";
}

std::shared_ptr<ComparisonResultVariant> DummyComporator::compare(ComparableImage first, ComparableImage second) {
    return {};
}

QList<Property> DummyComporator::getDefaultProperties() const {

    auto prop1 = Property::createIntProperty("Int property", "Int property description.", 42, 1, 100);
    auto prop2 = Property::createRealProperty("Real property", "Real property description.", 33.5, 10, 50);

    QList<QString> alternatives = { "First", "Second", "Third", "Fourth" };

    auto prop3 = Property::createAlternativesProperty("Real property", "Real property description.", alternatives, 1);

    auto prop4 = Property::createFilePathProperty("File path property", "This property represents a file path", "");

    QList<Property> properties = { prop1, prop2, prop3, prop4 };

    return properties;

}

void DummyComporator::setProperties(QList<Property> properties) {
    for (auto iter = properties.begin(); iter != properties.end(); ++iter) {
        qDebug() << iter->toString();
    }
}
