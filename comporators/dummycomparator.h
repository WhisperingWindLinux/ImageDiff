#ifndef DUMMYCOMPARATOR_H
#define DUMMYCOMPARATOR_H

#include <interfaces/comporator.h>


class DummyComporator : public IComparator
{
public:

    DummyComporator() = default;
    virtual ~DummyComporator() = default;

    // AComparator interface

    QString name() override;
    QString hotkey() override;
    QString description() override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property>) override;
};

#endif // DUMMYCOMPARATOR_H
