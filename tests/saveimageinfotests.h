#ifndef SAVEIMAGEINFOTESTS_H
#define SAVEIMAGEINFOTESTS_H

#include <QObject>

class SaveImageInfoTests : public QObject {
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testParameterizedConstructor();
    void testSaveImageInfoType();
    void testPixmapAssignment();
};


#endif // SAVEIMAGEINFOTESTS_H
