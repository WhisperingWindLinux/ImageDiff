#ifndef SETPROPERTIESINTERACTORTESTS_H
#define SETPROPERTIESINTERACTORTESTS_H

#include <QObject>

class SetPropertiesInteractorTests : public QObject {
    Q_OBJECT

private slots:
    void testAllowUserToSetPropertiesIfNeed_NoDefaultProperties();
    void testAllowUserToSetPropertiesIfNeed_UserUpdatesProperties();
    void testAllowUserToSetPropertiesIfNeed_UserDoesNotUpdateProperties();
};


#endif // SETPROPERTIESINTERACTORTESTS_H
