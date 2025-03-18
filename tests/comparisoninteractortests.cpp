#include <QtTest>
#include "comparisoninteractortests.h"
#include "testutils.h"
#include <business/imageanalysis/comparisoninteractor.h>
#include <presentation/presenters/recentfilespresenter.h>

void ComparisonInteractorTests::init() {
    // Set up mock callbacks and interactor before each test
    mockCallbacks = new MockMainWindowCallbacks();
    interactor = new ComparisonInteractor(mockCallbacks);
}

void ComparisonInteractorTests::cleanup() {
    // Clean up resources after each test
    delete interactor;
    delete mockCallbacks;
}

void ComparisonInteractorTests::testConstructor() {
    // Verify that the interactor is initialized properly
    QVERIFY(interactor != nullptr);
}

void ComparisonInteractorTests::testClear() {
    // Arrange: Prepare test images and load them into the interactor
    QString image1Path = createTempImage("testClear1.png", QSize(20, 20), Qt::white);
    QString image2Path = createTempImage("testClear2.png", QSize(20, 20), Qt::white);

    interactor->loadTwoImagesBeingCompared(image1Path, image2Path, false, false, true);

    // Act: Clear the interactor state
    interactor->onImagesClosed();

    // Assert: Verify that paths and pixmaps are cleared
    QCOMPARE(interactor->firstImagePath.isEmpty(), true);
    QCOMPARE(interactor->secondImagePath.isEmpty(), true);
    QVERIFY(interactor->firstPixmap.isNull());
    QVERIFY(interactor->secondPixmap.isNull());
}

void ComparisonInteractorTests::testReloadImagesFromDisk() {
    // Arrange: Prepare test images and load them into the interactor
    QString image1Path = createTempImage("image1Reload.png", QSize(20, 20), Qt::white);
    QString image2Path = createTempImage("image2Reload.png", QSize(20, 20), Qt::white);

    interactor->loadTwoImagesBeingCompared(image1Path, image2Path, false, false, true);

    // Initial state verification
    verifyLoadedImages(image1Path, image2Path, false);

    // Act: Reload images from disk
    interactor->reloadImagesFromDisk();

    // Assert: Verify that images are reloaded with updated geometry flag
    verifyLoadedImages(image1Path, image2Path, true);
}

void ComparisonInteractorTests::testLoadImagesBeingCompared() {
    // Arrange: Prepare test images
    QString image1Path = createTempImage("image1Load.png", QSize(20, 20), Qt::white);
    QString image2Path = createTempImage("image2Load.png", QSize(20, 20), Qt::white);

    // Act: Load images into the interactor
    interactor->loadTwoImagesBeingCompared(image1Path, image2Path, true, false, true);

    // Assert: Verify that images are loaded correctly with geometry flag set to true
    verifyLoadedImages(image1Path, image2Path, true);
}

void ComparisonInteractorTests::testLoadImagesBeingComparedNotSameSize_Failed() {
    // Arrange: Prepare test images with different sizes
    QString image1Path = createTempImage("image1DifferentSize.png", QSize(20, 20), Qt::white);
    QString image2Path = createTempImage("image2DifferentSize.png", QSize(30, 30), Qt::white);

    // Act & Assert: Loading images of different sizes should fail
    QVERIFY_EXCEPTION_THROWN(
        interactor->loadTwoImagesBeingCompared(image1Path, image2Path, true, false, true),
        std::runtime_error
        );
}

// Helper function to create a temporary image file
QString ComparisonInteractorTests::createTempImage(const QString& fileName, const QSize& size, const QColor& color) {
    QPixmap pixmap(size);
    pixmap.fill(color);
    QString resultPath = FileUtils::savePixmapToTempDir(pixmap, fileName);
    createdFilesPath.append(resultPath);
    return resultPath;
}

// Helper function to verify loaded images and their associated states
void ComparisonInteractorTests::verifyLoadedImages(const QString& image1Path, const QString& image2Path, bool usePreviousGeometry) {
    QCOMPARE(interactor->firstImagePath, image1Path);
    QCOMPARE(interactor->secondImagePath, image2Path);

    QCOMPARE(mockCallbacks->m_path1, image1Path);
    QCOMPARE(mockCallbacks->m_path2, image2Path);

    QCOMPARE(mockCallbacks->m_usePreviousImageGeometry, usePreviousGeometry);

    QPixmap expectedPixmap1 = QPixmap(image1Path);QPixmap expectedPixmap2 = QPixmap(image2Path);

    QCOMPARE(mockCallbacks->m_pixmap1.toImage(), expectedPixmap1.toImage());
    QCOMPARE(mockCallbacks->m_pixmap2.toImage(), expectedPixmap2.toImage());
}

