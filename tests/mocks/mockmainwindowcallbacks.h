#ifndef MOCKMAINWINDOWCALLBACKS_H
#define MOCKMAINWINDOWCALLBACKS_H

#include <interfaces/amainwindowcallbacks.h>

// Mock for AMainWindowCallbacks
class MockMainWindowCallbacks : public AMainWindowCallbacks {
public:
    virtual ~MockMainWindowCallbacks() {};
    void onImagesBeingComparedLoaded(QPixmap &pixmap1,
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
    void onComparisonImagesLoaded(QPixmap &, QString) override {}
    void onComparisonTextLoaded(QString) override {}
    void saveImageAs(QPixmap &pixmap, QString path) override {
        lastSavedFilePath = "";
        if (!pixmap.isNull()) {
            lastSavedFilePath = path;
        }
    }
    void onRgbValueUnderCursonChanged(QString, int, int, int) override {}

    QList<Property> getUpdatedPropertiesFromUser(QList<Property> defaultProperties) override {
        return updatedPropertiesFromUser;
    }

    QList<Property> updatedPropertiesFromUser;
    QString lastSavedFilePath;

    QPixmap m_pixmap1;
    QString m_path1;
    QPixmap m_pixmap2;
    QString m_path2;
    bool m_usePreviousImageGeometry;
};

#endif // MOCKMAINWINDOWCALLBACKS_H
