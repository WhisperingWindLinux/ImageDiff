#ifndef COMPARABLEIMAGETESTS_H
#define COMPARABLEIMAGETESTS_H

#include <QObject>

class ComparableImageTests : public QObject {
    Q_OBJECT

private slots:
    void testConstructorWithQImage();
    void testConstructorWithQPixmap();
    void testGetImage();
    void testGetName();
};

#endif // COMPARABLEIMAGETESTS_H