void ComparisonInteractorTests::testSaveImage_FirstImage() {
    // Arrange;
    QString path = FileUtils::generateRandomPngFileNameInTempDir();
    interactor->firstImagePath = path;
    SaveImageInfo info;
    info.saveImageInfoType = SaveImageInfoType::FirstImage;
    info.image = QPixmap::fromImage(QImage(10, 10, QImage::Format_RGB32));

    // Act
    interactor->saveImage(info);

    // Assert
    QCOMPARE(mockCallbacks->lastSavedFilePath, path);
    QVERIFY(!mockCallbacks->lastSavedFilePath.isNull());
}

void ComparisonInteractorTests::testSaveImage_SecondImage() {
    // Arrange;
    QString path = FileUtils::generateRandomPngFileNameInTempDir();
    interactor->secondImagePath = path;
    SaveImageInfo info;
    info.saveImageInfoType = SaveImageInfoType::SecondImage;
    info.image = QPixmap::fromImage(QImage(10, 10, QImage::Format_RGB32));

    // Act
    interactor->saveImage(info);

    // Assert
    QCOMPARE(mockCallbacks->lastSavedFilePath, path);
    QVERIFY(!mockCallbacks->lastSavedFilePath.isNull());
}

void ComparisonInteractorTests::testSaveImage_ComparisonImage() {
    // Arrange;
    QString path1 = FileUtils::generateRandomPngFileNameInTempDir();
    QString path2 = FileUtils::generateRandomPngFileNameInTempDir();
    interactor->firstImagePath = path1;
    interactor->secondImagePath = path2;
    SaveImageInfo info;
    info.saveImageInfoType = SaveImageInfoType::ComparisonImage;
    info.image = QPixmap::fromImage(QImage(10, 10, QImage::Format_RGB32));

    // Act
    interactor->saveImage(info);

    // Assert
    QString file1Name = QFileInfo(path1).baseName();
    QString file2Name = QFileInfo(path2).baseName();

    QString expectedFileName = QString("%1_vs_%2_comparison%3").arg(file1Name, file2Name, ".png");
    QString tmpDirPath = QFileInfo(path1).absoluteDir().absolutePath();
    QString fullExpectedPath = QDir(tmpDirPath).filePath(expectedFileName);

    QCOMPARE(mockCallbacks->lastSavedFilePath, fullExpectedPath);
    QVERIFY(!mockCallbacks->lastSavedFilePath.isNull());
}

void ComparisonInteractorTests::testSaveImage_FirstImageArea() {
    // Arrange;
    QString path1 = FileUtils::generateRandomPngFileNameInTempDir();
    QString path2 = FileUtils::generateRandomPngFileNameInTempDir();
    interactor->firstImagePath = path1;
    interactor->secondImagePath = path2;
    SaveImageInfo info;
    info.saveImageInfoType = SaveImageInfoType::FirstImageArea;
    info.image = QPixmap::fromImage(QImage(10, 10, QImage::Format_RGB32));

    // Act
    interactor->saveImage(info);

    // Assert
    QString fileName = QFileInfo(path1).baseName();
    QString expectedFileName = QString("%1_area.png").arg(fileName);
    QString tmpDirPath = QFileInfo(path1).absoluteDir().absolutePath();
    QString fullExpectedPath = QDir(tmpDirPath).filePath(expectedFileName);

    QCOMPARE(mockCallbacks->lastSavedFilePath, fullExpectedPath);
    QVERIFY(!mockCallbacks->lastSavedFilePath.isNull());
}

void ComparisonInteractorTests::testSaveImage_SecondImageArea() {
    // Arrange;
    QString path1 = FileUtils::generateRandomPngFileNameInTempDir();
    QString path2 = FileUtils::generateRandomPngFileNameInTempDir();
    interactor->firstImagePath = path1;
    interactor->secondImagePath = path2;
    SaveImageInfo info;
    info.saveImageInfoType = SaveImageInfoType::SecondImageArea;
    info.image = QPixmap::fromImage(QImage(10, 10, QImage::Format_RGB32));

    // Act
    interactor->saveImage(info);

    // Assert
    QString fileName = QFileInfo(path2).baseName();
    QString expectedFileName = QString("%1_area.png").arg(fileName);
    QString tmpDirPath = QFileInfo(path2).absoluteDir().absolutePath();
    QString fullExpectedPath = QDir(tmpDirPath).filePath(expectedFileName);

    QCOMPARE(mockCallbacks->lastSavedFilePath, fullExpectedPath);
    QVERIFY(!mockCallbacks->lastSavedFilePath.isNull());
}

