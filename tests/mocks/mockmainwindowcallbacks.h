#ifndef MOCKMAINWINDOWCALLBACKS_H
#define MOCKMAINWINDOWCALLBACKS_H

#include <QtCore/qdebug.h>
#include <interfaces/mainwindowcallbacks.h>

// Mock for IMainWindowCallbacks
class MockMainWindowCallbacks : public IMainWindowCallbacks {
public:
    virtual ~MockMainWindowCallbacks() {};
    void onTwoImagesBeingComparedLoadedSuccessfully(QPixmap &pixmap1,
                                     QString path1,
                                     QPixmap &pixmap2,
                                     QString path2,
                                     bool usePreviousImageGeometry) override
    {
        m_pixmap1 = pixmap1;
        m_path1 = path1;
        m_pixmap2 = pixmap2;
        m_path2 = path2;
        m_usePreviousImageGeometry = usePreviousImageGeometry;
    }
    void onImageResultFromComparatorReceived(QPixmap &, QString) override {}
    void onTextResultFromComparatorReceived(QString) override {}
    void saveImage(QPixmap &pixmap, QString path) override {
        lastSavedFilePath = "";
        if (!pixmap.isNull()) {
            lastSavedFilePath = path;
        }
    }
    void onRgbValueUnderCursonChanged(RgbValue firstImageRgbValue, RgbValue secondImageRgbValue) override { }

    QList<Property> getUpdatedPropertiesFromUser(QString processorName,
                                                 QString processorDescription,
                                                 QList<Property> properties) override {
        return updatedPropertiesFromUser;
    }

    void updateRecentFilesMenu() override {
        m_isUpdateRecentFilesMenuCalled = true;
    }

    QList<Property> updatedPropertiesFromUser;
    QString lastSavedFilePath;

    QPixmap m_pixmap1;
    QString m_path1;
    QPixmap m_pixmap2;
    QString m_path2;
    bool m_usePreviousImageGeometry;
    bool m_isUpdateRecentFilesMenuCalled = false;

    // AMainWindowCallbacks interface

    // IProgressDialog interface
public:
    void showProgressDialog(QString caption, int totalSteps) override { }
    bool wasCanceled() override { return true; }
    void onUpdateProgressValue(int value) override { }
    void onMessage(QString message) override { }
    void onError(QString error) override { }
};

#endif // MOCKMAINWINDOWCALLBACKS_H
