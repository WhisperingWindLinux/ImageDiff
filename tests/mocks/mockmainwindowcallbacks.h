#ifndef MOCKMAINWINDOWCALLBACKS_H
#define MOCKMAINWINDOWCALLBACKS_H

#include <QtCore/qdebug.h>

#include <domain/interfaces/mainwindowcallbacks.h>

// Mock for IMainWindowCallbacks
class MockMainWindowCallbacks : public IMainWindowCallbacks {
public:
    virtual ~MockMainWindowCallbacks() {};
    void displayImages(QPixmap &pixmap1,
                                     QString path1,
                                     QPixmap &pixmap2,
                                     QString path2) override
    {
        m_pixmap1 = pixmap1;
        m_path1 = path1;
        m_pixmap2 = pixmap2;
        m_path2 = path2;
    }
    void onImageResultFromComparatorReceived(QPixmap &, QString) override {}

    void showHelp(QString &) override {}

    void saveImage(QPixmap &pixmap, QString path) override {
        lastSavedFilePath = "";
        if (!pixmap.isNull()) {
            lastSavedFilePath = path;
        }
    }


    void onTextResultFromComparatorReceived(QString &,
                                            QString  ,
                                            QString  ,
                                            QString &,
                                            QString &) override {}

    void onColorUnderCursorChanged(ImagePixelColor, ImagePixelColor) override {}

    QList<Property> showImageProcessorPropertiesDialog(QString,
                                                 QString ,
                                                 QList<Property>) override {
        return updatedPropertiesFromUser;
    }

    void updateRecentFilesMenu() override {
        m_isUpdateRecentFilesMenuCalled = true;
    }

    void onDisplayedImagesShouldBeReplaced(QPixmap &, QPixmap &) override {}

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
    void showProgressDialog(QString, int) override { }
    bool wasCanceled() override { return true; }
    void onUpdateProgressValue(int) override { }
    void onMessage(QString) override { }
    void onError(QString) override { }
};

#endif // MOCKMAINWINDOWCALLBACKS_H
