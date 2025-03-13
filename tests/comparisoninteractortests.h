#ifndef COMPARISONINTERACTORTESTS_H
#define COMPARISONINTERACTORTESTS_H

#include <QObject>

#include <tests/mocks/mockmainwindowcallbacks.h>

#include <interactors/comparisoninteractor.h>

class ComparisonInteractorTests : public QObject {
    Q_OBJECT

private:
    QStringList createdFilesPath;

    MockMainWindowCallbacks *mockCallbacks;
    ComparisonInteractor *interactor;
    void verifyLoadedImages(const QString &image1Path, const QString &image2Path, bool usePreviousGeometry);
    QString createTempImage(const QString &fileName, const QSize &size, const QColor &color);

    QString createTempImageWithRandomName(const QSize &size, const QColor &color);
private slots:
    void init();
    void cleanup();

    void testLoadImagesBeingComparedNotSameSize_Failed();
    void testLoadImagesBeingCompared();
    void testReloadImagesFromDisk();
    void testClear();
    void testConstructor();
    void testSaveImage_ComparisonImage();
    void testSaveImage_SecondImage();
    void testSaveImage_FirstImage();
    void testSaveImage_FirstImageArea();
    void testSaveImage_SecondImageArea();
    void testSaveImage_ComparisonImageArea();
    void testCallUpdateRecentFilesMenu();
    void testLoadImagesBeingCompared_OpenFilesFromDrop();
    void testLoadImagesBeingCompared_RecentFiles();
    void testLoadImagesBeingCompared_OpenFilesFromDropIncorrectUrl();
};


#endif // COMPARISONINTERACTORTESTS_H
