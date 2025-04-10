#ifndef APPLEICONMAKER_H
#define APPLEICONMAKER_H

#include <domain/interfaces/business/ifilter.h>

class AppleIconMaker : public IFilter
{
public:
    AppleIconMaker();

public:
    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    QString getFullName() const override;
    QImage filter(const QImage &image) override;

private:
    QImage createImageWithText(const QImage &sourceImage,
                               const QString &text);
};

#endif // APPLEICONMAKER_H
