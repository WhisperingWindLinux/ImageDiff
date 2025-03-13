#ifndef MOCKMAINWINDOWCALLBACKS_H
#define MOCKMAINWINDOWCALLBACKS_H

#include <interfaces/mainwindowcallbacks.h>

// Mock for AMainWindowCallbacks
class MockMainWindowCallbacks : public AMainWindowCallbacks {
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

    QList<Property> getUpdatedPropertiesFromUser(QList<Property> defaultProperties) override {
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
};

#endif // MOCKMAINWINDOWCALLBACKS_H
