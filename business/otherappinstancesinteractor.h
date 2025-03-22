#ifndef OTHERAPPINSTANCESINTERACTOR_H
#define OTHERAPPINSTANCESINTERACTOR_H

#include "domain/valueobjects/images.h"
#include <QtCore/qprocess.h>

class OtherAppInstancesInteractorCallback;

/* If the user selects a part of the image while holding down Control,
 * we copy the selected part and open it in a new instance of the application
 * for further analysis. Parts from both images are copied.
 */

class OtherAppInstancesInteractor
{
public:
    OtherAppInstancesInteractor(OtherAppInstancesInteractorCallback *callback);
    ~OtherAppInstancesInteractor();

    void openNewAppInstance(ImagesPtr images);

private:
    QList<QProcess*> appInstances;
    OtherAppInstancesInteractorCallback *callback;

    void openNewAppInstance(const QString &firstFilePath, const QString &secondFilePath);
    std::optional<QString> saveImageInTempDir(const QPixmap &pixmap, const QString &fileName);
    void cleanupProcesses();
};

#endif // OTHERAPPINSTANCESINTERACTOR_H