void ComparisonInteractorTests::testSaveImage_ComparisonImageArea() {
    // Arrange;
    QString path1 = FileUtils::generateRandomPngFileNameInTempDir();
    QString path2 = FileUtils::generateRandomPngFileNameInTempDir();
    interactor->firstImagePath = path1;
    interactor->secondImagePath = path2;
    SaveImageInfo info;
    info.saveImageInfoType = SaveImageInfoType::ComparisonImageArea;
    info.image = QPixmap::fromImage(QImage(10, 10, QImage::Format_RGB32));

    // Act
    interactor->saveImage(info);

    // Assert
    QString file1Name = QFileInfo(path1).baseName();
    QString file2Name = QFileInfo(path2).baseName();

    QString expectedFileName = QString("%1_vs_%2_area_comparison.png").arg(file1Name, file2Name);
    QString tmpDirPath = QFileInfo(path1).absoluteDir().absolutePath();
    QString fullExpectedPath = QDir(tmpDirPath).filePath(expectedFileName);

    QCOMPARE(mockCallbacks->lastSavedFilePath, fullExpectedPath);
    QVERIFY(!mockCallbacks->lastSavedFilePath.isNull());
}

void ComparisonInteractorTests::testCallUpdateRecentFilesMenu() {
    // Arrange: Prepare test images and load them into the interactor
    QString image1Path = createTempImage("image1.png", QSize(20, 20), Qt::white);
    QString image2Path = createTempImage("image2.png", QSize(20, 20), Qt::white);

    interactor->loadTwoImagesBeingCompared(image1Path, image2Path, false, false, true);

    // Initial state verification
    verifyLoadedImages(image1Path, image2Path, false);

    QVERIFY(mockCallbacks->m_isUpdateRecentFilesMenuCalled);
}

void ComparisonInteractorTests::testLoadImagesBeingCompared_OpenFilesFromDrop() {

    // Arrange: Prepare test images and load them into the interactor
    QString image1Path = createTempImage("image1.png", QSize(20, 20), Qt::white);
    QString image2Path = createTempImage("image2.png", QSize(20, 20), Qt::white);

    QList<QUrl> urls = { QUrl("file://"+image1Path),  QUrl("file://"+image2Path) };
    interactor->loadTwoImagesBeingCompared(urls);

    QCOMPARE(image1Path, mockCallbacks->m_path1);
    QCOMPARE(image2Path, mockCallbacks->m_path2);
}

void ComparisonInteractorTests::testLoadImagesBeingCompared_OpenFilesFromDropIncorrectUrl() {

    // Arrange: Prepare test images and load them into the interactor
    QString image1Path = createTempImage("image1.png", QSize(20, 20), Qt::white);
    QString image2Path = createTempImage("image2.png", QSize(20, 20), Qt::white);

    QList<QUrl> urls = { QUrl(image1Path),  QUrl(image2Path) };
    interactor->loadTwoImagesBeingCompared(urls);

    QCOMPARE("", mockCallbacks->m_path1);
    QCOMPARE("", mockCallbacks->m_path2);
}

// Test the function that opens images from the recent files menu.
// The menu item is formatted as "path to file 1 -> path to file 2".
void ComparisonInteractorTests::testLoadImagesBeingCompared_RecentFiles() {

    // Arrange: Prepare test images and load them into the interactor
    QString image1Path = createTempImage("image1.png", QSize(20, 20), Qt::white);
    QString image2Path = createTempImage("image2.png", QSize(20, 20), Qt::white);

    RecentFilesPresenter formater;
    QString pair1 = formater.pairToString({image1Path, image2Path});
    interactor->loadTwoImagesBeingCompared(pair1, true);

    QCOMPARE(image1Path, mockCallbacks->m_path1);
    QCOMPARE(image2Path, mockCallbacks->m_path2);

    QString pair2 = formater.pairToString({"", ""});

    QVERIFY_EXCEPTION_THROWN(interactor->loadTwoImagesBeingCompared(pair2, true), std::runtime_error);

    QCOMPARE(image1Path, mockCallbacks->m_path1);
    QCOMPARE(image2Path, mockCallbacks->m_path2);
}








